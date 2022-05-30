#include "deleteTable.h"
#include "ui_delete.h"

delete::delete(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::delete)
{
    ui->setupUi(this);
}

delete::~delete()
{
delete ui;
}
