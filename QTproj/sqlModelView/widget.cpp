#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->resize(800, 400);
    model = new mySqlQueryModel(this);
    ui->tableView->setModel(model);

    //打开数据库文件
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("guns.db");
    db.open();

    //fetch数据到model中
    model->setQuery("SELECT * FROM guns");
    model->setHeaderData(0, Qt::Horizontal, "序号");
    model->setHeaderData(1, Qt::Horizontal, "类别");
    model->setHeaderData(2, Qt::Horizontal, "型号");
    model->setHeaderData(3, Qt::Horizontal, "价格");
    model->setHeaderData(4, Qt::Horizontal, "库存");
    model->setHeaderData(5, Qt::Horizontal, "备注");

    //去掉tableView中自动生产的序号
    ui->tableView->verticalHeader()->hide();

    //为下拉框设置数据模型
    typeModel = new mySqlQueryModel(this);
    typeModel->setQuery("SELECT type FROM guns GROUP BY type");
    ui->comboBox->setModel(typeModel);
}

Widget::~Widget()
{
    delete ui;
}
