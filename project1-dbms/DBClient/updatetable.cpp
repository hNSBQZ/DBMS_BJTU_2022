#include "updatetable.h"
#include "ui_updatetable.h"

updateTable::updateTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::updateTable)
{
    ui->setupUi(this);
}

updateTable::~updateTable()
{
    delete ui;
}

void updateTable::on_pushButton_clicked()
{
    QString table=ui->line_tableName->text();
    QString set=ui->line_contain->text();
    QString where=ui->line_condition->text();
    sendData(table+"%update|"+table+"%"+set+"%"+where);
}
