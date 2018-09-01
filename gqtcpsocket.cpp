#include "gqtcpsocket.h"
#include <QDebug>
#include <QThread>
#include <QProcess>
#include <QHostAddress>
#include <QFile>
#include <QQueue>
#include <QImage>

//The Qt Concurrent module extends the basic threading support found in
//Qt Core module and simplifies the development of code that can be executed
//in parallel on all available CPU cores.
#include <QtConcurrent/QtConcurrent>

TcpSocket::TcpSocket(qintptr socketDescriptor, QObject *parent) : QTcpSocket(parent),
    m_socketDescriptor(socketDescriptor)
{
    /*
     *
     * 套接字以openMode指定的模式打开，并输入由socketState指定的套接字状态。
     * 清除读写缓冲区，丢弃所有挂起的数据。
     注意:不能用相同的本机套接字描述符初始化两个抽象套接字。
     * */
    this->setSocketDescriptor(socketDescriptor);
    this->init();
    futureWatcherTest();
}

void TcpSocket::futureWatcherTest()
{
    //QFutureWatcher
    //Be aware that not all asynchronous computations can be canceled or paused.
    //For example, the future returned by QtConcurrent::run() cannot be canceled;
    //but the future returned by QtConcurrent::mappedReduced() can.
    //This is useful if only status or progress information is needed; not the actual result data.
    connect(&watcher, &QFutureWatcher<int>::finished, this, &TcpSocket::myfinish);
    connect(&watcher, &QFutureWatcher<int>::canceled, this, &TcpSocket::myfinish);
    int i = 0;QString str = "str";
    //run是阻塞的 在新线程中运行函数
    //QFuture<int> 的返回类型必须与 &TcpSocket::test函数返回类型一致 int
    //指定线程池
    //QThreadPool pool;
    //QFuture<int> future = QtConcurrent::run(&pool, this, &TcpSocket::test, i, str);
    QFuture<int> future = QtConcurrent::run(this, &TcpSocket::test, i, str);
    watcher.setFuture(future);
    future.result();
    watcher.pause();
    //设置定时器重新启动watcher.resume();
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &TcpSocket::futureWatcherTimer);
    //QtConcurrent::map()
    m_timer->start(1000);

    //常量成员函数
    //arr是QByteArray::split的被哪个对象调用，'.'为QByteArray::split形参
//    QByteArray arr = "hello.world!";
//    QFuture<QList<QByteArray>> fu = QtConcurrent::run(arr, &QByteArray::split, '.');
//    QList<QByteArray> list = fu.result();
//    qDebug() << list.at(0) << list.at(1);
    //非常量成员函数
    //img是QImage::invertPixels的被哪个对象调用，QImage::InvertRgba为QImage::invertPixels形参
//    QImage img(":/icon/icon/find.png");
//    QFuture<void> fu = QtConcurrent::run(&img, &QImage::invertPixels, QImage::InvertRgba);
//    fu.waitForFinished();
//    img.save("./xx1.png");

    //消息队列模板类
//    QQueue<int> *queue = new QQueue<int>;
//    queue->enqueue(1);
//    queue->enqueue(2);
//    queue->enqueue(4);
//    while (false == queue->isEmpty())
//        qDebug() << queue->dequeue();
}

int TcpSocket::test(int i, QString str)
{
    //qDebug() << i << str << QThread::currentThread();
//    for(i; i < 100000; i++)
//    {
//        qDebug() << i;
//    }
    return 0;
}

void TcpSocket::myfinish()
{

}


void TcpSocket::init()
{

    this->isStartReceive = false;
    this->index = 0;
    this->m_fileCount = 0;
    this->m_receiveSize = 0;
    this->tempFileCounts = 0;

    m_disconnect = connect(this, &TcpSocket::disconnected,
                   [&]()
                    {
                        emit socketDisConnect(m_socketDescriptor, this->peerAddress().toString(),
                                              this->peerPort(), QThread::currentThread());
                        //this->deleteLater();
                    });
}

void TcpSocket::readFileData(QTcpSocket *tcp, QString fileName)
{

}

