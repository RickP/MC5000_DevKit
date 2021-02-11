#ifndef SERIALCOMMUNICATION_H
#define SERIALCOMMUNICATION_H
#include <QSerialPort>
#include <QObject>

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

public slots: // slots are public methods available in QML
    void connect(QString port);
    void updateRegisters();

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
};

#endif // SERIALCOMMUNICATION_H
