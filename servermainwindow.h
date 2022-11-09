#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "client.h"
#include "networkparams.h"

#include <QMainWindow>
#include <QFile>
#include <QSslServer>
#include "sslserver.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Ui::MainWindow *ui;
    QFile logFile;
    networkParams m_networkParameters;
    QString ipName;
    quint16 portNumberName;
    quint16 clientAmountName = 0;
    SslServer* server;
    void enterPressed(QString heading = QString());
    QSslConfiguration SslConfig;
    std::unordered_map<QString, QSslSocket*> clientList;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setIpName(const QString&);
    void setPortNumberName(quint16);
    void setClientAmountName(quint16);
    void incomingConnection(int);
    void handleConnection();

public slots:
    void clearLog();
    void postText(QString s);
    void receiveNetworkData(QByteArray b);
    void readyRead();

};

#endif // MAINWINDOW_H
