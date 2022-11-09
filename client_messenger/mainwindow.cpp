#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sslclient.h"
#include <QDialog>
#include <QPalette>
#include <QLabel>
#include <QMessageBox>
#include <QCloseEvent>
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

// \brief: send message
void MainWindow::on_sendButton_clicked()
{
    ui->messageBox->setText(ui->sendMessage->text());

}


//void MainWindow::on_actionConnect_to_server_triggered()
//{

//}


void MainWindow::on_actionAbout_triggered()
{
    QDialog* infoAboutAuthor = new QDialog(this);
    infoAboutAuthor->setModal(true);
    infoAboutAuthor->setWindowTitle("About");
    QPalette palette = infoAboutAuthor->palette();
    palette.setColor(infoAboutAuthor->backgroundRole(), Qt::green);
    infoAboutAuthor->setPalette(palette);
    QGridLayout *gridLayout = new QGridLayout(infoAboutAuthor);
    QPixmap photo;
    photo.load("/home/sasijeppy/Downloads/icon.jpg");
    QLabel *photoLabel = new QLabel(infoAboutAuthor);
    photoLabel->setPixmap(photo.scaled(300, 300, Qt::KeepAspectRatio));
    gridLayout->addWidget(photoLabel, 0, 0, 16, 1, Qt::AlignBottom);
    QLabel *authorLabel = new QLabel("Maksim Kainitoff (фото 5-летней давности)", infoAboutAuthor);
    gridLayout->addWidget(authorLabel, 4, 1);
    QLabel *buildLabel = new QLabel(QString("Build QT version ") + QT_VERSION_STR, infoAboutAuthor);
    gridLayout->addWidget(buildLabel, 5, 1);
    QLabel *dateLabel = new QLabel(QString("DATE: ") + __DATE__ + ' ' + __TIME__, infoAboutAuthor);
    gridLayout->addWidget(dateLabel, 6, 1);
    QLabel *qtlabel = new QLabel(QString("Current Qt Version: ") + qVersion(), infoAboutAuthor);
    gridLayout->addWidget(qtlabel, 7, 1);

    QPushButton * close  = new QPushButton("Kill window", infoAboutAuthor);
    QObject::connect(close, &QPushButton::clicked, this, &MainWindow::onButtonCloseDialogClicked);

    //! Add button slot that closes dialog!

    infoAboutAuthor->setLayout(gridLayout);
    infoAboutAuthor->show();
}
void MainWindow::onButtonCloseDialogClicked(){

}


