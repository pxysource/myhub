#include "widget.h"

//这里用ttySAC3作为出库刷卡的串口

//计算总的停车费用
void Widget::parkConsume(QTime inTime)
{
    double cost;
    int seconds;

    QTime outTime(QTime::currentTime());

    seconds =  outTime.secsTo(inTime);
    stopTime = QTime(0, 0, seconds);

    parkCost = parkPrice * (seconds / 3600 + !(0 == (seconds % 3600)));
}

void Widget::parkInfo(uint32_t newid)
{
    QString msg = sqlGetParkInfo(newid);

    QTime inTime = QTime::fromString(msg);
    parkConsume(inTime);
}


