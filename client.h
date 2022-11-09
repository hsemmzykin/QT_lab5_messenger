#ifndef CLIENT_H
#define CLIENT_H

#include "networkparams.h"
#include <QSslSocket>
#include <QHostAddress>

class QTcpSocket;
class Client : public QObject
{
    Q_OBJECT
public:
    QString ip;
    int port;
    QString username;
    int status;
public:
    QSslSocket *socket;

public:
    explicit Client(QObject *parent = nullptr);
    ~Client();
    Client(const Client&) = default;
    bool initialize(networkParams netPar);
    bool isConnected();

signals:
    void readFromSocket(QByteArray);
    void openedConnection(Client);
    void closedConnection(Client);

public slots:
    void readFromSocket();
    void writeToSocket(const QByteArray &bytes);
};

#endif // CLIENT_H
