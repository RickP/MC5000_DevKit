#ifndef SERIALCOMMUNICATION_H
#define SERIALCOMMUNICATION_H
#include <QSerialPort>
#include <QObject>
#include <QMap>

class SerialCommunication : public QObject {
Q_OBJECT

Q_PROPERTY(int mcuConnections READ mcuConnections NOTIFY mcuConnectionChanged)
Q_PROPERTY(QStringList serialports READ serialports NOTIFY serialportsChanged)
Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged)
Q_PROPERTY(QList<int> accRegisters READ accRegisters NOTIFY accRegistersChanged)
Q_PROPERTY(QList<int> datRegisters READ datRegisters NOTIFY datRegistersChanged)
Q_PROPERTY(QList<bool> isProgrammed READ isProgrammed NOTIFY isProgrammedChanged)

const QString LABEL_MARKER = ":";
const QString COMMENT_MARKER = "#";
const char LABEL_HEXCODE = 0x10 << 2;
const char START_CHAR = 0x7F;
const char END_CHAR = 0x7E;
const int SERIAL_DELAY = 20;

public:
explicit SerialCommunication(QObject *parent = nullptr);
~SerialCommunication();
int mcuConnections() const;
QStringList serialports();
QString errorMessage();
QList<int> datRegisters();
QList<int> accRegisters();
QList<bool> isProgrammed();

public slots:         // slots are public methods available in QML
void connect(QString port);
void updateRegisters();
void upload(QStringList);
void loadPorts();

signals:
void mcuConnectionChanged();
void errorMessageChanged();
void serialportsChanged();
void accRegistersChanged();
void datRegistersChanged();
void isProgrammedChanged();

private:
QSerialPort m_serial;
int m_mcuConnections = 0;
QString m_errorMessage = "";
QStringList m_serialports = {};
QList<int> m_datRegisters = {0,0,0,0,0};
QList<int> m_accRegisters = {0,0,0,0,0};
QList<bool> m_isProgrammed = {false, false, false, false};
char encode8BitVal(QString);
char* encode16BitVal(QString);
void writeSerialByte(char, bool debug=true);

const QMap<QString, char> registerEncodings = {
    {"x0", 0x40},
    {"x1", 0x48},
    {"p0", 0x50},
    {"p1", 0x58},
    {"dat", 0x60},
    {"acc", 0x70}
};

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
