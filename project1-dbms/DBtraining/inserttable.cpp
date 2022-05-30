#include "inserttable.h"
#include "ui_inserttable.h"

insertTable::insertTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::insertTable)
{
    ui->setupUi(this);
}

insertTable::~insertTable()
{
    delete ui;
}

void insertTable::on_pushButton_clicked()
{
    QString tableName=ui->lineEdit->text();
    QString value=ui->lineEdit_2->text();
    qDebug(qPrintable(tableName+"|"+value+"|"));
    QString status=insert(tableName,value);
    qDebug()<<"hello";
    qDebug(qPrintable(status));
    sendData(status);
}

void insertTable::on_pushButton_2_clicked()
{
    return;
}


