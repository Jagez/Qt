#ifndef RECEIVEFILESTHREAD_H
#define RECEIVEFILESTHREAD_H

#include <QObject>
#include <QTcpSocket>
#include <QFutureWatcher>
#include <QTimer>

class TcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpSocket(qintptr socketDescriptor, QObject *parent = nullptr);
    void futureWatcherTest();
    int test(int i, QString str);
    void myfinish();

public:
    bool isStartReceive;                            //是否开始接收文件
    qint64 m_receiveSize;                           //当前接收的大小
    QStringList m_strListReceive;                   //接收的文件名和大小组合的列表
    QStringList m_strListFileName;                  //接收的文件名列表
    int m_fileCount;                                //接收的文件数

    QFutureWatcher<int> watcher;
    QTimer *m_timer;

private:
    qintptr m_socketDescriptor;
    int index;                                      //读取第几个文件
    int tempFileCounts;
    QHash<QTcpSocket *, QString>  *m_hashFileSocket;    //文件名和socket的集合
    QMetaObject::Connection m_disconnect;

private:
    void init();
    void readFileData(QTcpSocket* tcp, QString fileName);

signals:
    void startReceive();
    //void readData();
    void socketDisConnect(int, const QString &, quint16, QThread *);

public slots:
    void readData();                    //线程函数
    void sentDataSlot(const QByteArray &data, const int handle);
    void sentDisConnSlot(int i);            //socket断开会触发该槽
    void futureWatcherTimer();
};

#endif // RECEIVEFILESTHREAD_H
