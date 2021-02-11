#ifndef SERIALCOMMUNICATION_H
#define SERIALCOMMUNICATION_H
#include <QSerialPort>
#include <QObject>
#include <QMap>

class SerialCommunication : public QObject
{
Q_OBJECT

Q_PROPERTY(int mcuConnections READ mcuConnections NOTIFY mcuConnectionChanged)
Q_PROPERTY(QStringList serialports READ serialports NOTIFY serialportsChanged)
Q_PROPERTY(QList<int> accRegisters READ accRegisters NOTIFY accRegistersChanged)
Q_PROPERTY(QList<int> datRegisters READ datRegisters NOTIFY datRegistersChanged)

public:
explicit SerialCommunication(QObject *parent = nullptr);
~SerialCommunication();
int mcuConnections() const;
QStringList serialports();
QList<int> datRegisters();
QList<int> accRegisters();

public slots:     // slots are public methods available in QML
void connect(QString port);
void updateRegisters();
void upload(QStringList);

signals:
void mcuConnectionChanged();
void serialportsChanged();
void accRegistersChanged();
void datRegistersChanged();

private:
QSerialPort m_serial;
int m_mcuConnections = 0;
QStringList m_serialports;
QList<int> m_datRegisters;
QList<int> m_accRegisters;

const QMap<QString, QPair<uint8_t, QStringList>> commands = {
    {
        "nop", {
            0x01 << 2, {}
        }
    },
    {
        "mov", {
            0x02 << 2, {"ri", "r"}
        }
    },
    {
        "jmp", {
            0x03 << 2, {"l"}
        },
    },
    {
        "slp", {
            0x04 << 2, {"ri"}
        },
    },
    {
        "slx", {
            0x05 << 2, {"r"}
        },
    },
    {
        "teq", {
            0x06 << 2, {"ri", "ri"}
        },
    },
    {
        "tgt", {
            0x07 << 2, {"ri", "ri"}
        },
    },
    {
        "tlt", {
            0x08 << 2, {"ri", "ri"}
        },
    },
    {
        "tcp", {
            0x09 << 2, {"ri", "ri"}
        },
    },
    {
        "add", {
            0x0A << 2, {"ri"}
        },
    },
    {
        "sub", {
            0x0B << 2, {"ri"}
        },
    },
    {
        "mul", {
            0x0C << 2, {"ri"}
        },
    },
    {
        "not", {
            0x0D << 2, {}
        }
    },
    {
        "dgt", {
            0x0E << 2, {"ri"}
        },
    },
    {
        "dst", {
            0x0F << 2, {"ri", "ri"}
        },
    }
};


};

#endif // SERIALCOMMUNICATION_H
