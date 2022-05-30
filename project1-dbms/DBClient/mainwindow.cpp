#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket=new QTcpSocket;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString IP=ui->IPLineEdit->text();
    QString port=ui->PortLineEdit->text();

    socket->connectToHost(QHostAddress(IP),port.toShort());

    connect(socket,&QTcpSocket::connected,[this]()
    {
        QByteArray ba;
        QMessageBox::information(this,"sign","connectOk");
        userName=ui->userNameLineEdit->text();
        pwd=ui->PwdLineEdit->text();
        ba.append("L%"+userName+"|"+pwd);
        socket->write(ba);
    });
    connect(socket,&QTcpSocket::readyRead,this,&MainWindow::HandleServerSlot);

    connect(socket,&QTcpSocket::disconnected,[this]()
    {
        QMessageBox::information(this,"sign","connectWrong");
    });

}

void MainWindow::HandleServerSlot()
{
    if(HaveLogin)return;
    QString result=QString(socket->readAll());
                            qDebug(qPrintable(result));
                            if((result.trimmed())=="correct")
                            {
                                FuctionCombining *f=new FuctionCombining(socket,userName);
                                f->show();
                                HaveLogin=true;
                                this->close();
                            }
                            else
                                QMessageBox::information(this,"sign","pwdOrUserNameWrong");
}
