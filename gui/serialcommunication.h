#ifndef SERIALCOMMUNICATION_H
#define SERIALCOMMUNICATION_H

#include <QObject>

class SerialCommunication : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int mcuConnections READ mcuConnections NOTIFY mcuConnectionChanged)

public:
    explicit SerialCommunication(QObject *parent = nullptr);
    int mcuConnections() const;

public slots: // slots are public methods available in QML
    void connect();

signals:
    void mcuConnectionChanged();

private:
    int m_mcuConnections = 0;

};

#endif // SERIALCOMMUNICATION_H
