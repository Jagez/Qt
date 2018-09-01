#ifndef MYSQLQT_H
#define MYSQLQT_H
#include <QStringList>
#include <QSqlDatabase>

class MySQLQt
{
public:
    MySQLQt();
    ~MySQLQt();

public:
//连接数据库
    bool connectSql(QSqlDatabase &db1, QString hostName, QString dbName, QString userName, QString pwd, QSqlError *sqlErr/* out */, QString connName = "", int port = 3306, QString sqlDervice = QString("QMYSQL"));
//断开数据库连接
    void disConnectSql(QSqlDatabase *dbConn/* in */);
//执行sql语句
    void execSql(QSqlQuery *query/* in */, const QString sql);
//批量插入数据
    bool insertBatch(QSqlQuery *query/* in */, const QString tbName, QStringList list/* in */, int placeholder);
//从文件中读取行进行行插入
    bool insertLineFromFile(QSqlQuery *query/* in */, QString tbName, const QString filePath, int lineCount, int offsetStart);
//获取表的字段名称
    QStringList getFields(QSqlQuery *query/* in */, const QString tbName);
//获取表某字段的所有数据
    QStringList getFieldAllValue(QSqlQuery *query/* in */, const QString fieldName, QString tbName);
//获取某个字段的值
    QString getFieldValue(QSqlQuery *query/* in */, const QString tbName, const QString fieldName, QString condition);
//获取每行数据
    QStringList getLineValue(QSqlQuery *query/* in */, QString tbName, const QString condition);
//获取影响的行数
    int getRowAffected(QSqlQuery *query/* in */, QSqlQuery *q);
//获取数据库连接选项
    QString getConnectOpts(QSqlDatabase *dbConn/* in */);
    void showDrivers();

private:

};

#endif // MYSQLQT_H
