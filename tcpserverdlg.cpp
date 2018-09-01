#include "tcpserverdlg.h"
#include "gqtcpserver.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFileDialog>
#include <QFileInfo>
#include <QDateTime>
#include <QDir>
#include <QMessageBox>
#include <QHostAddress>

#define ARR_SUB_MAXCHILDITEM 2048
#define TCP_PORT 3306
#define TCP_ADDRESS "127.0.0.1"

TcpServer::TcpServer(QWidget *parent) : QDialog(parent)
{
    this->setWindowTitle("tcpSocketServer");
    //this->resize(700, 500);
    this->setFixedSize(700, 500);
    m_strFilePath = tr("C:/Users/MISAKI/workspace");
    //初始化UI
    this->init();
    //连接信号和槽
    this->signalConnect();

    //文件上传初始化
    this->filesUpLoad();

    //开始监听  connect
    //this->tcpConnect();




}

TcpServer::~TcpServer()
{
    //m_tcpServer->close();
}

void TcpServer::areaAddWidget(QStringList fileList, QTreeWidgetItem *item)
{
    int i = 0;
    QTreeWidgetItem *childItem[ARR_SUB_MAXCHILDITEM];
    for (i; i < fileList.count(); i++)
    {
        QFileInfo info(fileList.at(i));qDebug() << info.fileName();
        childItem[i] = new QTreeWidgetItem(QStringList()<< info.fileName() << tr("%1").arg(info.size() / 1024 / 1024)
                << tr("type") <<tr("%1").arg(info.fileTime(
        QFileDevice::FileMetadataChangeTime).toString("yyyy-MM-dd H:m:s")));
        m_treeWidget->addTopLevelItem(childItem[i]);
    }



    ////////////////////////////////////文件树start/////////////////////////////////////
    /*
    //上下文菜单
    m_treeWidget->setContextMenuPolicy(Qt::ActionsContextMenu);     //m_treeWidget的Actions
    m_strListtreeWidgetHeadKey << trUtf8("名称") <<tr("大小") << tr ("类型")
                               << tr("修改时间");
    m_treeWidget->setHeaderLabels(m_strListtreeWidgetHeadKey);
    QDir dir(m_strFilePath);
    if (m_strFilePath.isEmpty() || dir.exists() == false)
        return;
    //设置过滤文件类型
    dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoSymLinks |
                  QDir::NoDotAndDotDot | QDir::Readable);
    //设置文件排序规则  目录->文件  不区分大小写
    dir.setSorting(QDir::DirsFirst | QDir::Name | QDir::IgnoreCase);
    QFileInfo info(m_strFilePath);
    if (m_strFilePath.isEmpty())
    {
        return;
    }

    //qint64 value = Q_INT64_C(932838457459459);     //以独立于平台的方式包装带符号的64位整数文本。
    //每行的值列表
    m_strListtreeWidgetHeadValue << info.fileName() << tr("%1").arg(info.size() / 1024 / 1024)
         << tr("type") <<tr("%1").arg(info.fileTime(QFileDevice::FileMetadataChangeTime).toString("yyyy-MM-dd H:m:s"));
    //判断每行的值列表是否为空
    if (true == m_strListtreeWidgetHeadValue.isEmpty()) return;
    QTreeWidgetItem *treeItem = new QTreeWidgetItem(m_strListtreeWidgetHeadValue);
    //设置提示和图标
    treeItem->setToolTip(0, info.absoluteFilePath());
    treeItem->setIcon(0, QIcon(":/icon/icon/pack.png"));

    qDebug() << fileList.at(0);
    m_treeWidget->addTopLevelItem(treeItem);

    //添加子对象
    this->addChildTreeWidget(info.absoluteFilePath(), treeItem);

    m_treeWidget->expandToDepth(0);                     //展开深度为0
    */
    ////////////////////////////////////文件树end/////////////////////////////////////


}

