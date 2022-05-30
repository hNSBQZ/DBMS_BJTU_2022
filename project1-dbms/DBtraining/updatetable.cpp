#include"mainwindow.h"
#include "updatetable.h"
#include "ui_updatetable.h"
#include"qdir.h"
#include"QtDebug"

#include"iostream"

using namespace std;

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

void updateTable::on_back_pushButton_clicked()
{
    emit sendsignal();
    this->close();
}

void updateTable::on_update_pushButton_clicked()
{
    table_name = ui->update_textEdit->toPlainText();
    set = ui->set_textEdit->toPlainText();
    where = ui->where_textEdit->toPlainText();

    sendData(qPrintable(update_function(table_name,set,where)));
}
