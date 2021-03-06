﻿#include "widget.h"
#include "ui_widget.h"

void Widget::on_pushButtonRecognizeCardNo_clicked(bool checked)
{
    //按下“识别卡号”按钮
    if(checked)
    {
        //qDebug() << __LINE__
        initTTY();

        //开关当前处于按下的状态
        ui->pushButtonRecognizeCardNo->setText("探测中...");

        //禁用所有的输入框
        ui->ComboBoxPort->setDisabled(true);
        ui->comboBoxDataSize->setDisabled(true);
        ui->ComboBoxBaudRate->setDisabled(true);

        //开启定时器，不断探测卡片是否在检测范围内
        timer.start(200);
    }
    //弹起“识别卡号”按钮
    else
    {
        //qDebug() << __LINE__;
        closeTTY();

        //开关处于弹起状态
        ui->pushButtonRecognizeCardNo->setText("识别卡号");

        //启用所有输入框
        ui->ComboBoxPort->setEnabled(true);
        ui->comboBoxDataSize->setEnabled(true);
        ui->ComboBoxBaudRate->setEnabled(true);

        //暂停定时器，不询问卡片状态
        timer.stop();
    }
}

void Widget::initTTY()
{
    //根据下拉框选项初始化串口
    const char *tty = (QString("/dev/%1").arg(ui->ComboBoxPort->currentText())).toStdString().c_str();
    ttyFd = open(tty, O_RDWR | O_NOCTTY);
    if(-1 == ttyFd)
    {
        QMessageBox::information(this, "错误", strerror(errno));
        exit(-1);
    }

    //声明设置串口的结构体
    struct termios config;
    bzero(&config, sizeof(config));

    // 设置无奇偶校验
    // 设置数据位为8位
    // 设置为非规范模式（对比与控制终端）
    config.c_iflag &= ~IXON;
    //
    // 2. 输出：禁用自定义输出模式
    //
    // 3. 控制：数据字符长度为8位
    //         禁用校验码
    config.c_cflag &= ~CSIZE;
    switch(ui->comboBoxDataSize->currentText().toInt())
    {
    case 5: config.c_cflag |= CS5; break;
    case 6: config.c_cflag |= CS6; break;
    case 7: config.c_cflag |= CS7; break;
    case 8: config.c_cflag |= CS8; break;
    }
    config.c_cflag &= ~PARENB;

    //设置波特率
    switch(ui->ComboBoxBaudRate->currentText().toInt())
    {
    case 9600  : cfsetspeed(&config, B9600); break;
    case 115200: cfsetspeed(&config, B115200); break;
    }

    // CLOCAL和CREAD分别用于本地连接和接受使能
    // 首先要通过位掩码的方式激活这两个选项。
    config.c_cflag |= CLOCAL | CREAD;

    // 一位停止位
    config.c_cflag &= ~CSTOPB;

    // 可设置接收字符和等待时间，无特殊要求可以将其设置为0
    config.c_cc[VTIME] = 0;
    config.c_cc[VMIN] = 1;

    // 用于清空输入/输出缓冲区
    tcflush (ttyFd, TCIFLUSH);
    tcflush (ttyFd, TCOFLUSH);

    //完成配置后，可以使用以下函数激活串口设置
    tcsetattr(ttyFd, TCSANOW, &config);
}

void Widget::closeTTY()
{
    //加上作用域符号才能调试close系统函数
    ::close(ttyFd);
}

void Widget::detectCard()
{
    init_REQUEST();
    char recvinfo[128];

    //向串口发送指令
    tcflush(ttyFd, TCIFLUSH);
    write(ttyFd, PiccRequest_IDLE, PiccRequest_IDLE[0]);

    usleep(10*1000);

    bzero(recvinfo, 128);
    //qDebug() << __LINE__;
    read(ttyFd, recvinfo, 128);
    //qDebug() << __LINE__;

    //应答帧状态部分为0 则请求成功
    if(recvinfo[2] == 0x00)
    {
        //qDebug() << "detected!";
        //这里发射信号,区别于普通的函数调用
        emit onDetected();
    }
    else
    {
        //qDebug() << "no card...";
    }
}

QString Widget::getCarNumber()
{
    QString carNo;
    carNo.clear();
    carNo.push_back("川");

    int count = 5;
    srand(time(NULL));
    while(count--)
    {
        unsigned char ch;
        unsigned int tmp = rand() % 36;

        if(tmp <= 9)
        {
            ch = tmp + 30;
        }
        else
        {
            ch = tmp + 65;
        }

        carNo.push_back(QChar(ch));
    }

    return carNo;
}


void Widget::getCardNumber()
{
    static uint32_t oldid, newid;

    // 获取附近卡片的卡号... ...
    // 无效卡号不计入
    oldid = newid==0xFFFFFFFF ? oldid : newid;
    newid = get_id(ttyFd);
    qDebug() << "fetch card Number:" << newid;
    if(newid == 0 || newid == 0xFFFFFFFF)
    {
        return;
    }

    // flag为真意味着：卡片刚放上去
    if(flagxx)
    {
        if(newid == oldid)
        {
            beep(5, 0.05);
        }
        else
        {
            QString carNo = getCarNumber();
            beep(1, 0.3);

            ui->labelDisplayCardNo->setText(QString("%1").arg(newid));
            ui->LineEditCardNumber->setText(QString("%1").arg(newid));
            ui->LineEditCarNumber->setText(carNo);

            //使能注册按钮
            ui->pushButtonRegister->setEnabled(true);

        }

        flagxx = false;
    }
    //qDebug() << "refres alarm";
    alarm(1);
}
