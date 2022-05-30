#include "operaterighttable.h"
#include "ui_operaterighttable.h"

OperateRightTable::OperateRightTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OperateRightTable)
{
    ui->setupUi(this);
}

OperateRightTable::~OperateRightTable()
{
    delete ui;
}

void OperateRightTable::on_pushButton_clicked()
{
    QString operate=ui->operate->text();
    QString UserName=ui->userName->text();
    QString table=ui->table->text();
    QString right=ui->right->text();

    sendData(OperateRights(operate,UserName,table,right));
}
