#include "serialcommunication.h"
#include <QSerialPortInfo>
#include <QThread>
#include <QTimer>


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

SerialCommunication::SerialCommunication(QObject *parent) :
    QObject(parent),
    m_serial(new QSerialPort(this))
{
    loadPorts();
    connect(m_serial, &QSerialPort::readyRead, this, &SerialCommunication::readData);
}


void SerialCommunication::loadPorts() {
    m_serialports.clear();
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {
        m_serialports.append(info.portName());
    }
    emit serialportsChanged();
}

SerialCommunication::~SerialCommunication()
{
    m_serial->close();
}

void SerialCommunication::readData()
{
    const QByteArray data = m_serial->readAll();
    if (data.length() == 3 && data.at(0) == 0x7F) {
        int mcuId = data.at(1) - 0x31;
        if (data.at(2) == 1) {
             m_uploadDone[mcuId] = true;
            if (mcuId < (m_mcuConnections-1)) {
                upload(mcuId+1);
            }
        }
    } else if (data.length() == 6 && (data.at(5) & 0x3F) == checksum(data.right(5), 4)) {
        int mcuId = data.at(0) - 0x31;

        if (!m_uploadDone.at(mcuId)) return;

        if (m_mcuConnections < 2) {
            m_mcuConnections = 2;
            m_codeList.empty();
            for (int i=0; i<m_mcuConnections; i++) {
                m_codeList.append("");
            }
            emit mcuConnectionChanged();
            upload(0);
            return;
        }
        m_isProgrammed[mcuId] = data.at(5) & 0x40;
        int acc = data.at(1) << 7;
        acc |= data.at(2);
        m_accRegisters[mcuId] = acc-1000;
        int dat = data.at(3) << 7;
        dat |= data.at(4);
        m_datRegisters[mcuId] = dat-1000;
        emit datRegistersChanged();
        emit accRegistersChanged();
        emit isProgrammedChanged();
    }
}

void SerialCommunication::connectPort(QString port) {
    m_serial->close();
    m_serial->setPortName(port);
    m_serial->setBaudRate(19200);
    m_serial->open(QIODevice::ReadWrite);

    if (!m_serial->isOpen()) return;

    // check mcu connection
    writeSerialByte(0x31);
}

void SerialCommunication::startUpload(QStringList codeList) {
    for (int i=0; i < m_mcuConnections; i++) {
        if (!m_uploadDone.at(i)) {
            return;
        }
    }
    for (int i = 0; i < 3; i++) m_tries[i] = 0;
    m_codeList = codeList;
    upload();
}

void SerialCommunication::upload(int mcuNum) {
    m_errorMessage = "";
    m_isProgrammed = {false, false, false, false};
    emit isProgrammedChanged();
    QByteArray output;
    if (m_codeList.length() > mcuNum && m_codeList.at(mcuNum) != "") {
        QStringList lines = m_codeList.at(mcuNum).split('\n', QString::SkipEmptyParts);
        QStringList labels;


        for (int j = 0; j < lines.size(); j++) {
            QStringList parts = lines.at(j).split(QRegExp("\\s+"), QString::SkipEmptyParts);
            if (parts.size() == 0) continue;
            if (parts.at(0).endsWith(LABEL_MARKER) && !labels.contains(parts.at(0))) {
                labels.append(parts.at(0));
            }
        }

        for (int j = 0; j < lines.size(); j++) {
            QStringList parts = lines.at(j).split(QRegExp("\\s+"), QString::SkipEmptyParts);
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
                        if (k < parts.size()-1) {
                            part = parts.at(++k);
                        } else {
                            break;
                        }
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
    if (m_serial->isOpen()) {
        if (output.length() > MAX_PROGRAM_LENGTH) {
            m_errorMessage = QString("Program is too large!");
            emit errorMessageChanged();
            return;
        } else {
            // header
            m_uploadDone[mcuNum] = false;
            writeSerialByte(START_CHAR);
            writeSerialByte(0x31 + mcuNum);
            for (int x=0; x < output.length(); x++) {
                writeSerialByte(output.at(x));
            }
            writeSerialByte(checksum(output, output.length()));
            writeSerialByte(END_CHAR);
            QTimer::singleShot(SERIAL_DELAY * (MAX_PROGRAM_LENGTH + 10), this, &SerialCommunication::checkUpload);
            return;
        }
    } else {
        m_errorMessage = QString("No serial connection!");
        emit errorMessageChanged();
        return;
    }
}

void SerialCommunication::checkUpload() {
    for (int i=0; i < m_mcuConnections; i++) {
        if (!m_uploadDone.at(i)) {
            if (++m_tries[i] < UPLOAD_RETRIES) {
                upload(i);
            } else {
                m_uploadDone[i] = true;
            }
        }
    }
}

void SerialCommunication::writeSerialByte(char byte, bool debug) {
    QThread::msleep(SERIAL_DELAY);
    m_serial->write(&byte, 1);
    m_serial->waitForBytesWritten(100);

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
    for (int i=0; i < m_mcuConnections; i++) {
        if (!m_uploadDone.at(i)) {
            return;
        }
    }
    if (m_mcuConnections && m_serial->isOpen()) {
        for (int i=0; i < m_mcuConnections; i++) {
            writeSerialByte(0x31+i);
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
