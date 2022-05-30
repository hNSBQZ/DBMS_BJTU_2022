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
    explicit deleteTable(QWidget *parent = nullptr);
    ~deleteTable();

    QString folderPath;
    QString filePath;
    QString table_name;
    QString where;

private slots:
    void on_delete_pushButton_clicked();

    void on_back_pushButton_clicked();

private:
    Ui::deleteTable *ui;

signals:
    void sendsignal();
           //这个函数用户向主界面通知关闭的消息
    void sendData(QString);   //用来传递数据的信号

};

#endif // DELETETABLE_H
