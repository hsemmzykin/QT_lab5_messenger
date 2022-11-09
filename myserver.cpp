#include "myserver.h"
#include <QStringList>
#include <QTcpSocket>

MyServer::MyServer() : QTcpServer() {}

MyServer::~MyServer() {}

bool MyServer::initializeServer(networkParams networkParams) {
    bool ok = false;

    qDebug() << "Making connections.";
    ok = connect(this, SIGNAL(newConnection()), SLOT(initializeConnection()));

    if (ok) {
        qDebug() << "Opening MyServer on" << networkParams.hostAddress << networkParams.port;
        ok = listen(networkParams.hostAddress, networkParams.port);
    }

    return ok;
}
bool MyServer::isConnected() {
    if (activeSocket != NULL)
        return activeSocket->isOpen();

    return false;
}

void MyServer::readFromClient() {
    emit readFromClient(activeSocket->readAll());
}

void MyServer::writeToClient(const QByteArray &bytes) {
    qint64 bytesWritten = activeSocket->write(bytes);

    qDebug() << "Wrote" << bytesWritten << "to socket.";
}

void MyServer::initializeConnection() {
    activeSocket = nextPendingConnection();

    // make connections
    bool ok = connect(activeSocket, SIGNAL(readyRead()), this, SLOT(readFromSocket()));
    Q_ASSERT(ok);

    ok = connect(activeSocket, SIGNAL(disconnected()), this, SIGNAL(closedConnection()));
    Q_ASSERT(ok);

    if (ok)
        emit openedConnection();

    close();
}
