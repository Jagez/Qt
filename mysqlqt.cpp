#include "mysqlqt.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlDatabase>

MySQLQt::MySQLQt()
{

}

MySQLQt::~MySQLQt()
{

}

/**
 * @brief MySQLQt::connectSql       连接数据库
 * @param hostName                  主机名称
 * @param dbName                    数据库名称
 * @param userName                  MySQL用户名称
 * @param pwd                       用户密码
 * @param port                      端口
 * @param dbConn                    数据库连接信息
 * @param sqlErr                    错误信息
 */
bool MySQLQt::connectSql(QSqlDatabase &db1, QString hostName, QString dbName, QString userName, QString pwd,
                         QSqlError *sqlErr, QString connName, int port, QString sqlDervice)
{

    if (port == 0) port = 3306;
    if (hostName.isEmpty() || dbName.isEmpty() || userName.isEmpty() || pwd.isEmpty()) return false;
    //数据库配置
    QSqlDatabase dbConn;
    dbConn = QSqlDatabase::addDatabase(sqlDervice, "ggo");
    dbConn.setUserName("jage");
    dbConn.setHostName("192.168.146.166");
    dbConn.setPassword("283587151");
    dbConn.setPort(3306);
    dbConn.setDatabaseName("jage");
    //qDebug() << hostName << dbName<<userName<<pwd<<port << dbConn.isOpen() <<sqlDervice;
    db1 = QSqlDatabase::addDatabase(sqlDervice, connName);
    db1.setUserName(userName);
    db1.setHostName(hostName);
    db1.setPassword(pwd);
    db1.setPort(port);
    db1.setDatabaseName(dbName);
    if(false == db1.open())
    {
        qDebug() << "Error:" << db1.lastError().text();
        return false;
    }
    if(true == db1.isOpenError())
    {
        *sqlErr = db1.lastError();
        qDebug() << "Connect to Database Failed!"<< *sqlErr;
        return false;
    }
    //开始事务
    if (false == db1.transaction())
        qDebug() << "transcation err! Maybe mysql Engine is not support transaction.";
    return true;
}

void MySQLQt::disConnectSql(QSqlDatabase *dbConn)
{
    if (dbConn->isValid() && dbConn->isOpen())
    {
        //提交事务，关闭数据库连接
        dbConn->commit();
        dbConn->close();
    }
}

void MySQLQt::execSql(QSqlQuery *query, const QString sql)
{

}

bool MySQLQt::insertBatch(QSqlQuery *query, const QString tbName, QStringList list, int placeholder)
{
    if (query != NULL && !tbName.isEmpty() && !list.isEmpty() && placeholder != 0)
      {
          //组合占位符
          QStringList str;
          do {
              str << "?";
              placeholder--;
          }while(placeholder);
          QString string = str.join(",");

          QString sql = "insert into " + tbName + " values(" + string + ");";
          query->prepare(sql);
          //添加绑定
          while(placeholder)
          {
              query->addBindValue(list[placeholder]);
              placeholder--;
          }
          if(!query->execBatch(QSqlQuery::ValuesAsRows))
          {
              qDebug() << query->lastError();
              return false;
          }
          return true;

      }
    return true;
}

bool MySQLQt::insertLineFromFile(QSqlQuery *query, QString tbName, const QString filePath, int lineCount, int offsetStart)
{
    return true;
}


/**
 * @brief MySQLQt::insertBatch      批量插入数据
 * @param query
 * @param tbName                    表名
 * @param list                      数组指针
 * @param placeholder               占位符个数
 * @return 0成功 非0失败
 */


/**
 * @brief MySQLQt::getFields        获取表的所有字段名
 * @param query
 * @param tbName
 * @return QStringList
 */
QStringList MySQLQt::getFields(QSqlQuery *query, const QString tbName)
{
    if (query != NULL && tbName != NULL)
    {
        QStringList list;
        query->exec("select COLUMN_NAME from information_schema.COLUMNS where table_name = 'jage_user' and table_schema = 'jage';");
        while (query->next())
        {

            list << query->value(0).toString();
        }
        return list;
    }
}

/**
 * @brief MySQLQt::getFieldAllValue     获取字段的所有值
 * @param query
 * @param fieldName                     字段名
 * @param tbName                        表名
 * @return 成功返回QStringList
 */
QStringList MySQLQt::getFieldAllValue(QSqlQuery *query, const QString fieldName, QString tbName)
{
    QStringList resultList;
    //QSqlQuery query;
    QString sql = "select * from " + tbName;
    if (!query->exec(sql))
        qDebug() << query->lastError();
    int fieldNo = query->record().indexOf(fieldName);
    while (query->next())
    {
        QString fields = query->value(fieldNo).toString();
        resultList << fields;
    }
    return resultList;
}


/**
 * @brief MySQLQt::getFieldValue    获取某个字段的单个值
 * @param query                     sql操作
 * @param tbName                    表名
 * @param fieldName                 字段名
 * @param condition                 条件
 * @return 表示返回成功QString
 */
QString MySQLQt::getFieldValue(QSqlQuery *query, const QString tbName, const QString fieldName, QString condition)
{
    if (query != NULL && tbName != NULL && fieldName != NULL && condition != NULL)
    {
        QString sql = "select " + fieldName + " from " + tbName + " where " + condition + " ;";
        qDebug() << sql;
        query->exec(sql);
        while (query->next())
        {
            qDebug() << query->value(0).toString();
            return query->value(0).toString();
        }
    }
}

/**
 * @brief MySQLQt::getLineValue     获取一行
 * @param query
 * @param tbName
 * @param condition
 * @return 返回QStringList
 */
QStringList MySQLQt::getLineValue(QSqlQuery *query, QString tbName, const QString condition)
{
    if (query != NULL && tbName != NULL && condition != NULL)
    {
        QStringList list;
        QString sql = "select * from " + tbName + " where " + condition + " ;";
        qDebug() << sql;
        while (query->next())
        {
            for (int i = 0; i <= this->getFields(query, tbName).length(); i++)
            {
                list << query->value(i).toString();
                qDebug() << list;
            }
        }
        return list;
    }
}
/**
 * @brief MySQLQt::getRowAffected
 * @param dbConn
 * @param query
 * @return 成功返回影响行数，失败返回-1
 */
int MySQLQt::getRowAffected(QSqlQuery *query, QSqlQuery *q)
{
    if (q->isValid() && q != NULL)
    {
        return q->numRowsAffected();
    }
    else
    {
        return -1;
    }
}