void TcpServer::addChildTreeWidget(QString path, QTreeWidgetItem *item)
{

    ////////////////////////        文件树start            /////////////////////////////////////////
    /*QFileInfo info(m_strFilePath);
    if (true == path.isEmpty() || NULL == item)
    {
        return;
    }
    //检查路径是否可用
    QDir dir(path);
    if (false == dir.exists())
    {
        return;
    }
    dir.setFilter(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs | QDir::NoSymLinks
                   | QDir::Readable);
    dir.setSorting(QDir::DirsFirst | QDir::Name | QDir::IgnoreCase);
    QTreeWidgetItem *childItem[ARR_SUB_MAXCHILDITEM];
    int i = 0;
    foreach (QFileInfo info, dir.entryInfoList())
    {
        childItem[i] = new QTreeWidgetItem(QStringList()<< info.fileName() << tr("%1").arg(info.size() / 1024 / 1024)
                                           << tr("type") <<
                                           tr("%1").arg(info.fileTime(QFileDevice::FileMetadataChangeTime).toString("yyyy-MM-dd H:m:s")));
        childItem[i]->setToolTip(0, info.absolutePath());
        //childItem[i]->setText(1, info.absoluteFilePath());
        //设置item双击可编辑
        //childItem[i]->setFlags(childItem[i]->flags() | Qt::ItemIsEditable);
        item->addChild(childItem[i]);
        if (true == info.isDir())
        {
            childItem[i]->setIcon(0, QIcon(":/icon/icon/pack.png"));

            //递归调用
            this->addChildTreeWidget(info.absoluteFilePath(), childItem[i]);

            childItem[i]->setText(2, "2");
        }
        else
        {
            childItem[i]->setIcon(0, QIcon(":/icon/icon/mail.png"));
            childItem[i]->setText(2, "1");
        }
        i++;
    }
    if (dir.entryInfoList().count() >= ARR_SUB_MAXCHILDITEM)
    {
        QMessageBox::information(this, trUtf8("Tip"), trUtf8("列表数超出了%1个，可能无法显示全部").arg(ARR_SUB_MAXCHILDITEM));
    }
    */
    //////////////////////////     文件树end      //////////////////////////////////////////////
}

void TcpServer::init()
{
    m_btnConnet = new QPushButton(trUtf8("连接"), this);
    //m_btnConnet->setMinimumSize(QSize(320, 25));
    m_btnSeleFiles = new QPushButton(trUtf8("选择文件"), this);
    m_btnSeleFiles->setEnabled(false);
    m_uploadFiles = new QPushButton(trUtf8("send"), this);
    m_uploadFiles->setEnabled(false);
    label = new QLabel(tr("进度:"), this);
    m_progressTransfer = new QProgressBar(this);
    m_progressTransfer->setValue(0);
    m_progressTransfer->setMinimumSize(QSize(320, 50));
    label1 = new QLabel(tr(""), this);
    m_treeWidget = new QTreeWidget(this);
    m_treeWidget->setMinimumSize(QSize(550, 300));

//    此属性保存小部件的大小增量当用户调整窗口大小时，大小将以sizeIncrement().width()像素水平移动，sizeIncrement.height()像素垂直移动，baseSize()作为基础。
//    首选小部件大小为非负整数i和j:宽度= baseSize().width() + i * sizeIncrement() ().width();
//    高= baseSize().height() + j * sizeIncrement() ().height();
//    注意，虽然可以为所有小部件设置大小增量，但它只影响windows。
//    默认情况下，此属性包含一个宽度和高度为零的大小。
//    警告:在Windows下，大小增加没有影响，可能会被X11上的窗口管理器忽略。
    //m_treeWidget->setSizeIncrement(m_scrollArea->size());
    //m_treeWidget->setBaseSize(m_scrollArea->size());

    m_treeItem = new QTreeWidgetItem();
    m_strListtreeWidgetHeadKey << trUtf8("名称") <<tr("大小") << tr ("类型")
        << tr("修改时间");
    m_treeWidget->setHeaderLabels(m_strListtreeWidgetHeadKey);
    isSend = false;                     //是否已经发送了文件
    isStartReceive = false;             //是否开始接收文件，接收完头部信息再接收文件，防止粘包

    m_receiveSize = 0;                  //总大小
    index = 0;

    thread = new QThread(this);
    tempFileCounts = 0;

    QHBoxLayout *layoutTop = new QHBoxLayout;
    layoutTop->addStretch();
    layoutTop->addWidget(m_btnConnet);
    layoutTop->addStretch();
    layoutTop->addWidget(m_btnSeleFiles);
    layoutTop->addStretch();
    layoutTop->setMargin(15);

    QHBoxLayout *layoutPro = new QHBoxLayout;
    layoutPro->addStretch();
    layoutPro->addWidget(label);
    layoutPro->addWidget(m_progressTransfer);
    layoutPro->addWidget(label1);
    layoutPro->addStretch();
    layoutPro->addWidget(m_uploadFiles);
    layoutPro->addStretch();

    QHBoxLayout *layoutCenter = new QHBoxLayout;
    layoutCenter->addStretch();
    layoutCenter->setMargin(15);
    layoutCenter->addWidget(m_treeWidget);
    layoutCenter->addStretch();

    QVBoxLayout *vLayout = new QVBoxLayout(this);
    vLayout->addLayout(layoutTop);
    vLayout->addLayout(layoutPro);
    vLayout->addLayout(layoutCenter);
    vLayout->setMargin(15);



}

