#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->move(640, 0);
    this->setWindowFlags(Qt::FramelessWindowHint);

    //使得按钮可以被按下
    ui->pushButtonStartSlotCard->setCheckable(true);

    parkPrice = 2.2;

    connect(&timer, SIGNAL(timeout()), this, SLOT(detectCard()));
    //车辆入库时
    connect(this, SIGNAL(onDetected()), this, SLOT(getCardNumber()));
    //车辆出库时
    connect(this, SIGNAL(onDetectedOut()), this, SLOT(getStopInfo()));

    /*******************************数据库***********************************/
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("ParkSys.db");

    //打开并连接数据库
    if(!db.open())
    {
        qDebug() << "open database ParkSys.db failed";
    }

    //query默认关联打开了的数据库
    QSqlQuery query;
    QString SQL;
    SQL = "CREATE TABLE IF NOT EXISTS stopinfo(卡号 INTEGER PRIMARY KEY,车牌 TEXT,"
                                                "入库时间 TEXT)";
    if(!query.exec(SQL))
    {
        qDebug() << query.lastError().databaseText();
    }
    /***********************************************************************/

    /******************************tcpClient********************************/
    sock = new QTcpSocket(this);
    sock->connectToHost(QHostAddress(hostIp), port);

    if(!sock->waitForConnected(2*1000))
    {
        QMessageBox::information(this, "错误", sock->errorString(), QMessageBox::Ok);
        //连接失败做异常处理
    }
    else
    {
        qDebug() << "connection ok";
    }
}

Widget::~Widget()
{
    delete ui;
}

