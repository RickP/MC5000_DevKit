#include "serialcommunication.h"
#include <QSerialPortInfo>
#include <QThread>


static void printHex(uint8_t byte) {
    char s[3];
    const uint8_t hex_lookup[] = "0123456789ABCDEF";
    s[0] = hex_lookup[byte >> 4];
    s[1] = hex_lookup[byte & 0x0f];
    s[2] = '\0';
    qDebug() << s;
}

static uint8_t checksum (QByteArray bytes, uint8_t size) {
    unsigned char chk = 0;
    for (int i=0; i<size; i++) {
        chk -= bytes.at(i);
    }
    return chk >> 2;
}

SerialCommunication::SerialCommunication(QObject *parent) : QObject(parent)
{
    loadPorts();
}

void SerialCommunication::loadPorts() {
    m_serialports.clear();
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {
        QString portName = info.portName();
        if (portName.contains("tty", Qt::CaseInsensitive) && portName.contains("usb", Qt::CaseInsensitive)) {
            m_serialports.append(portName);
        }
    }
    if (m_serialports.length() == 1) {
        connect(m_serialports.at(0));
    }
    emit serialportsChanged();
}

SerialCommunication::~SerialCommunication()
{
    m_serial.close();
}

void SerialCommunication::connect(QString port) {
    m_serial.close();
    m_serial.setPortName(port);
    m_serial.setBaudRate(19200);
    m_serial.open(QIODevice::ReadWrite);

    if (!m_serial.isOpen()) return;

    // check number of mcus
    for (int i=0;; i++) {
        writeSerialByte(0x31+i);
        if (m_serial.waitForReadyRead(500)) {
            m_serial.readAll();
            m_mcuConnections++;
        } else {
            break;
        }
    }
    QStringList emptyList;
    for (int i=0; i < m_mcuConnections; i++) {
        emptyList.append("");
    };
    upload(emptyList);
    emit mcuConnectionChanged();
}

void SerialCommunication::upload(QStringList codeList) {
    m_errorMessage = "";
    for (int i = 0; i < codeList.size(); i++) {
        m_isProgrammed[i] = false;
        emit isProgrammedChanged();
        QByteArray output;
        if (codeList.at(i) != "") {
            QStringList lines = codeList.at(i).split('\n', Qt::SkipEmptyParts);
            QStringList labels;


            for (int j = 0; j < lines.size(); j++) {
                QStringList parts = lines.at(j).split(QRegExp("\\s+"), Qt::SkipEmptyParts);
                if (parts.size() == 0) continue;
                if (parts.at(0).endsWith(LABEL_MARKER) and !labels.contains(parts.at(0))) {
                    labels.append(parts.at(0));
                }
            }

            for (int j = 0; j < lines.size(); j++) {
                QStringList parts = lines.at(j).split(QRegExp("\\s+"), Qt::SkipEmptyParts);
                char addToCommand = 0;
                for (int k = 0; k < parts.size(); k++) {
                    QString part = parts.at(k);

                    if (part.startsWith(COMMENT_MARKER)) {
                        break;
                    }

                    if (part.endsWith(LABEL_MARKER)) {
                        if (k != 0) {
                            m_errorMessage = QString("A label has to be in front (Line %1)").arg(j+1);
                            emit errorMessageChanged();
                            return;
                        } else {
                            output.append(LABEL_HEXCODE);
                            output.append(labels.lastIndexOf(part));
                            part = parts.at(++k);
                        }
                    }

                    if (part == '+' || part == '-') {
                        if (k != 0) {
                            m_errorMessage = QString("+/- has to be in front (Line %1)").arg(j+1);
                            emit errorMessageChanged();
                            return;
                        } else {
                            addToCommand = (part == '+') ? 0x01 : 0x02;
                        }
                    } else if (commands.contains(part)) {
                        const auto commandConfig = commands.value(part);
                        const char command = commandConfig.first;
                        const QStringList parameters = commandConfig.second;

                        if (parts.length() < k + 1 + parameters.length()) {
                            m_errorMessage = QString("Command %1 needs %2 parameters (Line %3)").arg(part).arg(parameters.length()).arg(j+1);
                            emit errorMessageChanged();
                            return;
                        }

                        output.append(command | addToCommand);

                        for (int l=0; l < parameters.length(); l++) {
                            if (parameters.at(l) == "ri") {
                                char* value16 = encode16BitVal(parts.at(k+l+1));
                                if (value16[0] == 0x7F) {
                                    m_errorMessage = QString("Unknown register %1 (Line %2)").arg(parts.at(k+l+1)).arg(j+1);
                                    emit errorMessageChanged();
                                    return;
                                }
                                output.append(value16, 2);
                            }
                            else if (parameters.at(l) == "r") {
                                char value8 = encode8BitVal(parts.at(k+l+1));
                                if (value8 == 0x7F) {
                                    m_errorMessage = QString("Unknown register %1 (Line %2)").arg(parts.at(k+l+1)).arg(j+1);
                                    emit errorMessageChanged();
                                    return;
                                }
                                output.append(value8);
                            }
                            else if (parameters.at(l) == "l") {
                                QString label = parts.at(k+l+1) + ":";
                                output.append(labels.indexOf(label));
                            }
                        }
                        break;
                    } else {
                        m_errorMessage = QString("Unknown command %1 (Line %2)").arg(part).arg(j+1);
                        emit errorMessageChanged();
                        return;
                    }
                }
            }
        }

        // Write program
        if (m_serial.isOpen()) {
            // header
            bool program_success = false;
            int tries = 0;
            while (!program_success && tries++ <= UPLOAD_RETRIES) {
                char data = 0;
                writeSerialByte(START_CHAR);
                writeSerialByte(0x31 + i);
                for (int x=0; x < output.length(); x++) {
                    writeSerialByte(output.at(x));
                }
                writeSerialByte(checksum(output, output.length()));
                writeSerialByte(END_CHAR);
                // check programming state
                if (m_serial.waitForReadyRead(500)) {
                    if (m_serial.read(&data, 1) == 1 && data == 0x31 + i) {
                        program_success = true;
                    }
                    m_serial.readAll();
                }
            }
        } else {
            m_errorMessage = QString("No serial connection!");
            emit errorMessageChanged();
            return;
        }
    }
}

