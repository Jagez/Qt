#ifndef MYSQLLIB_H
#define MYSQLLIB_H

//Windows 使用MySQL.h
#include <windows.h>
#include <C:/Software/mysql-5.5.50-winx64/include/mysql.h>
//linux     使用MySQL.h
//#include <mysql/mysql.h>


#include <QStandardItemModel>
/*
 *  int st;         //linux下定义socket描述符
 *  SOCKET st;      //Linux下定义socket描述符
 *
*/
class MySQLLib
{
public:
    MySQLLib();
    ~MySQLLib();
    const char *geterror();
    int connect_sql(const char *HostName, const char *User, const char *Passwd, const char *DbName);
    int open_sql(const char *sql, QStandardItemModel **p);
    int open_str_sql();
    int exec_sql(const char *sql);
    void script_msg(const char *sql);
    void disconnect_sql();

private :
    MYSQL *connection;
    MYSQL mysql;
    char buf_error[1024];

};

#endif // MYSQLLIB_H
