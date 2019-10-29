#include "mysqlquerymodel.h"

mySqlQueryModel::mySqlQueryModel(QObject *parent)
    :QSqlQueryModel(parent)
{

}

Qt::ItemFlags mySqlQueryModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags f;
    f = QSqlQueryModel::flags(index);

    if(3 == index.column())
    {
        f |= Qt::ItemIsEditable;
    }

    return f;
}

//自定义数据的跟新模式
bool mySqlQueryModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    //非第三列数据, 不能改
    if(3 != index.column())
    {
        return false;
    }

    //修改价格(价格是第三列数据)
    //使用SQL语句去更新数据库文件
    QSqlQuery query;
    query.prepare("UPDATE guns SET price=? WHERE id=?");

    //将新的价格填入第一个问号
    query.bindValue(0, value.toFloat());

    //序号对应的位置
    QModelIndex indexID = QSqlQueryModel::index(index.row(), 0);
    //将序号填入第二个位置
    query.bindValue(1, QSqlQueryModel::data(indexID).toInt());

    bool ok = true;
    ok = query.exec();
    if(!ok)
    {
        qDebug() << query.lastError().databaseText();
    }

    setQuery("SELECT * FROM guns");
    setHeaderData(0, Qt::Horizontal, "序号");
    setHeaderData(1, Qt::Horizontal, "类别");
    setHeaderData(2, Qt::Horizontal, "型号");
    setHeaderData(3, Qt::Horizontal, "价格");
    setHeaderData(4, Qt::Horizontal, "库存");
    setHeaderData(5, Qt::Horizontal, "备注");

    //返回数据更新与否的真实状态
    return ok;
}

//自定义数据的呈现方式
QVariant mySqlQueryModel::data(const QModelIndex &item, int role) const
{
    //先将数据交给基类处理，得到一个从数据库文件获取的原生数据value
    QVariant value = QSqlQueryModel::data(item, role);

    // 接下来，才能对这个原生数据value进行改造
    // 凡是第三列中的显示文本，一律在面前加个￥
    if(3 == item.column() && Qt::DisplayRole == role)
    {
        return QString("￥%1").arg(value.toFloat());
    }

    //凡是第四列中的颜色，一律为红色
    if(4 == item.column() && Qt::TextColorRole == role)
    {
        return QColor(Qt::red);
    }

    //凡是第一列中的对其方式，一律居中
    if(1 == item.column() && Qt::TextAlignmentRole == role)
    {
        return Qt::AlignCenter;
    }

    return value;
}
