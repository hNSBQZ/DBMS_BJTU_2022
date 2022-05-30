#include "insettable.h"
#include "ui_insettable.h"

insetTable::insetTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::insetTable)
{
    ui->setupUi(this);
}

insetTable::~insetTable()
{
    delete ui;
}

void insetTable::on_pushButton_clicked()
{
    QString table=ui->lineEdit_table->text();
    QString value=ui->lineEdit_value->text();
    qDebug()<<"??";
    qDebug("%s",qPrintable(table));
    sendData(table+"%insert|"+table+"%"+value);
}


void insetTable::on_pushButton_2_clicked()
{
    return;
}
