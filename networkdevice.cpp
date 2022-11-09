#include "networkdevice.h"
#include <client.h>
#include <myserver.h>
NetworkDevice::NetworkDevice(QObject* parent) : QObject(parent){}
NetworkDevice::~NetworkDevice(){
    if (!m_devicePtr.isNull()){
        delete m_devicePtr;
    }
}
bool NetworkDevice::initialize(networkParams netPar){
    if (!m_devicePtr.isNull()){
        qDebug() << "Multiple initialization is not allowed\n";
        return false;
    }
    bool ok = false;
    m_netPar = netPar;
    switch (m_netPar.nType){
        case nodeType::ClientNode:
            qDebug() << "Initializing client\n";
            m_devicePtr = new Client;
            ok = reinterpret_cast<Client*>(m_devicePtr.data())->initialize(m_netPar);
            ok ? (qDebug() << "Client initialized") : (qDebug() << "Unable to initialize client");
        break;
    case nodeType::ServerNode:
        qDebug() << "Initializing server.";

                m_devicePtr = new MyServer();
                ok = static_cast<MyServer*>(m_devicePtr.data())->initializeServer(m_netPar);

                if (ok)
                    qDebug() << "Server object initialized.";
                else
                    qDebug() << "Couldn't initialize server.";
        break;
    default:
        qDebug() << "Invalid node type.";
                break;
            }

            if (ok) {
                qDebug() << "Connecting read signal.";
                ok = connect(m_devicePtr, SIGNAL(readFromSocket(QByteArray)), this, SIGNAL(readData(QByteArray)));
            }

            if (ok) {
                qDebug() << "Connecting write signal.";
                ok = connect(this, SIGNAL(writeData(QByteArray)), m_devicePtr, SLOT(writeToSocket(QByteArray)));
            }

            if (ok) {
                qDebug() << "Connecting closed signal.";
                ok = connect(m_devicePtr, SIGNAL(closedConnection()), this, SIGNAL(closedConnection()));
            }

            if (ok) {
                qDebug() << "Connecting opened signal.";
                ok = connect(m_devicePtr, SIGNAL(openedConnection()), this, SIGNAL(openedConnection()));
            }

            return ok;
        }
