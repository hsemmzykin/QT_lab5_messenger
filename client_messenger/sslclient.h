#ifndef SSLCLIENT_H
#define SSLCLIENT_H

#include "qpixmap.h"
#include <QObject>
#include <QSslSocket>
class sslclient : public QObject
{
    Q_OBJECT
    QSslSocket * socket = nullptr;
    QString serverIp;
    quint16 serverPort;
    QString status;
    QPixmap avatar;
    QString myName;
    QString connectedDate;
    QObject * parent = nullptr;
public:
    explicit sslclient(QObject *parent = nullptr, QString serverIpPort = "127.0.0.1:45678");

public slots:
    void onConnected();

};

#endif // SSLCLIENT_H
