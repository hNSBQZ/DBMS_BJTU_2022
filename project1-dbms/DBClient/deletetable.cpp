#include "deletetable.h"
#include "ui_deletetable.h"

deleteTable::deleteTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::deleteTable)
{
    ui->setupUi(this);
}

deleteTable::~deleteTable()
{
    delete ui;
}

void deleteTable::on_pushButton_clicked()
{
    QString table=ui->line_from->text();
    QString where=ui->line_where->text();
    sendData(table+"%delete|"+table+"%"+where);
}
