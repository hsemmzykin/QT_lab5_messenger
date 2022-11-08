#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sslclient.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    client = new sslclient(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

