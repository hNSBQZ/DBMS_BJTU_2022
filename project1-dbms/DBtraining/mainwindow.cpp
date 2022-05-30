#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"select.h"
#include"updatetable.h"
#include"deleteTable.h"

#include"iostream"
#include"qdebug.h"
#include"qdir.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    server=new QTcpServer;

    server->listen(QHostAddress::AnyIPv4,PORT);

    connect(server,&QTcpServer::newConnection,this,&MainWindow::newClientHandle);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newClientHandle()
{
    socket=server->nextPendingConnection();
    connect(socket,&QTcpSocket::readyRead,this,&MainWindow::clientInfoSlot);

}

void MainWindow::clientInfoSlot()
{
    QString Info=QString(socket->readAll());
    QStringList InfoList;
    if(Info[0]=="L")
    {
        InfoList=Info.split("%",QString::SkipEmptyParts);
        QFile userFile(rootAddress+"\\user.txt");
        if(!userFile.open(QIODevice::ReadOnly|QIODevice::Text))
            qDebug()<<"文件打开失败";
        qDebug(qPrintable(InfoList[1]));
        QString InfoCheck;
        QTextStream InfoIn(&userFile);
        bool checkInfo=false;
        while(!InfoIn.atEnd())
        {
            InfoIn>>InfoCheck;
            qDebug(qPrintable(InfoCheck));
            if(InfoCheck==InfoList[1])
            {
                checkInfo=true;
                break;
            }
        }
        QByteArray ba;
        if(checkInfo)
            {ba.append("correct");
            qDebug()<<"right";}
        else ba.append("wrong");
        socket->write(ba);
        userFile.close();
    }
    else
    {
        InfoList=Info.split("|",QString::SkipEmptyParts);
        qDebug()<<"wula";
        qDebug("%s",qPrintable(Info));
        QString secondInfo=InfoList[0];
        QStringList secondInfoList=secondInfo.split("%",QString::SkipEmptyParts);
        QString table=secondInfoList[1];
        QStringList tableList=table.split(",",QString::SkipEmptyParts);
        QFile rightfile(rootAddress+"//right.txt");
        if(!rightfile.open(QIODevice::ReadOnly|QIODevice::Text))
            qDebug()<<"open failed";
        QTextStream in(&rightfile);
        QSet<QString> rightSet;
        while(!in.atEnd())
        {
            QString s;
            in>>s;
            rightSet.insert(s);

        }
        bool HaveRight=true;
        foreach(QString rightTable,tableList)
        {
            QString right=secondInfoList[0]+"%"+rightTable+"%"+secondInfoList[2];
            if(!rightSet.contains(right))
            {
                HaveRight=false;
                break;
            }
        }
        if(!HaveRight)
        {
            QByteArray ba;
            socket->write(ba.append("NoRight"));
            return;
        }
        else
        {
            if(secondInfoList[2]=="insert")
            {
                qDebug()<<"insert";
                QByteArray ba;
                QStringList valueList=InfoList[1].split("%",QString::SkipEmptyParts);
                socket->write(ba.append(insert(valueList[0],valueList[1])));
            }
            else if(secondInfoList[2]=="update")
            {
                qDebug()<<"update";
                QByteArray ba;
                QStringList valueList=InfoList[1].split("%",QString::SkipEmptyParts);
                socket->write(ba.append(update_function(valueList[0],valueList[1],valueList[2])));
            }
            else if(secondInfoList[2]=="delete")
            {
                qDebug()<<"delete";
                QByteArray ba;
                QStringList valueList=InfoList[1].split("%",QString::SkipEmptyParts);
                socket->write(ba.append(delete_function(valueList[0],valueList[1])));
            }
            else if(secondInfoList[2]=="select")
            {
                qDebug()<<"select";
                QByteArray ba;
                QStringList valueList=InfoList[1].split("%");
                QString result=select(valueList[0],valueList[1],valueList[2],valueList[3]);
                QString returnResult="";
                QFile resultFile(result);
                qDebug("%s",qPrintable(result));
                if(!resultFile.open(QIODevice::ReadOnly|QIODevice::Text))
                    qDebug()<<"open Wrong";
                QTextStream resultIn(&resultFile);
                while(!resultIn.atEnd())
                {
                    QString tmpString;
                    resultIn>>tmpString;
                    if(tmpString=="")break;
                    returnResult+=tmpString+"#";
                }
                ba.append(returnResult);
                socket->write(ba);
                resultFile.close();
                resultFile.remove();
            }
            else
            {
                qDebug()<<"alter";
                QByteArray ba;
                QStringList valueList=InfoList[1].split("%",QString::SkipEmptyParts);
                socket->write(ba.append(AlterTable(valueList[0],valueList[1],valueList[2],valueList[3])));
            }
        }
    }

}

