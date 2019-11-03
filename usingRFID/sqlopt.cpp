#include "widget.h"

bool Widget::sqlIsCardNumberExist(uint32_t newid)
{
    QSqlQuery query;
    QString SQL;
    QSqlRecord record;
    bool ret = false;

    SQL = "SELECT * FROM stopinfo";
    if(!query.exec(SQL))
    {
        //注意这里应该进行异常处理，抛出异常等
        qDebug() << query.lastError().databaseText();
        ret = true;
    }
    else
    {
        record = query.record();
        int cardNoCol = record.indexOf("卡号");//index of the field "卡号"

        while(query.next())
        {
           if(newid == query.value(cardNoCol).toUInt())
           {
               ret = true;
               break;
           }
        }
    }

    return ret;
}

QString Widget::sqlGetCarNo(uint32_t newid)
{
    QSqlQuery query;
    QString SQL;
    QSqlRecord record;
    QString ret;
    ret.clear();

    SQL = "SELECT * FROM carinfo";
    if(!query.exec(SQL))
    {
        //注意这里应该进行异常处理，抛出异常等
        qDebug() << query.lastError().databaseText();
    }
    else
    {
        record = query.record();
        int cardNoCol = record.indexOf("卡号");//index of the field "卡号"

        while(query.next())
        {
           if(newid == query.value(cardNoCol).toUInt())
           {
               ret.append((query.value("车牌").toString()));
               break;
           }
        }
    }

    return ret;
}

bool Widget::sqlInsert(uint32_t newid)
{
    bool ret = true;
    QSqlQuery query;
    QString SQL;

    carNo =	sqlGetCarNo(newid);

    SQL = "INSERT INTO stopinfo(卡号,车牌,入库时间) VALUES(?,?,?)";
    query.prepare(SQL);

    query.bindValue(0, newid);
    query.bindValue(1, carNo);
    query.bindValue(2, QTime::currentTime().toString());

    //执行填充好未定数据的SQL语句，此时query.exec()中不需要指定SQL
    if(!query.exec())
    {
        qDebug() << query.lastError().databaseText();
        ret = false;
    }

    return ret;
}

QString Widget::sqlGetParkInfo(uint32_t newid)
{
    QSqlQuery query;
    QString SQL;
    QSqlRecord record;
    QString ret;
    ret.clear();

    SQL = "SELECT * FROM stopinfo";
    if(!query.exec(SQL))
    {
        //注意这里应该进行异常处理，抛出异常等
        qDebug() << query.lastError().databaseText();
    }
    else
    {
        record = query.record();
        int cardNoCol = record.indexOf("卡号");//index of the field "卡号"

        while(query.next())
        {
           if(newid == query.value(cardNoCol).toUInt())
           {
               ret.append(query.value("入库时间").toString());
               break;
           }
        }
    }

    return ret;
}
