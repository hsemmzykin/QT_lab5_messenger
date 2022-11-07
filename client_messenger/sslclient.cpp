#include "sslclient.h"
#include <QRegExp>
sslclient::sslclient(QString serverIpPort, QObject *parent)
    : QObject{parent}
{
    QRegExp regIpPort("^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
    regIpPort.indexIn(serverIpPort);
    QStringList list = regIpPort.capturedTexts();
    if (list.size() != 3){
        qDebug() << "Incorrect ip and/or port";
    }
    else{
        QStringList::iterator it = list.begin();
        ++it;
        serverIp = *it;
        ++it;
        serverPort = (*it).toUInt();
    }

    this->socket = new QSslSocket(this);
    this->socket->setPeerVerifyMode(QSslSocket::VerifyNone);
    connect(this->socket, &QSslSocket::encrypted, this, &sslclient::onConnected);

    this->socket->connectToHostEncrypted(serverIp, serverPort);
}
void sslclient::onConnected(){
    qDebug() << this->socket->sessionProtocol();
}
