#ifndef NETWORKDEVICE_H
#define NETWORKDEVICE_H

#include <QObject>
#include "networkparams.h"
#include <QPointer>
#include <QFile>

class NetworkDevice : public QObject
{
    Q_OBJECT
    QPointer<QObject> m_devicePtr;
    networkParams m_netPar;

public:
    explicit NetworkDevice(QObject* parent = nullptr);
    ~NetworkDevice();
    bool initialize(networkParams netPar);
signals:
    void readData(const QByteArray&);
    void writeData(const QByteArray&);
    void openedConnection();
    void closedConnection();
};

#endif // NETWORKDEVICE_H
