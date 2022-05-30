#include "mainwindow.h"
#include "select.h"
#include "ui_select.h"
#include"qdir.h"
#include"QtDebug"
#include"vector"
#include"algorithm"
#include"function.h"

#include<iostream>

using namespace std;

Select::Select(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Select)
{
    ui->setupUi(this);
}

Select::~Select()
{
    delete ui;
}

//发送信息
void Select::on_back_pushButton_clicked()
{
    emit sendsignal();
    this->close();
}

//确认按钮
void Select::on_confirm_pushButton_clicked()
{
    colunm_name = ui->attribute_textEdit->toPlainText();
    table_name = ui->tablename_textEdit->toPlainText();
    where = ui->condition_textEdit->toPlainText();
    order = ui->order__textEdit->toPlainText();

    sendData(select(colunm_name,table_name,where,order));
}
