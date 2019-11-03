#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QMovie>
#include <QPalette>
//timer
#include <QTimer>
//sql
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QDebug>
#include <QString>
#include <QSqlError>
#include <QFile>
#include <QByteArray>
#include <QStringList>
#include <QList>
#include <QMessageBox>
//画图事件
#include <QPaintEvent>
#include <QPainter>
//鼠标事件
#include <QMouseEvent>
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

    int sqlUserRegister();
    void sqlShow();
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

signals:
    void onDetected();

private slots:
    void on_pushButtonRecognizeCardNo_clicked(bool checked);

    void detectCard();
    void getCardNumber();

    void on_pushButtonExit_clicked();

    void on_pushButtonRegister_clicked();

private:
    Ui::Widget *ui;

    //串口文件描述符
    int ttyFd;
    QTimer timer;

    //错误的做法:
    //数据库不能在这创建, 创建时必须添加一个数据库
    //QSqlDatabase db;
    //不能提前创建query, query只关联已经打开的数据库, 提前创建会报错
    //需要用query的时候再创建
    //默认操作打开了的数据库文件，无需特意关联
    //QSqlQuery query;
    //错误现象
    //QSqlQuery::exec: database not open
    //"Driver not loaded"

    //存放标准SQL语句
    QString SQL;
    // 定义一个数据模型对象，用来给试图提供数据来源
    QSqlQueryModel *model;

    void initTTY();
    void closeTTY();
    //这里获取车牌号采取随机生成, 实际情况改改就行
    QString getCarNumber();
};

#endif // WIDGET_H