void TcpSocket::readData()
{
    //QProcess::execute("C:/Users/MISAKI/Documents/QTWorkspace/GOSICK/command/pngcheck.exe");
    //QProcess::execute("C:/Software/LogView/LogView.exe");
    //qDebug() << "IncomingConnetion Thread: " << QThread::currentThreadId();
//    emit startReceive();

    //qobject_cast      standard C++ dynamic_cast()
//    Returns the given object cast to type T if the object is of type T (or of a subclass);
//    otherwise returns 0. If object is 0 then it will also return 0.
//    The class T must inherit (directly or indirectly) QObject and
//    be declared with the Q_OBJECT macro.
//    A class is considered to inherit itself.

    //获取当前socket
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(this->sender());
    qDebug() << "head: " << QThread::currentThread();
    //先接收头部信息
    if (false == isStartReceive)
    {
        QByteArray  arr = socket->readAll();
        QString str;
        QString head;
        head = QString(arr).section("@#", 0, 0);
        qDebug() << "head:" << arr;
        //获取文件数
        m_fileCount = head.section("ead", 1, 1).toInt();
        tempFileCounts = head.section("ead", 1, 1).toInt();
        qDebug() << "tempFileCounts" << tempFileCounts << "isStartReceive: " << isStartReceive;
        str = QString(arr).section("@#", 1, 1);
        int i = 0;
        for (i; i < m_fileCount; i++)
        {
            m_strListReceive << str.section(";", i, i);         //组合所有文件的名称和大小
            m_strListFileName << m_strListReceive.at(0);        //组合所有的文件名
        }
        //接收完头部
        this->isStartReceive = true;
        //计算总文件大小

        qDebug() << "head receiveed!";
    }
    //再接收文件
    else
    {
        //while (isStartReceive)
        {
            //readFileData(socket, str);
            QFile file;
            int fileSize;
            qint64 receiveSize = 0;             //当前文件接收的大小
            //打开文件
            QString fileName;
            fileName = m_strListReceive.at(index);
            if (true == fileName.isEmpty())
            {
                qDebug() << "fileName.isEmpty():" << __LINE__;
                return;
            }
            fileSize = fileName.section("##", 1, 1).toInt();
            fileName = fileName.section("##", 0, 0);

            file.setFileName(fileName);
            if (false == file.isOpen())
            {
                bool isOpen = file.open(QIODevice::WriteOnly | QIODevice::Append);
                if (false == isOpen)
                {
                    qDebug() << "open(QIODevice::WriteOnly) err:" << __LINE__;
                }
                //++index;
            }


            //文件是否接收完
            if (m_receiveSize < fileSize)
            {
                QByteArray  arr = socket->readAll();
                qint64 writeSize = file.write(arr);
                m_receiveSize += writeSize;                 //累积接收的大小,总文件大小
                receiveSize += writeSize;                   //当前文件接收了的大小
                qDebug() << m_receiveSize;
            }
            //写入的大小和文件的大小不一致
            if (m_receiveSize != fileSize)
            {


                //qDebug() << "writeSize != fileSize err:" << __LINE__;
                return;
            }

            qDebug() << m_receiveSize;
            //某文件接收完毕
            if(m_receiveSize == fileSize)
            {
                qDebug() << fileName << "接收完毕";
                file.flush();
                file.close();
                //断开信号槽的连接
                //this->disconnect(m_disconnect);
                this->disconnectFromHost();
                //this->deleteLater();
                //this->isStartReceive = false;
                //break;
            }



            //判断全部文件是否接收完
        }

        //qDebug() << "当前进程号:" <<QThread::currentThread();

    }

}
void TcpSocket::sentDataSlot(const QByteArray &data, const int handle)
{

}

void TcpSocket::sentDisConnSlot(int i)
{  /*  if (i == socketID)
    {
        this->disconnectFromHost();
    }
    else if (i == -1) //-1为全部断开
    {
        disconnect(dis); //先断开连接的信号槽，防止二次析构
        this->disconnectFromHost();
        this->deleteLater();
    }*/


}

void TcpSocket::futureWatcherTimer()
{
    watcher.resume();
    m_timer->stop();
}