bool TcpServer::tcpConnect()
{
    //1
    m_tcpServer = new GQTcpServer(10000, this);
    //2
    bool flag = m_tcpServer->listen(QHostAddress::Any, 8888);

    if (false == flag)
    {
        return false;
        qDebug() << m_tcpServer->errorString();
    }


    //有新的client连接会触发
    //connect(m_tcpServer, &QTcpServer::newConnection, this, &TcpServer::newClientConnection);


    return true;
}

void TcpServer::signalConnect()
{
    QObject::connect(m_btnSeleFiles, &QPushButton::clicked, this, &TcpServer::on_seleFiles);
    QObject::connect(m_uploadFiles, &QPushButton::clicked, this, &TcpServer::on_uploadFiles);
    QObject::connect(m_btnConnet, &QPushButton::clicked, this, &TcpServer::on_connect);
    //窗口关闭，等待子线程退出并销毁
//    QObject::connect(this, &TcpServer::destroyed, this, &TcpServer::destroyThread);
}

void TcpServer::filesUpLoad()
{

}

void TcpServer::data()
{
    //组成头部信息
    QString head("head@#");
    int i = 0 ;
    m_strHead.append(head);
    for (i; i < m_strListSelFiles.count(); i++)
    {
        QFileInfo info(m_strListSelFiles.at(i));
        m_strHead.append(m_strListSelFiles.at(i));
        m_strHead.append(tr("##%1;").arg(info.size()));
    }
    qDebug() << m_strHead;

    //发送头部信息
    const char *ch = m_strHead.toStdString().data();
    m_tcpSocket->write(ch, sizeof(ch));

//    FILE *file = NULL;
//    const char ch[1024 * 4] = {0};
//    int len = 0;

}



void TcpServer::on_seleFiles()
{
    if (true == this->isSend)
    {
        //删除所有子节点
        m_treeWidget->clear();
    }
    //警告:在对话执行过程中不要删除父类。如果您想这样做，您应该使用一个QFileDialog构造函数来创建对话框。
    m_strListSelFiles = QFileDialog::getOpenFileNames(this, tr("选择文件"),
                                  QDir::homePath());
    if (false == m_strListSelFiles.isEmpty())
    {
        this->isSend = false;
        //m_treeWidget不能为空，这种操作应放在最后面！而且要判断是否为空
        if (NULL != m_treeWidget)
        {

            this->areaAddWidget(m_strListSelFiles, m_treeItem);
        }

    }
    else
    {
        return;
    }

}

