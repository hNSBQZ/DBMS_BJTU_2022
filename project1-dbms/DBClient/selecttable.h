#ifndef SELECTTABLE_H
#define SELECTTABLE_H

#include <QWidget>

namespace Ui {
class selectTable;
}

class selectTable : public QWidget
{
    Q_OBJECT

public:
    explicit selectTable(QWidget *parent = 0);
    ~selectTable();

private slots:
    void on_pushButton_clicked();

private:
    Ui::selectTable *ui;

signals:
    void sendData(QString);
};

#endif // SELECTTABLE_H