void MainWindow::reshow(){
    this->show();
}

void MainWindow::receiveData(QString data)
{
   qDebug("%s",qPrintable(data));

  if(!data.contains(".")){
      ui->textBrowser->setText(data);
      QFile resultFile("C:\\Users\\24625\\Desktop\\result.txt");
      if(!resultFile.open(QIODevice::ReadOnly|QIODevice::Text)){
              qDebug()<<"文件打开失败";
      }

      resultFile.remove();
  }
  else{
   ui->textBrowser->clear();

   QFile resultFile(data);
   if(!resultFile.open(QIODevice::ReadOnly|QIODevice::Text)){
           qDebug()<<"文件打开失败";
   }

   QTextStream out(&resultFile);
   QTextCursor tc = ui->textBrowser->textCursor();

   while(!out.atEnd()){
       out >> data;
       QStringList temp_column = data.split("%");
       foreach(QString s , temp_column){
           tc.movePosition(QTextCursor::End);
           tc.insertText(s + " ");
       }
       tc.insertText("\r\n");
   }

   resultFile.close();
   resultFile.remove();
  }
}

void MainWindow::on_select_pushButton_clicked()
{
//    this->hide();
    Select *selectWindow = new Select;
    selectWindow->show();
//    connect(selectWindow,SIGNAL(sendsignal()), this,SLOT(reshow()));
    connect(selectWindow, SIGNAL(sendData(QString)), this, SLOT(receiveData(QString)));
}

void MainWindow::on_update_pushButton_clicked()
{
//    this->hide();
    updateTable *updateWindow = new updateTable;
    updateWindow->show();
//    connect(updateWindow,SIGNAL(sendsignal()), this,SLOT(reshow()));
    connect(updateWindow, SIGNAL(sendData(QString)), this, SLOT(receiveData(QString)));
}

void MainWindow::on_delete_pushButton_clicked()
{
//    this->hide();
    deleteTable *deleteWindow = new deleteTable;
    deleteWindow->show();
//    connect(deleteWindow,SIGNAL(sendsignal()), this,SLOT(reshow()));
    connect(deleteWindow, SIGNAL(sendData(QString)), this, SLOT(receiveData(QString)));
}


void MainWindow::on_insert_pushButton_clicked()
{
    insertTable *i=new insertTable;
    i->show();
    connect(i, SIGNAL(sendData(QString)), this, SLOT(receiveData(QString)));
}

void MainWindow::on_alterTable_pushButton_clicked()
{
    alterTable *a=new alterTable;
    a->show();
    connect(a, SIGNAL(sendData(QString)), this, SLOT(receiveData(QString)));
}


void MainWindow::on_createUser_pushButton_clicked()
{
    createUserTable *c=new createUserTable;
    c->show();
    connect(c, SIGNAL(sendData(QString)), this, SLOT(receiveData(QString)));
}

void MainWindow::on_authorization_pushButton_clicked()
{
    OperateRightTable *o=new OperateRightTable;
    o->show();
    connect(o, SIGNAL(sendData(QString)), this, SLOT(receiveData(QString)));
}

void MainWindow::on_createTable_pushButton_clicked()
{
    CreateTableW *c=new CreateTableW;
    c->show();
    connect(c, SIGNAL(sendData(QString)), this, SLOT(receiveData(QString)));
}



void MainWindow::on_dropTable_pushButton_clicked()
{
    DropTable *d=new DropTable;
    d->show();
    connect(d, SIGNAL(sendData(QString)), this, SLOT(receiveData(QString)));
}
