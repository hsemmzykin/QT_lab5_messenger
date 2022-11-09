#ifndef MYSERVER_H
#define MYSERVER_H

#include <QTcpServer>
#include <networkparams.h>
#include <QSslSocket>
class MyServer : public QTcpServer
{
    Q_OBJECT
    QTcpSocket* activeSocket;
public:
    explicit MyServer();
    ~MyServer();
    bool initializeServer(networkParams netwPar);
    bool isConnected();
signals:
    void readFromClient(const QByteArray&);
    void openedConnection();
    void closedConnection();
public slots:
    void readFromClient();
    void writeToClient(const QByteArray&);
private slots:
    void initializeConnection();
};

#endif // MYSERVER_H
