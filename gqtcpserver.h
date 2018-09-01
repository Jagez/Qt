#ifndef GQTCPSERVER_H
#define GQTCPSERVER_H
#include <QTcpServer>
#include "gqtcpsocket.h"

class GQTcpServer : public QTcpServer
{
public:
    GQTcpServer(int maxClientConnCounts, QObject *parent);
    int m_maxConnectNum;
private:
    QHash<int,TcpSocket *> *m_hashTcpClient;
    int m_connectionCounts;

protected:
    //多线程实现文件传输，重新实现该方法 ，handlesocket描述符
    void incomingConnection(qintptr handle);

signals:
    void connectClient(const qintptr, const QString &, const quint16);
    void readData(const int, const QString &, quint16, const QByteArray &);
    void sentData(const QByteArray &, const int);
    void sentDisConn(int i);                     //断开连接时发送该信号

public slots:
    void readDataSlot(const int, const QString &, const quint16, const QByteArray &);
    void socketDisConnectSlot(int handle, const QString &ip, quint16 prot, QThread *th);
};

#endif // GQTCPSERVER_H
