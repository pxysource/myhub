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

    //使得按钮可以被按下
    ui->pushButtonRecognizeCardNo->setCheckable(true);
    //开始的时候注册按钮无效
    ui->pushButtonRegister->setDisabled(true);

    connect(&timer, SIGNAL(timeout()), this, SLOT(detectCard()));
    connect(this, SIGNAL(onDetected()), this, SLOT(getCardNumber()));

    model = new QSqlQueryModel(this);
    ui->tableView->setModel(model);
    //去掉tableView中自动生产的序号
    ui->tableView->verticalHeader()->hide();

    //管理SQLITE数据库
    //ParkSys.db: 1.carinfo:车辆登记注册信息(卡号, 车牌号)
    //            2.stopinfo:停车信息(卡号, 车牌号, 停车时间)
    //创建数据库固定格式(至少暂时我了解是这样)
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("ParkSys.db");

    //打开并连接数据库
    if(!db.open())
    {
        qDebug() << "open  database ParkSys.db failed";
    }

    QSqlQuery query;
    //构建一张表:carinfo
    SQL = "CREATE TABLE IF NOT EXISTS carinfo(卡号 INTEGER PRIMARY KEY, 车牌 TEXT)";
    if(!query.exec(SQL))
    {
        qDebug() << query.lastError().databaseText();
    }

    sqlShow();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::sqlShow()
{
    /**********************在调试窗口打印数据库的表carinfo的信息****************/
#if 0
    QSqlQuery query;
    SQL = "SELECT * FROM carinfo";

    if(!query.exec(SQL))
    {
        qDebug() << query.lastError().databaseText();
        return;
    }

    // 以行为单位展示查询结果
    while(query.next())
    {
        int     cardId = query.record().value(0).toInt();
        QString carNo  = query.record().value(2).toString().trimmed();

        qDebug() << qPrintable(QString("[%1] %2").arg(cardId).arg(carNo));
    }
#endif
    /***********************************************************************/

    //fetch数据到model中
    model->setHeaderData(0, Qt::Horizontal, "卡号");
    model->setHeaderData(1, Qt::Horizontal, "车牌");
    model->setQuery("SELECT * FROM carinfo");
}

void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, QPixmap(":/res/registerInte.jpeg"));
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    //点击鼠标右键，退出程序
    if(Qt::RightButton == event->button())
    {
        this->close();
    }
}

//从文件批量录入车辆的卡号注册信息
int Widget::sqlUserRegister()
{
    QSqlQuery query;
    //从文件批量录入数据
    QFile file("cardID.txt");

    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << __LINE__ << __FILE__ << __FUNCTION__ << file.errorString();
        return -1;
    }

    QByteArray ba;
    QString line;

    //使用文件的信息来进行批量插入操作
    //将插入操作中相同的SQL语句进行prepare，未定的数据用问号代替
    SQL = "INSERT INTO carinfo(卡号, 车牌) VALUES(?,?)";
    query.prepare(SQL);

    // 一行一行地读文件的数据
    for(int i = 1; !file.atEnd(); i++)
    {
        // 遇到空行，略过
        if((ba=file.readLine()).isEmpty())
        {
            continue;
        }

        // 遇到只包含空白符（比如空格、换行符、制表符)，略过
        if((line=QString(ba)).trimmed().isEmpty())
        {
            continue;
        }

        // 将一行中的数据，以逗号分隔开
        QStringList tmp = line.split(QChar(','));

        // 往SQL中的问号填充数据，问号的序号从0开始
        query.bindValue(0, QString(tmp[0]).toInt()); // 卡号
        query.bindValue(1, tmp[1]); // 车牌

        // 执行填充好未定数据的SQL语句，此时query.exec()中不需要指定SQL
        if(!query.exec())
        {
            qDebug() << query.lastError().databaseText();
            break;
        }
    }

    //查看数据
    sqlShow();

    return 0;
}


void Widget::on_pushButtonExit_clicked()
{
    this->close();
}

void Widget::on_pushButtonRegister_clicked()
{
    //注册进度显示(.gif)
    QMovie pp(":/res/progress.gif");
    pp.setScaledSize(QSize(100, 100));
    ui->labelProgress->setMovie(&pp);
    pp.start();

    //注册按钮禁用
    ui->pushButtonRegister->setDisabled(true);

    QString carNo = ui->LineEditCarNumber->text();
    QString newid = ui->LineEditCardNumber->text();
    ui->textBrowserStatus->append(QString("%1%2").arg(carNo).arg("的车主注册成功"));
    ui->textBrowserStatus->append(QString("%1%2").arg("卡号为:").arg(newid));

    QSqlQuery query;
    //插入数据到数据库ParkSys.db的carinfo表中
    SQL = QString("INSERT INTO carinfo value(%1,'%2')").arg(newid).arg(carNo);
    if(!query.exec(SQL))
    {
        qDebug() << query.lastError().databaseText();
    }

    ui->labelProgress->clear();
    sqlShow();
}
