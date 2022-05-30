#include "createusertable.h"
#include "ui_createusertable.h"

createUserTable::createUserTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::createUserTable)
{
    ui->setupUi(this);
}

createUserTable::~createUserTable()
{
    delete ui;
}

void createUserTable::on_pushButton_clicked()
{
    QString userName=ui->userName->text();
    QString pwd=ui->pwd->text();

    sendData(CreateUsers(userName,pwd));
}
