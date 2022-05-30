#include "altertable.h"
#include "ui_altertable.h"

alterTable::alterTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::alterTable)
{
    ui->setupUi(this);
}

alterTable::~alterTable()
{
    delete ui;
}

void alterTable::on_pushButton_clicked()
{
    QString operate=ui->line_operate->text();
    QString table=ui->line_table->text();
    QString column=ui->line_column->text();
    QString dataType=ui->line_DataType->text();
    sendData(table+"%alter|"+operate+"%"+table+"%"+column+"%"+dataType);
}
