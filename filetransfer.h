#ifndef FILETRANSFER_H
#define FILETRANSFER_H

#include <QDialog>
#include <QTcpSocket>
#include <QLineEdit>
#include <QPushButton>
#include <QProgressBar>
#include <QScrollArea>
#include <QToolButton>
#include <QTreeWidget>
#include <QLabel>
#include <QTimer>
#include <QHash>

#define ARR_SUB_MAXVALUE 1024

class FileTransfer : public QDialog
{
    Q_OBJECT
public:
    explicit FileTransfer(QWidget *parent = nullptr);
    void areaAddWidget(QStringList fileList, QTreeWidgetItem *item);               //ScrollArea添加treeWidget
    void addChildTreeWidget(QString path, QTreeWidgetItem *item);

public:

    QPushButton *m_btnConnet;                      //connect
    QPushButton *m_btnSeleFiles;                    //选择文件按钮
    QPushButton *m_uploadFiles;                     //发送按钮
    QProgressBar *m_progressTransfer;               //进度条
    QLabel *label;
    QWidget *m_scrollArea;
    QTreeWidget *m_treeWidget;                      //节点树
    QTreeWidgetItem *m_treeItem;
    QToolButton *m_toolBtn[ARR_SUB_MAXVALUE];
    QString m_strFilePath;                          //文件树  打开的文件路径
    bool isSend;                                    //是否已发送     发送完就清空选择的文件
    QStringList m_strListSelFiles;                  //选择的文件
    int sentFileCounts;                             //已发送的文件数
     QHash<QTcpSocket *, QString> m_hashSocketHead;

private:
    void init();
    bool tcpConnect();
    void signalConnect();
    void filesUpLoad();
    void data(QTcpSocket *socket, QString fileName);
    void sendData(QTcpSocket *socket, QString fileName);
private:
    QTcpSocket *m_tcpSocket;                        //通信套接字
    QStringList m_strListtreeWidgetHeadKey;              //treeWidget的头部字段列表
    QStringList m_strListtreeWidgetHeadValue;            //treeWidget的头部字段每行的值列表
    QString m_strHead;                              //头部信息， 包含文件的全路径
    qint64 m_receiveSize;                          //已接收服务器文件大小
    qint64 m_sentSize;                             //已发送的文件大小
    QTimer *m_timer;                                 //定时器防止黏包
    int m_fileCounts;                                //已发送的文件数
signals:
    void startReadNextFile();

public slots:
    void on_seleFiles();
    void on_uploadFiles();
    void on_connect();
    void timerOut();

};

#endif // FILETRANSFER_H
