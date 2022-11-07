#ifndef SSLCLIENT_H
#define SSLCLIENT_H

#include "qpixmap.h"
#include <QObject>
#include <QSslSocket>
class sslclient : public QObject
{
    Q_OBJECT
    QSslSocket * socket = nullptr;
    QString serverIp = "127.0.0.1";
    quint16 serverPort = 45678;
    QString status;
    QPixmap avatar;
    QString connectedDate;

public:
    explicit sslclient(QString serverIpPort, QObject *parent = nullptr);

public slots:
    void onConnected();

};

#endif // SSLCLIENT_H
