#include "sslserver.h"
#include <QSslSocket>
SslServer::SslServer(QObject* parent) :  QSslServer(parent){}
void SslServer::incomingConnection(quintptr descriptorSocket){
    QSslSocket * clientSocket = reinterpret_cast<QSslSocket*>(descriptorSocket);
}
