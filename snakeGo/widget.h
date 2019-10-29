#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPoint>
#include <QPainter>
#include <QPalette>
#include <QPen>
#include <QBrush>
#include <QPixmap>
#include <QMouseEvent>
#include <QPixmap>
#include <QColor>
#include <QTimer>
#include <QKeyEvent>
#include <QVector>
#include <QEvent>
#include <QLabel>
#include <QDebug>
#include <QString>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    //画图事件
    void paintEvent(QPaintEvent *event);
    //鼠标事件
    void mousePressEvent(QMouseEvent *event);
    //void mouseMoveEvent(QMouseEvent *event);
    //按键事件
    void keyPressEvent(QKeyEvent *event);
    //初始化snake, 让其有一定的长度
    void initSnake();
    //画蛇
    void moveSnake();
    //事件过滤器
    bool eventFilter(QObject *watched, QEvent *event);
    //游戏失败
    bool isGameOver();
    //吃东西
    bool eatFood();

private:
    Ui::Widget *ui;
    QPixmap *pix;
    QPixmap *food;
    QTimer timer;
    QTimer timer2;
    QPixmap *snake;
    QPoint newFoodPoint;
    QVector<QPoint> pointList;
    enum Direction{LEFT, RIGHT, TOP, BOTTOM};
    Direction flag;
    QLabel *label;
    bool haveFood;

private slots:
    //产生食物
    void generateFood();

    void snakeAutoMove();
};

#endif // WIDGET_H
