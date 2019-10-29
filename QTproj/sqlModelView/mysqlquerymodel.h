#ifndef MYSQLQUERYMODEL_H
#define MYSQLQUERYMODEL_H

#include <QObject>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QModelIndex>
#include <QDebug>
#include <QSqlError>
#include <QColor>

// 继承并修改QSqlQueryModel，使之可以修改数据，
// 并对数据的展现加以渲染(以各种方式显示数据)
class mySqlQueryModel : public QSqlQueryModel
{
public:
    mySqlQueryModel(QObject *parent=Q_NULLPTR);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant data(const QModelIndex &item, int role) const;
};

#endif // MYSQLQUERYMODEL_H
