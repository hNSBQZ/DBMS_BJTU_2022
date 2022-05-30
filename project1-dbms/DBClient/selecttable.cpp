#include "selecttable.h"
#include "ui_selecttable.h"

selectTable::selectTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::selectTable)
{
    ui->setupUi(this);
}

selectTable::~selectTable()
{
    delete ui;
}

void selectTable::on_pushButton_clicked()
{
    QString attribute=ui->line_attribute->text();
    QString table=ui->line_from->text();
    QString where=ui->line_where->text();
    QString order=ui->line_order->text();
    sendData(table+"%select|"+attribute+"%"+table+"%"+where+"%"+order);
}
