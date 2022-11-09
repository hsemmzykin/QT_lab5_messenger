#ifndef NETWORKPARAMS_H
#define NETWORKPARAMS_H
#include <QObject>
#include <QHostAddress>

enum class nodeType{
    Null = -1,
    ClientNode = 0,
    ServerNode = 1
};
struct networkParams{
    nodeType nType = nodeType::Null;
    QHostAddress hostAddress;
    qint16 port = {0};
};
enum class statusClient{
    NoDiturbance,
    Online,
    Mute
};

#endif // NETWORKPARAMS_H
