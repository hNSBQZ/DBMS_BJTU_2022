#ifndef SELECT_H
#define SELECT_H

#include <QWidget>
#include"vector"
#include"function.h"

namespace Ui {
class Select;
}

class Select : public QWidget
{
    Q_OBJECT

public:
    explicit Select(QWidget *parent = nullptr);
    ~Select();

//路径
    QString folderPath;
    QString filePath;
//表明
    QString table_name;
//列明
    QString colunm_name;
//选择语句
    QString where;
//排序
    QString order;


private slots:
    void on_back_pushButton_clicked();

    void on_confirm_pushButton_clicked();

private:
    Ui::Select *ui;

signals:
    void sendsignal();//这个函数用户向主界面通知关闭的消息

    void sendData(QString);   //用来传递数据的信号
};

#endif // SELECT_H
