#ifndef DROPTABLE_H
#define DROPTABLE_H

#include <QWidget>
#include"function.h"

namespace Ui {
class DropTable;
}

class DropTable : public QWidget
{
    Q_OBJECT

public:
    explicit DropTable(QWidget *parent = 0);
    ~DropTable();

private slots:
    void on_pushButton_clicked();

private:
    Ui::DropTable *ui;
signals:
    void sendData(QString);
};

#endif // DROPTABLE_H
