#ifndef SSLSERVER_H
#define SSLSERVER_H

#include <QSslServer>
class SslServer : public QSslServer
{
public:
    SslServer(QObject* parent = nullptr);
protected:
    void incomingConnection(quintptr);

};

#endif // SSLSERVER_H
