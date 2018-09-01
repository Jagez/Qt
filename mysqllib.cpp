#include "mysqllib.h"
#include <QMessageBox>
//#include <string.h>
#include <stdio.h>
#include <QModelIndex>


//封装MySQL功能的操作
MySQLLib::MySQLLib()
{
    mysql_init(&mysql);

    connection = NULL;
    memset(buf_error, 0, sizeof(buf_error));
}

MySQLLib::~MySQLLib()
{
    if (connection)
    {
        mysql_close(connection);
    }
}

int MySQLLib::connect_sql(const char *HostName, const char *User, const char *Passwd, const char *DbName)
{
    connection = mysql_real_connect(&mysql, HostName, User, Passwd, DbName, 0, 0, 0);

    if (connection == NULL)
    {
        memset(buf_error, 0, sizeof(buf_error));
        strcpy(buf_error, mysql_error(&mysql));
        //QMessageBox::information(0, "提示", mysql_error(&mysql));
        return -1;
    }
    else
    {
        mysql_query(&mysql, "SET NAMES utf8");
    }

    return 0;
}

int MySQLLib::open_sql(const char *sql, QStandardItemModel **p)
{
    if (mysql_query(connection, sql) != 0)
    {

        memset(buf_error, 0, sizeof(buf_error));
        strcpy(buf_error, mysql_error(&mysql));
        return -1;
    }
    MYSQL_RES *result = mysql_store_result(connection);
    if (result == NULL)
    {

        memset(buf_error, 0, sizeof(buf_error));
        strcpy(buf_error, mysql_error(&mysql));
        return -1;
    }

    int affectCount = mysql_affected_rows(connection);
    int fieldCount = mysql_field_count(connection);
    //测试
//    MYSQL_FIELD *field;
//    while (1)
//    {
//        field =  mysql_fetch_field(result);
//        if (field == NULL)
//        {
//            break;
//        }
//         QMessageBox::information(0, "执行失败", field->name);
//    }
//    while (1)
//    {
//         MYSQL_ROW row = mysql_fetch_row(result);
//         if  (row == NULL)
//         {
//             break;
//         }
//    }


    (*p) = new QStandardItemModel(affectCount, fieldCount);     //行， 列
    MYSQL_FIELD *field;
    int i = 0;  //行
    int j = 0;  //列
    for (i = 0; i < fieldCount; i++)
    {
         field = mysql_fetch_field(result);
         (*p)->setHeaderData(i, Qt::Horizontal, field->name);
    }


    for (i = 0;i < affectCount; i++)
    {
        MYSQL_ROW row = mysql_fetch_row(result);
        for (j = 0; j < fieldCount; j++)
        {
            (*p)->setData((*p)->index(i, j, QModelIndex()), row[j]);
        }
    }
    mysql_free_result(result);
    return 0;
}

int MySQLLib::open_str_sql()
{
    return 0;
}

int MySQLLib::exec_sql(const char *sql)
{
    if (mysql_query(connection, sql) != 0)
    {
        memset(buf_error, 0, sizeof(buf_error));
        strcpy(buf_error, mysql_error(&mysql));
        return -1;
    }
    return 0;
}



void MySQLLib::disconnect_sql()
{
    if (connection)
    {
        mysql_close(connection);
        connection = NULL;

    }
    return;
}

const char *MySQLLib::geterror()
{
    if (mysql_errno(&mysql) == 1062)
    {
        strcpy(buf_error, "mscId已存在！");
        return buf_error;
    }
    sprintf(buf_error, "%s, %d", buf_error, mysql_errno(&mysql));
    return buf_error;
}
