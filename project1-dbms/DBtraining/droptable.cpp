#include "droptable.h"
#include "ui_droptable.h"

DropTable::DropTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DropTable)
{
    ui->setupUi(this);
}

DropTable::~DropTable()
{
    delete ui;
}

void DropTable::on_pushButton_clicked()
{
    QString table=ui->lineEdit->text();
    sendData(DropTables(table));
}
