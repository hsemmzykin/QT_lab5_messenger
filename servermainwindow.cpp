#include "servermainwindow.h"
#include "client.h"
#include "ui_servermainwindow.h"
#include "networkdevice.h"
#include <QString>
#include <QDebug>
#include <QScrollBar>
#include <QSslCertificate>
#include <QSslKey>
#include <QSslSocket>
#include "openssl_generator.h"
#include <openssl/bio.h>
#include <QInputDialog>
#include <QRegExp>
#include <limits>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    bool ok;
    QString text = QInputDialog::getText(this, "Input IP:port",
                                     "IP:port", QLineEdit::Normal,
                                     "127.0.0.1:45678", &ok);
    if (ok && !text.isEmpty()){
        setWindowTitle(text + QString(" 0 users online"));
        QRegExp regIpPort("((?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?))\\:(\\d{2,5})");
        regIpPort.indexIn(text);
        QStringList list = regIpPort.capturedTexts();

        QStringList::iterator it = list.begin();
        ++it;
        ipName = *it;
        ++it;
        portNumberName = (*it).toUInt();
    }

    EVP_PKEY * privKey = generate_key();
    if (privKey == NULL){
       qDebug() << "NULL PRIVATE KEY";
       exit(-1);
    }

    X509 * certif = generate_x509(privKey);
    if (certif == NULL){
       qDebug() << "NULL CERT";
       exit(-1);
    }

    BIO* bio = BIO_new(BIO_s_mem());
    if (!PEM_write_bio_X509(bio, certif)){
       qDebug() << "NULL RETURNED BY PEM_write_bio_x509";
        exit(-1);
    }

    BUF_MEM* biostruct;
    BIO_get_mem_ptr(bio, &biostruct);
    std::unique_ptr<char[]> buf  = std::make_unique<char[]>(biostruct->length);
    if (static_cast<size_t>(BIO_read(bio, buf.get(), biostruct->length)) != biostruct->length){
       qDebug() << "BIO_read unable to get information from Buffer";
       exit(-1);
    }

    QSslCertificate cert(QByteArray(buf.get(), biostruct->length));
    QSslKey key(reinterpret_cast<Qt::HANDLE>(privKey));

    server = new SslServer(this);
//    connect(server, &QSslServer::startedEncryptionHandshake, this, SLOT(incommingConnection()));

// \brief: in Qt6.4 incommingConnection is a virtual method which is autom-ly called
    SslConfig = server->sslConfiguration();
    SslConfig.setLocalCertificate(cert);
    SslConfig.setPrivateKey(key);
    server->setSslConfiguration(SslConfig);

    do {
        server->listen(QHostAddress(ipName), portNumberName);
        if (!server->isListening()){
            postText("Server is not listening");
            if (portNumberName == std::numeric_limits<quint16>::max()){
                postText("No port is available");
                setWindowTitle("No port is available!");
                return;
            }
            portNumberName++;

        }
        else{
            postText("Server is listening on " + this->ipName + ":" + QString::number(this->portNumberName));
            break;
        }
    } while (true);

    setWindowTitle(ipName + ":" + QString::number(portNumberName) + " users online: " + QString::number(clientAmountName));
    connect(this->server, &SslServer::pendingConnectionAvailable, this, &MainWindow::handleConnection);
}
void MainWindow::handleConnection(){
    QSslSocket *  currentClientSocket =  dynamic_cast<QSslSocket*>(this->server->nextPendingConnection());
    if (currentClientSocket == nullptr){
        postText("Client socket is invalid");
        return;
    }
    connect(currentClientSocket, &QSslSocket::readyRead, this, &MainWindow::readyRead);
}

void MainWindow::readyRead(){
    QSslSocket *currentClientSocket = dynamic_cast<QSslSocket * >(sender());

    QString clientName;
    clientName += QString(currentClientSocket->readAll());

    if (clientName.isEmpty()){
        postText("Client hasn't presented themselves");
        currentClientSocket->disconnectFromHost();
        return;
    }

    auto result = clientList.emplace(clientName, currentClientSocket);
    if(!result.second){
        // \brief: result has 2 elements : iter and bool
        postText("This user is already connected to the server");
        currentClientSocket->disconnectFromHost();
        return;
    }

    this->clientAmountName++;
    this->setWindowTitle(ipName + ":" + QString::number(portNumberName) + " users online: " + QString::number(clientAmountName));
    this->postText("Client " + clientName + " with IP and PORT " + currentClientSocket->peerAddress().toString() + ":" + QString::number(currentClientSocket->peerPort()) + " is connected");

}

MainWindow::~MainWindow() {
    // close text file if opened
    if (logFile.isOpen()) {
        qDebug() << "Closing log file.";
        logFile.close();
    }

    delete ui;
}


void MainWindow::clearLog() {
    ui->textBrowser->clear();
}

void MainWindow::postText(QString s) {
    if (s.isEmpty()){
        return;
    }

    s += "\n";

    if (logFile.isOpen()) {
        // write s to text file
        // flush
        logFile.write(s.toLatin1());
        logFile.flush();
    }

    // viewability hack
    QScrollBar *sb = ui->textBrowser->verticalScrollBar();
    bool scrollBarAtBottom = sb->value() == sb->maximum();

    ui->textBrowser->insertPlainText(s);
    if (scrollBarAtBottom)
        sb->setValue(sb->maximum());
}
void MainWindow::setIpName(const QString& ip){
    ipName = ip;
}
void MainWindow::setPortNumberName(quint16 port){
    portNumberName = port;
}
void MainWindow::setClientAmountName(quint16 num = 0){
    clientAmountName = num;
}

void MainWindow::incomingConnection(int socketDescriptor)
{
    QSslSocket *serverSocket=new QSslSocket(this);
    this->clientAmountName++;
    setWindowTitle(ipName + ":" + QString::number(portNumberName) + " users online: " + QString::number(clientAmountName));
    if(serverSocket->setSocketDescriptor(socketDescriptor))
    {

        serverSocket->setSslConfiguration(SslConfig);
        QObject::connect(serverSocket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
        QObject::connect(serverSocket, SIGNAL(disconnected()), this, SLOT(slotClientDisconnected()));
        serverSocket->startServerEncryption();
    }
    else
    {
     delete serverSocket;
    }
}

void MainWindow::receiveNetworkData(QByteArray b) {
    postText(QString("%1: %2").arg("\nRemote", QString(b)));
}

