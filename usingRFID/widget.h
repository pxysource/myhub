#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTime>
#include <QTimer>
#include <QDebug>
#include <QMessageBox>
//sql
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
//tcp
#include <QTcpSocket>
#include <QHostAddress>
//file
#include <QFile>
//C
#include "iso14443a.h"
#include "crfid.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    //这样设置ip不好, 应写入文件
    void setHostIp(QString ipAddress){hostIp.clear(); hostIp.setAddress(ipAddress);}
    void setPort(QString ipPort){port = ipPort.toUShort();}
signals:
    //信号函数，只用写声明
    //在需要发射如下信号的时候, 用emit onDetect()
    void onDetected();
    //车辆出库时刷卡
    void onDetectedOut();

private slots:
    void on_pushButtonStartSlotCard_clicked(bool checked);
    void detectCard();
    void getCardNumber();
    void getStopInfo();

private:
    Ui::Widget *ui;
    QTimer timer;

    //串口文件描述符
    int ttyFd;
    //串口初始化和关闭
    void initTTY();
    void closeTTY();

    //tcp
    QTcpSocket *sock;
    QHostAddress hostIp;
    ushort port;

    void tcpWrite();
    void tcpRead();

    //sql
    //遍历数据库ParkSys.db的表stopinfo看新的卡号是否已经存在
    bool sqlIsCardNumberExist(uint32_t newid);
    //向数据库ParkSys.db的表stopinfo中插入数据
    bool sqlInsert(uint32_t newid);
    //遍历数据库ParkSys.db的表carinfo查找新的卡号对应的车牌
    QString sqlGetCarNo(uint32_t newid);
    //得到数据库ParkSys.db的表stopinfo中对应卡号的车辆的信息
    QString sqlGetParkInfo(uint32_t newid);

    //停车
    //停车的单价:每小时多少钱
    QString carNo;
    double parkPrice;
    double parkCost;
    QTime stopTime;
    //停车的相关信息
    void parkInfo(uint32_t newid);
    //计算总的停车费用
    void parkConsume(QTime inTime);
};

#endif // WIDGET_H
