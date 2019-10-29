#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QString>
#include <QStringList>
#include <QVector>
#include <QLabel>
#include <QLCDNumber>
#include <QPixmap>
#include <QPicture>
#include <QMovie>
#include <QSize>
#include <QDebug>
#include <QTimer>
#include <QTime>
#include <QDate>
//HTTP
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QUrlQuery>
#include <QByteArray>
//Json
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
//播放视频
//文件操作
#include <QFileDialog>
//鼠标事件
#include <QMouseEvent>
#include <QPoint>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void addPhoto();

    //鼠标事件需要复写的虚函数
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
private:
    Ui::Widget *ui;
    QTimer timer;
    QTimer photoTimer;
    //HTTP
    QNetworkReply *reply;
    QNetworkRequest request;
    QNetworkAccessManager *manager;

    //用于显示日期时间和天气的标签
    QLabel *labelDate;
    QLabel *labelTime;
    QLabel *labelWeather;

    //图片列表
    QStringList photoList;
    QStringList::iterator photoIt;

    //用于鼠标移动记录鼠标相对于widget左上角的位置
    QPoint begin;
private slots:
    void timeDisplay();
    void readyReadSlot();
    void displayPhoto();

};

#endif // WIDGET_H
