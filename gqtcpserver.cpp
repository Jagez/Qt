#include "gqtcpserver.h"
#include "gqtcpsocket.h"
//#include "filetransferthread.h"
#include <QThread>
#include <QDebug>
GQTcpServer::GQTcpServer(int maxClientConnCounts, QObject *parent) : QTcpServer(parent)
{
    //QHash类是一个模板类，提供一种基于词典哈希表
    //QHash<int, T>
    //初始化hash
    m_hashTcpClient = new QHash<int, TcpSocket *>;
    m_connectionCounts = 0;
    if (maxClientConnCounts > 100)
    {
        this->m_maxConnectNum = maxClientConnCounts;
    }
    else
    {
        //maxPendingConnections获取最大的连接数
        this->m_maxConnectNum = this->maxPendingConnections();
    }

    this->setMaxPendingConnections(maxClientConnCounts);
}
/*
 *
 * Note: If another socket is created in the reimplementation of this method,
 *  it needs to be added to the Pending Connections mechanism by calling addPendingConnection().
Note: If you want to handle an incoming connection as a new QTcpSocket object in another
thread you have to pass the socketDescriptor to the other thread and create the QTcpSocket object
there and use its setSocketDescriptor() method.
*/
void GQTcpServer::incomingConnection(qintptr handle)
{
    //判断是否超过最大连接数
    if (m_hashTcpClient->size() > this->m_maxConnectNum)
    {
        QTcpSocket tcpTemp;
        tcpTemp.setSocketDescriptor(handle);
        tcpTemp.disconnectFromHost();
        return;
    }
    //FileTransferThread *threadFile = new FileTransferThread(this);
    TcpSocket *tcptemp = new TcpSocket(handle);
    //断开连接，从列表中移除，并释放tcpsocket 重要！！！！！
    //connect(tcptemp, &TcpSocket::socketDisConnect, this, &GQTcpServer::socketDisConnectSlot);
    connect(tcptemp, &TcpSocket::readyRead, tcptemp, &TcpSocket::readData);
    //socket断开连接时发送信号
    //connect(this, &GQTcpServer::sentDisConn,tcptemp, &TcpSocket::sentDisConnSlot);
    //存储socket
    m_hashTcpClient->insert(handle, tcptemp);
    //创建子线程
    //FileTransferThread *tcpThread = new FileTransferThread(handle);
    //将子线程加入线程
    QThread *thread = new QThread(tcptemp);
    //移动线程
    tcptemp->moveToThread(thread);

    //断开连接时，线程退出
    //connect(tcptemp, &TcpSocket::disconnected, thread, &QThread::quit);
    //线程开启
    thread->start();


    //发送连接信号
    emit newConnection();       //重新实现incomingConnection要求发送该信号
    //emit connectClient(tcp->socketDescriptor(), tcp->peerAddress().toString(), tcp->peerPort());
    m_connectionCounts++;       //连接数自增
}


void GQTcpServer::readDataSlot(const int, const QString &, const quint16, const QByteArray &)
{

}

void GQTcpServer::socketDisConnectSlot(int handle,const QString & ip, quint16 prot,QThread * th)
{
    TcpSocket *socket = static_cast<TcpSocket *>(this->sender());
    //从socket列表中移除
    m_hashTcpClient->remove(handle);
    //通知线程管理那个线程断开
    //ThreadHandle::getClass().removeThread(th);
    //
    qDebug() << "socket was disconnect,thread id: " << QThread::currentThreadId();
}
