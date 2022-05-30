#include "fuctioncombining.h"
#include "ui_fuctioncombining.h"

FuctionCombining::FuctionCombining(QTcpSocket *s,QString UN,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FuctionCombining)
{
    ui->setupUi(this);
    socket=s;
    userName.append(UN);
    connect(socket,&QTcpSocket::readyRead,this,&FuctionCombining::HandleServerSlot);
}

FuctionCombining::~FuctionCombining()
{
    delete ui;
}

void FuctionCombining::on_pushButton_clicked()
{
    //select
    selectTable *s=new selectTable;
    s->show();
    connect(s, SIGNAL(sendData(QString)), this, SLOT(receiveData(QString)));
    return;
}

void FuctionCombining::on_pushButton_2_clicked()
{
    //update
    updateTable *u=new updateTable;
    u->show();
    connect(u, SIGNAL(sendData(QString)), this, SLOT(receiveData(QString)));
}


void FuctionCombining::on_pushButton_3_clicked()
{
    //delete
    deleteTable *d=new deleteTable;
    d->show();
    connect(d, SIGNAL(sendData(QString)), this, SLOT(receiveData(QString)));
}

void FuctionCombining::on_pushButton_4_clicked()
{
    //insert
    insetTable *i = new insetTable;
    i->show();
//    connect(updateWindow,SIGNAL(sendsignal()), this,SLOT(reshow()));
    connect(i, SIGNAL(sendData(QString)), this, SLOT(receiveData(QString)));
}

void FuctionCombining::HandleServerSlot()
{
    QString result=QString(socket->readAll());
    qDebug()<<"the message from server";
    qDebug("%s",qPrintable(result));
    if(!result.contains("%"))
        ui->textBrowser->setText(result);
    else
        {
            ui->textBrowser->clear();
            QTextCursor tc=ui->textBrowser->textCursor();
            QStringList resultList=result.split("#",QString::SkipEmptyParts);
            foreach(QString eachLine,resultList)
            {
                QStringList lineAttribute=eachLine.split("%",QString::SkipEmptyParts);
                foreach(QString singleAttribute,lineAttribute)
                {
                    tc.movePosition(QTextCursor::End);
                    tc.insertText(singleAttribute+" ");
                }
                tc.insertText("\r\n");
            }
        }
}

void FuctionCombining::receiveData(QString data)
{
    QByteArray ba;
    qDebug()<<"shit";
    qDebug(qPrintable(userName));
    qDebug()<<"shit";
    qDebug("%s",qPrintable(data));
    //ui->textBrowser->setText(userName);
    ba.append(userName+"%"+data);
    socket->write(ba);
}

void FuctionCombining::on_pushButton_5_clicked()
{
    //alter
    alterTable *a=new alterTable;
    a->show();
    connect(a, SIGNAL(sendData(QString)), this, SLOT(receiveData(QString)));
}
