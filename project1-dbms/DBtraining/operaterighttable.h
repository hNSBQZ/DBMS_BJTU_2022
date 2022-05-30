#ifndef OPERATERIGHTTABLE_H
#define OPERATERIGHTTABLE_H

#include <QWidget>
#include"function.h"

namespace Ui {
class OperateRightTable;
}

class OperateRightTable : public QWidget
{
    Q_OBJECT

public:
    explicit OperateRightTable(QWidget *parent = 0);
    ~OperateRightTable();

private:
    Ui::OperateRightTable *ui;
signals:
    void sendData(QString);
private slots:
    void on_pushButton_clicked();
};

#endif // OPERATERIGHTTABLE_H
