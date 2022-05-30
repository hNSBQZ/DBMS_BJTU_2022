#ifndef ALTERTABLE_H
#define ALTERTABLE_H

#include <QWidget>

namespace Ui {
class alterTable;
}

class alterTable : public QWidget
{
    Q_OBJECT

public:
    explicit alterTable(QWidget *parent = 0);
    ~alterTable();

private:
    Ui::alterTable *ui;
signals:
    void sendData(QString);
private slots:
    void on_pushButton_clicked();
};

#endif // ALTERTABLE_H
