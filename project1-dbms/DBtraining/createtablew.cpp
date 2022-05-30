#include "createtablew.h"
#include "ui_createtablew.h"

CreateTableW::CreateTableW(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CreateTableW)
{
    ui->setupUi(this);
}

CreateTableW::~CreateTableW()
{
    delete ui;
}

void CreateTableW::on_pushButton_clicked()
{
    QString m = ui->lineEdit->text(); //获取行编辑器内容
    QString n = ui->textEdit->toPlainText();//获取文本编辑器内容
    sendData(CreateTables(m,n));
}
