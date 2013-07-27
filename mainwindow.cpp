#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkProxy>
#include <QDebug>

#include <iostream>

#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->goButton, SIGNAL(clicked()), this, SLOT(buttonClick()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::GetUrl(QString url){
    QNetworkProxy torProxy;
    torProxy.setType(QNetworkProxy::Socks5Proxy);
    torProxy.setHostName("127.0.0.1");
    torProxy.setPort(9050);
    QNetworkProxy::setApplicationProxy(torProxy);
    qDebug() << "Connecting...";
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    manager->get(QNetworkRequest(QUrl(url)));
}

void MainWindow::replyFinished(QNetworkReply *reply){
    qDebug() << "Reply recieved";
    if (reply->isOpen()){
        QByteArray replyData;
        replyData = reply->readAll();
        reply->close();
        ui->responseEdit->setPlainText(replyData.data());
    }
}

void MainWindow::buttonClick(){
    GetUrl(ui->urlEdit->text());
}
