#ifndef UPDATETABLE_H
#define UPDATETABLE_H

#include <QWidget>

namespace Ui {
class updateTable;
}

class updateTable : public QWidget
{
    Q_OBJECT

public:
    explicit updateTable(QWidget *parent = 0);
    ~updateTable();

private:
    Ui::updateTable *ui;
signals:
    void sendData(QString);
private slots:
    void on_pushButton_clicked();
};

#endif // UPDATETABLE_H
