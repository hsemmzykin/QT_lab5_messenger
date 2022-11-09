#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "sslclient.h"
#include <QMediaPlayer>
#include <QCloseEvent>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_sendButton_clicked();
//    void on_actionConnect_to_server_triggered();
    void killDialogByPush(QCloseEvent*);
    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;
    sslclient* client = nullptr;
    QMediaPlayer newMessage;
};
#endif // MAINWINDOW_H
