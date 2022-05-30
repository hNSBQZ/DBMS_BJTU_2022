#include "mainwindow.h"
#include "deleteTable.h"
#include "ui_deletetable.h"
#include"qdir.h"
#include"QtDebug"

#include"iostream"

using namespace std;

deleteTable::deleteTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::deleteTable)
{
    ui->setupUi(this);
    folderPath = QDir::cleanPath(QDir::currentPath() + QDir::separator());
}

deleteTable::~deleteTable()
{
    delete ui;
}

void deleteTable::on_back_pushButton_clicked()
{
    emit sendsignal();
    this->close();
}

void deleteTable::on_delete_pushButton_clicked()
{
   table_name = ui->delete_textEdit->toPlainText();
   where = ui->where_textEdit->toPlainText();

   filePath = folderPath + "/"  + table_name + ".txt";

   sendData(qPrintable(delete_function(table_name,where)));
}
