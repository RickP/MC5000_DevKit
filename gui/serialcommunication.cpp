#include "serialcommunication.h"
#include <QDebug>
#include <QSerialPortInfo>

SerialCommunication::SerialCommunication(QObject *parent) : QObject(parent)
{
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

    // qDebug("0X%02X", commands.value("nop").first);

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

void SerialCommunication::upload(QStringList) {

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
