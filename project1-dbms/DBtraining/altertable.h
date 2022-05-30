#ifndef ALTERTABLE_H
#define ALTERTABLE_H

#include <QWidget>
#include "function.h"

namespace Ui {
class alterTable;
}

class alterTable : public QWidget
{
    Q_OBJECT

public:
    explicit alterTable(QWidget *parent = 0);
    ~alterTable();

private slots:
    void on_pushButton_clicked();

private:
    Ui::alterTable *ui;

signals:
    void sendData(QString);
};

#endif // ALTERTABLE_H
