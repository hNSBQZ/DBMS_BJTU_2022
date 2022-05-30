#ifndef DELETETABLE_H
#define DELETETABLE_H

#include <QWidget>

namespace Ui {
class deleteTable;
}

class deleteTable : public QWidget
{
    Q_OBJECT

public:
    explicit deleteTable(QWidget *parent = 0);
    ~deleteTable();

private slots:
    void on_pushButton_clicked();

signals:
    void sendData(QString);

private:
    Ui::deleteTable *ui;
};

#endif // DELETETABLE_H
