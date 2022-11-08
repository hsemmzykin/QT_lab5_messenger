#include "sslclient.h"
#include <QRegExp>
#include "mainwindow.h"
#include <QMessageBox>
#include <random>
sslclient::sslclient( QObject *parent, QString serverIpPort)
    : QObject{parent},
      parent(parent)
{
    srand(time(nullptr));
    this->myName = "John_Doe" + QString::number(rand() % 1000);

    QRegExp regIpPort("((?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?))\\:(\\d{2,5})");
    regIpPort.indexIn(serverIpPort);
    QStringList list = regIpPort.capturedTexts();
    if (list.size() != 3){
        qDebug() << "Incorrect IPv4 and/or port";
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
    dynamic_cast<MainWindow*>(this->parent)->setWindowTitle(this->socket->peerAddress().toString() + ":" + QString::number(this->socket->peerPort()) + " " + this->status);

    this->socket->write(this->myName.toStdString().c_str());
    if (!this->socket->waitForBytesWritten(3000)){
        qDebug() << this->socket->errorString();
        QMessageBox msgBox;
        msgBox.setText("Unable to send username to the server, please check connection!");
        msgBox.exec();
    }
    else{
        qDebug() << this->myName;
    }

}
