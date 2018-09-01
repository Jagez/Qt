#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "gqtcpsocket.h"
#include <QDialog>
#include <QTcpSocket>
#include <QLineEdit>
#include <QPushButton>
#include <QProgressBar>
#include <QScrollArea>
#include <QToolButton>
#include <QTreeWidget>
#include <QLabel>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#define ARR_SUB_MAXVALUE 1024
#define ARR_SUB_MAXVTHREAD 1024
#define ARR_SUB_MAXVSOCKET 1024
class TcpServer : public QDialog
{
    Q_OBJECT
public:
    explicit TcpServer(QWidget *parent = nullptr);
    ~TcpServer();
    void areaAddWidget(QStringList fileList, QTreeWidgetItem *item);               //ScrollArea添加treeWidget
    void addChildTreeWidget(QString path, QTreeWidgetItem *item);

public:

    QPushButton *m_btnConnet;                      //connect
    QPushButton *m_btnSeleFiles;                    //选择文件按钮
    QPushButton *m_uploadFiles;                     //发送按钮
    QProgressBar *m_progressTransfer;               //进度条
    QLabel *label;
    QLabel *label1;
    QWidget *m_scrollArea;
    QTreeWidget *m_treeWidget;                      //节点树
    QTreeWidgetItem *m_treeItem;
    QToolButton *m_toolBtn[ARR_SUB_MAXVALUE];
    QString m_strFilePath;                          //文件树  打开的文件路径
    bool isSend;                                    //是否已发送     发送完就清空选择的文件
    QStringList m_strListSelFiles;                  //选择的文件
    bool isStartReceive;                            //是否开始接收文件
    qint64 m_receiveSize;                           //当前接收的大小
    QStringList m_strListReceive;                   //接收的文件列表
    int index;       //读取第几个文件


    int m_fileCount;                                //接收的文件数

private:
    void init();
    bool tcpConnect();
    void signalConnect();
    void filesUpLoad();
    void data();

private:
    QTcpSocket *m_tcpSocket;                        //通信套接字
    QTcpServer *m_tcpServer;                        //监听套接字
    QStringList m_strListtreeWidgetHeadKey;              //treeWidget的头部字段列表
    QStringList m_strListtreeWidgetHeadValue;            //treeWidget的头部字段每行的值列表

    QString m_strHead;                              //头部信息
    int tempFileCounts;

    //1、创建自定义类的对象，不指定父对象
    TcpServer *receiveThread[ARR_SUB_MAXVTHREAD];
    //2、创建QThread线程子对象
    QThread *thread;
    //多线程会覆盖
//    QByteArray  m_arr;                                //接收的所有数据

protected:


//    void incomingConnection(qintptr handle);

signals:
    void startThread();                             //发射该信号会触发子线程处理函数
public slots:
    void on_seleFiles();                            //点击选择文件时
    void on_uploadFiles();                          //点击发送文件时
    void on_connect();                              //点击连接时
    void destroyThread();                           //窗口关闭时会触发
private slots:
    void newClientConnection();                      //当有新连接进来时会触发此函数
    void receiveData();                              //当write时会触发该函数
    void dealThreadSignal();                         //处理子线程发射的信号，即接收文件
};

#endif // TCPSERVER_H
