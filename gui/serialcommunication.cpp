#include "serialcommunication.h"
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>

SerialCommunication::SerialCommunication(QObject *parent) : QObject(parent)
{

}

void SerialCommunication::connect() {
    m_mcuConnections = 2;
    mcuConnectionChanged();
}

int SerialCommunication::mcuConnections() const {
 return m_mcuConnections;
}
