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

}

SerialCommunication::~SerialCommunication()
{
    m_serial.close();
}

QStringList SerialCommunication::serialports() {
    return m_serialports;
}

void SerialCommunication::connect(QString port) {
    m_serial.close();
    m_serial.setPortName(port);
    m_serial.setBaudRate(19200);
    m_serial.setFlowControl(QSerialPort::NoFlowControl);
    m_serial.open(QIODevice::ReadWrite);
    m_mcuConnections = 2;
    emit mcuConnectionChanged();
}

int SerialCommunication::mcuConnections() const {
 return m_mcuConnections;
}
