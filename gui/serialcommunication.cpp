#include "serialcommunication.h"
#include <QDebug>
#include <QSerialPortInfo>

SerialCommunication::SerialCommunication(QObject *parent) : QObject(parent)
{
    loadPorts();
}

void SerialCommunication::loadPorts() {
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
    for (int i=1;; i++) {
        m_serial.write(QString::number(i).toLocal8Bit(), 1);
        if (m_serial.waitForBytesWritten(100)) {
            if (m_serial.waitForReadyRead(100)) {
                m_serial.readAll();
                m_mcuConnections++;
            } else {
                break;
            }
        }
    }
    emit mcuConnectionChanged();
}

void SerialCommunication::upload(QStringList codeList) {
    m_errorMessage = "";
    for (int i = 0; i < codeList.size(); i++) {
        if (codeList.at(i) == "") continue;
        QStringList lines = codeList.at(i).split('\n', Qt::SkipEmptyParts);
        QStringList labels;
        QByteArray output;

        for (int j = 0; j < lines.size(); j++) {
            QStringList parts = lines.at(j).split(QRegExp("\\s+"), Qt::SkipEmptyParts);
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
                    output.append(LABEL_HEXCODE);
                    output.append(labels.lastIndexOf(part));
                    break;
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
                        m_errorMessage = QString("Command %1 needs %2 parameters (Line %3)").arg(part).arg(parameters.length()).arg(k+1);
                        emit errorMessageChanged();
                        return;
                    }

                    output.append(command | addToCommand);

                    for (int l=0; l < parameters.length(); l++) {
                        if (parameters.at(l) == "ri") {
                            char* value16 = encode16BitVal(parts.at(k+l+1));
                            if (value16[0] == 0x7F) {
                                m_errorMessage = QString("Unknown register %1 (Line %2)").arg(parts.at(k+l+1)).arg(k+1);
                                emit errorMessageChanged();
                                return;
                            }
                            output.append(value16, 2);
                        }
                        else if (parameters.at(l) == "r") {
                            char value8 = encode8BitVal(parts.at(k+l+1));
                            if (value8 == 0x7F) {
                                m_errorMessage = QString("Unknown register %1 (Line %2)").arg(parts.at(k+l+1)).arg(k+1);
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
                    m_errorMessage = QString("Unknown command %1 (Line %2)").arg(part).arg(k+1);
                    emit errorMessageChanged();
                    return;
                }
            }
        }
        qDebug() << output.toHex();
    }
}

char SerialCommunication::encode8BitVal(QString parameter) {
    if (registerEncodings.contains(parameter)) {
        return registerEncodings.value(parameter);
    }
    return SIGNAL_BYTE; // acc is default register
}

char* SerialCommunication::encode16BitVal(QString parameter) {
    static char ret[2];
    ret[0] = encode8BitVal(parameter);
    if (ret[0] !=SIGNAL_BYTE) {
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
        m_accRegisters.clear();
        m_datRegisters.clear();
        for (int i=1; i <= m_mcuConnections+1; i++) {
            m_serial.readAll();
            m_serial.write(QString::number(i).toLocal8Bit(), 1);
            if (m_serial.waitForBytesWritten(100)) {
                if (m_serial.waitForReadyRead(100)) {
                    char data[4];
                    if (m_serial.read(data, 4) == 4) {
                        int acc = data[0] << 7;
                        acc |= data[1];
                        m_accRegisters.append(acc-1000);
                        int dat = data[2] << 7;
                        dat |= data[3];
                        m_datRegisters.append(dat-1000);
                    }
                } else {
                    break;
                }
            }
        }
        emit datRegistersChanged();
        emit accRegistersChanged();
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
