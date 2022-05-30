#ifndef INSETTABLE_H
#define INSETTABLE_H

#include <QWidget>
#include<QString>
#include<qDebug>

namespace Ui {
class insetTable;
}

class insetTable : public QWidget
{
    Q_OBJECT

public:
    explicit insetTable(QWidget *parent = 0);
    ~insetTable();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

signals:
    void sendData(QString);
private:
    Ui::insetTable *ui;
};

#endif // INSETTABLE_H
