#include "widget.h"

void Widget::tcpWrite()
{
    QString writeStr;
    writeStr.clear();

    if(!stopTime.secsTo(QTime(0, 0, 0)))
    {
        writeStr = QString("欢迎%1入场").arg(carNo);
    }
    else
    {
        //停车时长两小时，收费二十元
        writeStr = QString("停车时长%1小时%2分钟, 收费%3元")\
                .arg(stopTime.hour())\
                .arg(stopTime.minute())\
                .arg(parkCost);
    }

    sock->write(writeStr.toStdString().c_str());
}

void Widget::tcpRead()
{
    //将收到的wav数据写入文件/pxy/Qtproj/park/speeh.wav
    QFile file("/pxy/Qtproj/park/speeh.wav");
    if(!file.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        qDebug() << "open speeh.wav failed";
        return;
    }

    //读取收到的wav信息, 并播放
    file.write(sock->readAll());
    file.close();

    QString cmd = QString("aplay %1").arg("/pxy/Qtproj/park/speeh.wav");
    system(cmd.toStdString().c_str());
}
