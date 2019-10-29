#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->addPhoto();
    //调整窗口大小为800*480
    this->resize(800, 480);
    //设置窗体无边框显示
    this->setWindowFlags(Qt::FramelessWindowHint);
    //取消布局和widget的间隔, 不取消, 就算取消边框也有一段空白间隔
    this->layout()->setMargin(0);

    //设置鼠标能触发移动事件
    this->setMouseTracking(true);

    labelDate = new QLabel(this);
    labelTime = new QLabel(this);
    labelWeather = new QLabel(this);
    labelDate->setMinimumWidth(140);
    //设置标签透明显示
    //labelDate->setStyleSheet("background:transparent");
    labelDate->move(560,20);
    labelTime->move(700,20);
    labelWeather->move(560, 40);
    labelWeather->setMinimumWidth(200);

    //在label上显示图片
    QSize size = ui->labelPhoto->size();
    QPixmap pp(":/res/1.jpg");
    //设置图片的pixmap缩放的尺寸
    pp.scaled(size);
    //这个属性的作用是允许（禁止）label缩放它的内容充满整个可用的空间
    ui->labelPhoto->setScaledContents(true);
    ui->labelPhoto->setPixmap(pp);

    //在label显示gif动图
    //QMovie *pp = new QMovie("/home/linux/Downloads/photo/2.gif");
    //ui->label->setMovie(pp);
    //pp->start();

    //显示日期
    labelDate->setText(QDate::currentDate().toString());
    //显示实时时间
    labelTime->setText(QTime::currentTime().toString());
    //设置1s超时, 超时则触发timeout()信号, 进行显示实时时间
    timer.start(1000);
    //播放图片定时
    photoTimer.start(3000);
    connect(&photoTimer, SIGNAL(timeout()), this, SLOT(displayPhoto()));
    connect(&timer, SIGNAL(timeout()), this, SLOT(timeDisplay()));

    //获取天气
    //http://iweather.market.alicloudapi.com/address?city=%E6%88%90%E9%83%BD&needday=1&prov=%E5%9B%9B%E5%B7%9D
    manager = new QNetworkAccessManager(this);
    QUrl url("http://iweather.market.alicloudapi.com/address?");
    QUrlQuery q;
    q.addQueryItem("city", "成都");
    q.addQueryItem("needday", "1");
    q.addQueryItem("prov", "四川");
    url.setQuery(q);

    //HTTP header
    request.setUrl(url);
    request.setRawHeader(QByteArray("Host"), QByteArray("iweather.market.alicloudapi.com"));
    request.setRawHeader(QByteArray("Authorization"), QByteArray("APPCODE e70c9fcbfbf04db69a60f0ae2592a9f1"));

    //收到HTTP回执信息
    reply = manager->get(request);
    connect(reply, SIGNAL(readyRead()), this, SLOT(readyReadSlot()));
}

Widget::~Widget()
{
    delete ui;
    delete labelDate;
    delete labelTime;
    delete labelWeather;
}

void Widget::addPhoto()
{
    QFileDialog file(this);
    QString filter = "*.*;;*.jpg;;*.jpeg;;*.bmp";
    photoList = file.getOpenFileNames(this, "addFile", "/home/linux/Downloads/photo", filter);
    file.show();
    photoIt = photoList.begin();
}

void Widget::mousePressEvent(QMouseEvent *e)
{
    //鼠标左键按下
    if(Qt::LeftButton == e->button())
    {
        begin = e->globalPos() - this->pos();
    }
    //鼠标右键按下关闭程序
    else if(Qt::RightButton == e->button())
    {
        this->close();
    }
}

void Widget::mouseMoveEvent(QMouseEvent *e)
{
    //鼠标左键按下并拖动窗口
    if(Qt::LeftButton & e->buttons())
    {
        this->move(e->globalPos() - begin);
    }
}

void Widget::timeDisplay()
{
    labelTime->setText(QTime::currentTime().toString());
}

void Widget::readyReadSlot()
{
    QString weather;
    QByteArray ba = reply->readAll();
    QJsonDocument jsonDoc(QJsonDocument::fromJson(ba));
    //ui->textBrowserMsg->append(QString(ba));
    QJsonObject root;
    QJsonObject	child;

    //Json文件的处理
    if(jsonDoc.isEmpty())
    {
        qDebug() << "error: can not get information of weather" << endl;
        labelWeather->setText("********");
        return;
    }
    root = jsonDoc.object();
    child = root.value("data").toObject();
    root = child;
    child = root.value("cityinfo").toObject();
    weather.push_back(child.value("provinces").toString());
    weather.push_back(" ");
    weather.push_back(child.value("city").toString());
    weather.push_back(" ");

    child = root.value("now").toObject();
    root = child;
    child = root.value("city").toObject();
    weather.push_back(child.value("weather").toString());
    labelWeather->setText(weather);
}

void Widget::displayPhoto()
{
    //在label上显示图片
    QSize size = ui->labelPhoto->size();
    if(photoList.end() == photoIt)
    {
        photoIt = photoList.begin();
    }
    QPixmap pp(*photoIt);
    //指向下一张图片的名字,
    photoIt++;
    //设置图片的pixmap缩放的尺寸
    pp.scaled(size);
    //这个属性的作用是允许（禁止）label缩放它的内容充满整个可用的空间
    ui->labelPhoto->setScaledContents(true);
    ui->labelPhoto->setPixmap(pp);
}
