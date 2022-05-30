#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"select.h"
#include"updatetable.h"
#include"deleteTable.h"
#include"inserttable.h"
#include"function.h"
#include"altertable.h"
#include"createusertable.h"
#include"operaterighttable.h"
#include"createtablew.h"
#include"droptable.h"
#include<QTcpServer>
#include<QTcpSocket>
#define PORT 8000

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    QTcpServer *server;
    QString rootAddress="C:\\Users\\24625\\Desktop";
    QTcpSocket *socket;

    void sendStringData();

private slots:
    void on_select_pushButton_clicked();

    void on_update_pushButton_clicked();

    void on_delete_pushButton_clicked();

    void newClientHandle();

    void clientInfoSlot();

    void on_insert_pushButton_clicked();

    void on_alterTable_pushButton_clicked();

    void on_createUser_pushButton_clicked();

    void on_authorization_pushButton_clicked();

    void on_createTable_pushButton_clicked();

    void on_dropTable_pushButton_clicked();

public slots:
    void reshow();

    void receiveData(QString data);   //接收传递过来的数据的槽

//    void readFile(QString filePath);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
