#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->resize(800, 480);
    //设置widget无边框
    this->setWindowFlags(Qt::FramelessWindowHint);
    //设置widget颜色
    QColor color(98, 65, 30);
    this->setPalette(QPalette(color));

    //设置鼠标能触发移动事件
    this->setMouseTracking(true);

    pix = new QPixmap(this->size().width() - 40, this->size().height() - 40);

    food = new QPixmap(20, 20);
    snake = new QPixmap(20, 20);

    initSnake();

    connect(&timer, SIGNAL(timeout()), this, SLOT(generateFood()));
    connect(&timer2, SIGNAL(timeout()), this, SLOT(snakeAutoMove()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);

    painter.drawPixmap(20, 20, *pix);
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    //右键则关闭widget
    if(Qt::RightButton == event->button())
    {
        close();
    }
    else if(Qt::LeftButton == event->button())
    {
        //begin = event->pos();
    }
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    QPoint tmpPoint;
    tmpPoint = *(pointList.begin());
    switch(event->key())
    {
    case Qt::Key_W:
        if(Direction::BOTTOM == flag)
        {
            return;
        }
        tmpPoint.setY(tmpPoint.y() - 20);
        flag = Direction::TOP;
        break;
    case Qt::Key_A:
        if(Direction::RIGHT == flag)
        {
            return;
        }
        tmpPoint.setX(tmpPoint.x() - 20);
        flag = Direction::LEFT;
        break;
    case Qt::Key_S:
        if(Direction::TOP == flag)
        {
            return;
        }
        tmpPoint.setY(tmpPoint.y() + 20);
        flag = Direction::BOTTOM;
        break;
    case Qt::Key_D:
        if(Direction::LEFT == flag)
        {
            return;
        }
        tmpPoint.setX(tmpPoint.x() + 20);
        flag = Direction::RIGHT;
        break;
    case Qt::Key_1:
        label->close();
        delete label;
        initSnake();
        return;
    }

    pointList.push_front(tmpPoint);
    moveSnake();
}

void Widget::initSnake()
{
    //去除事件过滤器, 让事件能正常响应
    this->removeEventFilter(this);

    pix->fill(Qt::darkBlue);
    food->fill(Qt::red);
    haveFood = false;
    snake->fill(Qt::white);
    pointList.clear();
    flag = Direction::LEFT;

    generateFood();

    QPainter painter;

    painter.begin(pix);
    QPoint point(pix->size().width() / 2, pix->size().height()/2);
    pointList.push_back(point);
    point.setX(point.x() + 20);
    pointList.push_back(point);
    point.setX(point.x() + 20);
    pointList.push_back(point);

    painter.drawPixmap(*pointList.begin(), *snake);
    painter.drawPixmap(*((pointList.begin() + 1)), *snake);
    painter.drawPixmap(*((pointList.begin() + 2)), *snake);
    painter.end();

    update();

    //定时器
    timer.start(50);
    timer2.start(500);
}

void Widget::generateFood()
{
    if(haveFood)
    {
        return;
    }
    QPainter painter;
    painter.begin(pix);
    painter.setRenderHint(QPainter::Antialiasing);
    newFoodPoint.setX(rand() % 740 / 20 * 20);
    newFoodPoint.setY(rand() % 440 / 20 * 20);
    painter.drawPixmap(newFoodPoint, *food);
    painter.end();

    update();
    haveFood = true;
}

void Widget::snakeAutoMove()
{
    QPoint tmpPoint;
    tmpPoint = *(pointList.begin());

    switch(flag)
    {
    case Direction::LEFT:
        tmpPoint.setX(tmpPoint.x() - 20);
        break;
    case Direction::RIGHT:
        tmpPoint.setX(tmpPoint.x() + 20);
        break;
    case Direction::TOP:
        tmpPoint.setY(tmpPoint.y() - 20);
        break;
    case Direction::BOTTOM:
        tmpPoint.setY(tmpPoint.y() + 20);
        break;
    }

    pointList.push_front(tmpPoint);
    moveSnake();
}

void Widget::moveSnake()
{
    QPainter painter;
    QPoint point = *(pointList.end() - 1);

    if(isGameOver())
    {
        return;
    }

    painter.begin(pix);
    if(eatFood())
    {
        snake->fill(Qt::white);
    }
    else
    {
        snake->fill(Qt::darkBlue);
        pointList.pop_back();
    }
    painter.drawPixmap(point, *snake);
    snake->fill(Qt::white);
    painter.drawPixmap(*(pointList.begin()), *snake);
    painter.end();

    update();
}


bool Widget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == this && QEvent::KeyPress == event->type())
    {
        QKeyEvent *e = dynamic_cast<QKeyEvent *>(event);
        if(Qt::Key_W == e->key() ||
           Qt::Key_A == e->key() ||
           Qt::Key_S == e->key() ||
           Qt::Key_D == e->key())
        {
            return true;
        }
    }

    return false;
}

bool Widget::isGameOver()
{
    bool ret = false;
    int count = 0;
    //撞墙
    QPoint point = *(pointList.begin());

    foreach(QPoint tmp, pointList)
    {
        if(point == tmp)
        {
            //count > 1:表示蛇撞到了自己的身体
            count++;
        }
    }

    if(point.x() < 0 || point.x() > 740 || point.y() < 0 || point.y() > 420 || count > 1)
    {
        //为widget安装事件过滤器, 过滤键盘事件中的某些按键
        this->installEventFilter(this);

        //停止定时器
        timer.stop();
        timer2.stop();

        label = new QLabel("Game   Over", this);

        //设置label的frame属性
        label->setFrameShape(QFrame::Panel);
        label->setFrameShadow(QFrame::Raised);
        //设置字体
        label->setFont(QFont("Ubuntu", 25));
        //设置字体颜色
        label->setStyleSheet("color: red");

        //设置label的最小尺寸, 默认的最小尺寸可能显示不完全
        label->setMinimumSize(200, 50);

        //设置label中的text对齐方式: 水平方向和垂直方向都对齐
        label->setAlignment(Qt::AlignCenter);

        //设置label的显示位置
        label->move(300, 200);
        label->show();

        ret = true;
    }

    return ret;
}

bool Widget::eatFood()
{
    bool ret =false;

    if(newFoodPoint == *(pointList.begin()))
    {
        ret = true;
        haveFood = false;
    }

    return ret;
}
