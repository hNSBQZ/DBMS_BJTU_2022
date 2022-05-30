#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QTcpSocket>
#include<QHostAddress>
#include<QMessageBox>
#include"fuctioncombining.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void HandleServerSlot();

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QString userName;
    QString pwd;
    bool HaveLogin=false;
};

#endif // MAINWINDOW_H
