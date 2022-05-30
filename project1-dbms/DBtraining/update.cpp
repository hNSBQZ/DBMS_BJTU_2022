#include "mainwindow.h"
#include "updatetable.h"
#include "ui_update.h"

update::update(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::update)
{
    ui->setupUi(this);
}

update::~update()
{
    delete ui;
}

void update::on_update_pushButton_clicked()
{

}

void update::on_back_pushButton_clicked()
{
    this->close();
    MainWindow *mainWindow = new MainWindow;
    mainWindow->show();
}
