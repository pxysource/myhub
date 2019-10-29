#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "mysqlquerymodel.h"
#include <QSqlDatabase>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;

    //定义一个数据模型对象，用来给示图提供数据来源
    mySqlQueryModel *model;

    //定义一个数据模型对象，用来给下拉框提供数据来源
    mySqlQueryModel *typeModel;
};

#endif // WIDGET_H
