#include "client.h"

#include <QTcpSocket>
#include <QDebug>

Client::Client(QObject *parent) : QObject(parent) {}

Client::~Client() {
    if (socket != NULL) {
        socket->close();
        delete socket;
    }
}

// initialize with host and port
bool Client::initialize(networkParams networkParams) {
    socket = new QSslSocket();
    socket->connectToHost(networkParams.hostAddress, networkParams.port);

    if (!socket->waitForConnected()) {
        qDebug() << "Couldn't connect to host.";
        return false;
    }

    bool ok = connect(socket, SIGNAL(readyRead()), this, SLOT(readFromSocket()));
    Q_ASSERT(ok);

    ok = connect(socket, SIGNAL(disconnected()), this, SIGNAL(closedConnection()));
    Q_ASSERT(ok);

    emit openedConnection();
    qDebug() << "Connected.";

    return true;
}

bool Client::isConnected() {
    if (socket == NULL)
        return false;

    return socket->isOpen();
}

void Client::readFromSocket() {
    emit readFromSocket(socket->readAll());
}

void Client::writeToSocket(const QByteArray &bytes) {
    qint64 bytesWritten = socket->write(bytes);

    qDebug() << "Wrote" << bytesWritten << "to socket.";
}
