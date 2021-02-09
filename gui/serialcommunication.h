#ifndef SERIALCOMMUNICATION_H
#define SERIALCOMMUNICATION_H
#include <QSerialPort>
#include <QObject>

class SerialCommunication : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int mcuConnections READ mcuConnections NOTIFY mcuConnectionChanged)
    Q_PROPERTY(QStringList serialports READ serialports NOTIFY serialportsChanged)

public:
    explicit SerialCommunication(QObject *parent = nullptr);
    ~SerialCommunication();
    int mcuConnections() const;
    QStringList serialports();

public slots: // slots are public methods available in QML
    void connect(QString port);

signals:
    void mcuConnectionChanged();
    void serialportsChanged();

private:
    int m_mcuConnections = 0;
    QStringList m_serialports;
    QSerialPort m_serial;

};

#endif // SERIALCOMMUNICATION_H
