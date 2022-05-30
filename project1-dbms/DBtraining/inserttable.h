#ifndef INSERTTABLE_H
#define INSERTTABLE_H

#include <QWidget>
#include<function.h>
#include<QDebug>

namespace Ui {
class insertTable;
}

class insertTable : public QWidget
{
    Q_OBJECT

public:
    explicit insertTable(QWidget *parent = 0);
    ~insertTable();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();


private:
    Ui::insertTable *ui;

signals:
    void sendData(QString);   //用来传递数据的信号

};

#endif // INSERTTABLE_H
