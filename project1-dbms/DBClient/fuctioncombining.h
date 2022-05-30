#ifndef FUCTIONCOMBINING_H
#define FUCTIONCOMBINING_H

#include <QWidget>
#include<QTcpSocket>
#include"insettable.h"
#include"updatetable.h"
#include"deletetable.h"
#include"altertable.h"
#include"selecttable.h"

namespace Ui {
class FuctionCombining;
}

class FuctionCombining : public QWidget
{
    Q_OBJECT

public:
    explicit FuctionCombining(QTcpSocket *s,QString UN,QWidget *parent = 0);
    ~FuctionCombining();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void HandleServerSlot();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::FuctionCombining *ui;
    QTcpSocket *socket;
    QString userName="";

public slots:
    void receiveData(QString data);

};

#endif // FUCTIONCOMBINING_H