void TcpServer::on_uploadFiles()
{
    if (true == m_strListSelFiles.isEmpty())
        return;

    //发送数据
    this->data();

    isSend = true;
}

void TcpServer::on_connect()
{
    if (false == this->tcpConnect())
    {
        return;
    }
}

void TcpServer::destroyThread()
{
    thread->quit();
    thread->wait();
}

void TcpServer::newClientConnection()
{
    //QTcpSocket *m_tcpSocket;                        //通信套接字
    //取出新连接进来的通信套接字
    //Note: The returned QTcpSocket object cannot be used from another thread.
//    If you want to use an incoming connection from another thread,
//            you need to override incomingConnection().
    m_tcpSocket = m_tcpServer->nextPendingConnection();
    qDebug() << m_tcpSocket->peerAddress().toString() << m_tcpSocket->peerPort();

    QObject::connect(m_tcpSocket, &QTcpSocket::readyRead, this, &TcpServer::receiveData);
}

void TcpServer::receiveData()
{
/*
    QByteArray arr = m_tcpSocket->readAll();
    m_fileCount = 0;
//    QString str(arr.data());
//    qDebug() << QString(arr.data());
    //先接收头部信息
    if (false == isStartReceive)
    {
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
            m_strListReceive << str.section(";", i, i);
        }
        //接收完头部
        this->isStartReceive = true;
        qDebug() << "head receiveed!";
    }
    else
    {
        if (0 <= tempFileCounts)
        {

            receiveThread[tempFileCounts] = new ReceiveFilesThread;
            //3、把定义好的类加入子线程中
            receiveThread[tempFileCounts]->moveToThread(thread);            //主线程 QApplication::instance()->thread()
            //4、启动子线程（并未调用线程处理函数）
//            if (false == thread->isRunning())
                thread->start(QThread::TimeCriticalPriority);   //参数为线程优先级
            //5、通过信号槽启动线程处理函数   指定线程处理函数接收receiveFiles()
            QObject::connect(this, &TcpServer::startThread, receiveThread[tempFileCounts], &ReceiveFilesThread::receiveFiles);
            //连接子线程发射的信号
            QObject::connect(receiveThread[tempFileCounts], &ReceiveFilesThread::startReceive, this, &TcpServer::dealThreadSignal);
            //不能直接调用线程处理函数，否则是会同一线程,只能通过信号槽
            emit startThread();


            tempFileCounts--;
        }

    }
    qDebug() << "当前进程号:" <<QThread::currentThread();
*/
}

void TcpServer::dealThreadSignal()
{
    QByteArray  arr = m_tcpSocket->readAll();
    if (true == isStartReceive)        //再接收文件
    {
        QFile file;
        int fileSize;
        qint64 receiveSize = 0;             //当前文件接收的大小
        //打开文件
        QString fileName = m_strListReceive.at(index);
        fileSize = fileName.section("##", 1, 1).toInt();
        fileName = fileName.section("##", 0, 0);

        file.setFileName(fileName);
        bool isOpen = file.open(QIODevice::WriteOnly);
        if (false == isOpen)
        {
            qDebug() << "open(QIODevice::WriteOnly) err:" << __LINE__;
        }
        qint64 writeSize = file.write(arr);
        m_receiveSize += writeSize;                 //累积接收的大小,总文件大小
        receiveSize += writeSize;                   //当前文件接收了的大小
        //写入的大小和文件的大小不一致
        if (writeSize != fileSize)
        {
            file.close();
            qDebug() << "writeSize != fileSize err:" << __LINE__;
            return;
        }
        if(receiveSize == fileSize)
        {
            qDebug() << fileName << "接收完毕";
        }
        file.close();

        ++index;

        //判断全部文件是否接收完
    }
}