void SerialCommunication::writeSerialByte(char byte, bool debug) {
    m_serial.write(&byte, 1);
    m_serial.waitForBytesWritten(100);
    QThread::msleep(SERIAL_DELAY);

    if (debug) {
        printHex(byte);
    }
}

char SerialCommunication::encode8BitVal(QString parameter) {
    if (registerEncodings.contains(parameter)) {
        return registerEncodings.value(parameter);
    }
    return START_CHAR; // acc is default register
}

char* SerialCommunication::encode16BitVal(QString parameter) {
    static char ret[2];
    ret[0] = encode8BitVal(parameter);
    if (ret[0] != START_CHAR) {
        ret[1] = 0x00;
    }
    else {
        bool ok;
        int16_t dec = parameter.toInt(&ok, 10);
        if (ok) {
            if (dec < -999) {
                dec = -999;
            } else if (dec > 999) {
                dec = 999;
            }
            dec += 1000;
            ret[0] = dec >> 6;
            ret[1] = dec & 0x003F;
        }
    }
    return ret;
}

void SerialCommunication::updateRegisters() {
    if (m_mcuConnections && m_serial.isOpen()) {
        bool updateValues = true;
        m_accRegisters.clear();
        m_datRegisters.clear();
        for (int i=0; i < m_mcuConnections; i++) {
            m_serial.readAll();
            writeSerialByte(0x31+i, false);
            if (m_serial.waitForReadyRead(100)) {
                char data[5];
                int bytes_read = m_serial.read(data, 5);
                if (bytes_read == 5 && (data[4] & 0x3F) == checksum(QByteArray(data, 4), 4)) {
                    m_isProgrammed[i] = data[4] & 0x40;
                    int acc = data[0] << 7;
                    acc |= data[1];
                    m_accRegisters.append(acc-1000);
                    int dat = data[2] << 7;
                    dat |= data[3];
                    m_datRegisters.append(dat-1000);
                } else {
                    QThread::msleep(SERIAL_DELAY*2);
                }
            } else {
                updateValues = false;
                break;
            }
        }
        if (updateValues) {
            emit datRegistersChanged();
            emit accRegistersChanged();
            emit isProgrammedChanged();
        }
    }
}

QString SerialCommunication::errorMessage() {
    return m_errorMessage;
}

QList<int> SerialCommunication::datRegisters() {
    return m_datRegisters;
}

QList<int> SerialCommunication::accRegisters() {
    return m_accRegisters;
}

QStringList SerialCommunication::serialports() {
    return m_serialports;
}

int SerialCommunication::mcuConnections() const {
    return m_mcuConnections;
}

QList<bool> SerialCommunication::isProgrammed() {
    return m_isProgrammed;
}
