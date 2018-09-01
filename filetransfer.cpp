#include "filetransfer.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFileDialog>
#include <QFileInfo>
#include <QDateTime>
#include <QDir>
#include <QMessageBox>
#include <QHostAddress>
#include <QTextStream>
#include "tcpserverdlg.h"
#include "gqtcpsocket.h"

#define ARR_SUB_MAXCHILDITEM 2048
#define TCP_PORT 8888
#define TCP_ADDRESS "127.0.0.1"

FileTransfer::FileTransfer(QWidget *parent) : QDialog(parent)
{
    this->setWindowTitle("tcpSocketClient");
    //this->resize(700, 500);
    this->setFixedSize(700, 500);
    m_strFilePath = tr("C:/Users/MISAKI/workspace");
    //初始化UI
    this->init();
    //连接信号和槽
    this->signalConnect();

    //文件上传初始化
    this->filesUpLoad();
    TcpServer *tcpServerDlg = new TcpServer(this);
    tcpServerDlg->show();
}

void FileTransfer::areaAddWidget(QStringList fileList, QTreeWidgetItem *item)
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

void FileTransfer::addChildTreeWidget(QString path, QTreeWidgetItem *item)
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

void FileTransfer::init()
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
    isSend = false;

    //文件已接收大小
    m_receiveSize = 0;

    //初始化定时器
    m_timer = new QTimer(this);

    m_fileCounts = 0;
    sentFileCounts = 0;
    m_sentSize = 0;

    //m_hashSocketHead = new QHash<QTcpSocket *, QString>;

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

bool FileTransfer::tcpConnect()
{
    return true;
}

void FileTransfer::signalConnect()
{
    connect(m_btnSeleFiles, &QPushButton::clicked, this, &FileTransfer::on_seleFiles);
    connect(m_uploadFiles, &QPushButton::clicked, this, &FileTransfer::on_uploadFiles);
    connect(m_btnConnet, &QPushButton::clicked, this, &FileTransfer::on_connect);

}

void FileTransfer::filesUpLoad()
{

}

void FileTransfer::data(QTcpSocket *socket, QString fileName)
{
    //发送数据
    //头部信息  包含文件的大小和名称
    //组成头部信息
    QString head("head");
    int i = 0 ;
    m_strHead.append(head);
    m_strHead.append(tr("%1").arg(m_strListSelFiles.count()));
    m_strHead.append("@#");
    m_fileCounts = 0;
    //qDebug() << m_strListSelFiles.count();
    for (i; i < m_strListSelFiles.count(); i++)
    {
        QFileInfo info(m_strListSelFiles.at(i));
        m_strHead.append(info.fileName());
        m_strHead.append(tr("##%1;").arg(info.size()));
    }
    qDebug() << m_strHead << m_strHead.toStdString().data();

    //发送头部信息
//    const char *ch = m_strHead.toStdString().data();
//    int size = sizeof(m_strHead.toStdString().data());
    //发送多少就接收多少
    qint64  headSize = m_tcpSocket->write(m_strHead.toStdString().data());
    if (headSize > 0)               //头部信息发送成功
    {
        //发送完头部信息延时200毫秒
        //定时器
        connect(m_timer, &QTimer::timeout,
                [=](){
            this->sendData(socket,fileName);
            this->m_timer->stop();
            });
        m_timer->start(200);
    }
    else
    {
        qDebug() << "头部信息发送失败:" << __LINE__;
    }




}

void FileTransfer::sendData(QTcpSocket *socket, QString fileName)
{
    qint64 len = 0;
    qint64 sentsize = 0;    //已发送的每个文件大小
    //发送文件数据
    QString strfile = m_strHead.section("@#", 0, 0);
    QString fileAndSize = m_strHead.section("@#", 1, 1);
    QString file1 = fileAndSize.section("##", 0, 0);
    qDebug() << strfile << "file" << file1;
    qDebug() << m_strListSelFiles.count();

        QFile file(fileName);
        file.open(QIODevice::ReadOnly);
//            QTextStream stream(&file);
//            stream.setCodec("UTF-8");
        if (file.isOpen() == false)
            qDebug() << file.errorString();
        do
        {
            //每次发送数据的大小
            char buffer[4 * 1024] = {0};
            len = 0;
            //读取该文件的数据，每次读取buffer大小
            len = file.read(buffer, sizeof(buffer));
            //读取多少就发送多少
            len = socket->write(buffer, len);
            //已发送文件数据大小             每个文件发送了的大小
            sentsize += len;
            m_sentSize += len;          //总发送大小
        }while(sentsize != file.size());   //如果每个文件大小不等于发送了的大小则继续发送
    socket->disconnectFromHost();
    socket->close();
        file.close();
        this->m_timer->start(20);
}


void FileTransfer::timerOut()
{
//    this->m_timer->stop();
//    //int i = 0;
//    if (m_fileCounts < m_strListSelFiles.count())
//    {
//        this->sendData(m_strListSelFiles.at(m_fileCounts));
//        m_fileCounts++;                     //已发送的文件数
//    }
}

void FileTransfer::on_seleFiles()
{
    if (true == this->isSend)
    {
        //删除所有子节点
        m_treeWidget->clear();
        m_strListSelFiles.clear();
    }
    //警告:在对话执行过程中不要删除父类。如果您想这样做，您应该使用一个QFileDialog构造函数来创建对话框。
    m_strListSelFiles = QFileDialog::getOpenFileNames(this, tr("选择文件"),
                                  QDir::homePath());
    if (false == m_strListSelFiles.isEmpty())
    {
        //为没一个文件创建一个socket和头部信息
        int i= 0;
        for (i; i < m_strListSelFiles.count(); i++)
        {
            QTcpSocket *socket = new QTcpSocket;    //没指定父对象，断开连接时手动delete
            m_hashSocketHead.insert(socket, m_strListSelFiles.at(i));
            //连接服务器
            socket->connectToHost(QHostAddress(TCP_ADDRESS), TCP_PORT);
            //qDebug() << m_tcpSocket->errorString();
            //成功连接触发该信号
            connect(socket, &QTcpSocket::connected,
                    [=]()
                    {
                        qDebug() << "connected";
                    });
        }
        this->isSend = false;
        m_uploadFiles->setEnabled(true);
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

void FileTransfer::on_uploadFiles()
{


    m_btnSeleFiles->setEnabled(true);
    m_uploadFiles->setEnabled(false);

    if (true == m_strListSelFiles.isEmpty())
        return;

    //客户端发送数据,根据socket发送数据
    QHashIterator<QTcpSocket *, QString> iterator(m_hashSocketHead);
    int i = 0;
    while (iterator.hasNext())
    {
        iterator.next();
        auto tcp = iterator.key();
        this->data(tcp, iterator.value());
    }


    isSend = true;
    //清空文件列表
    //m_strListSelFiles.clear();
    m_uploadFiles->setEnabled(false);

}

void FileTransfer::on_connect()
{

    m_tcpSocket = new QTcpSocket(this);
    //连接服务器
    m_tcpSocket->connectToHost(QHostAddress(TCP_ADDRESS), TCP_PORT);
    //qDebug() << m_tcpSocket->errorString();
    //成功连接触发该信号
    connect(m_tcpSocket, &QTcpSocket::connected,
            [=]()
            {
                m_btnSeleFiles->setEnabled(true);
                m_uploadFiles->setEnabled(false);
                qDebug() << "connected";
            });
    //连接失败触发该信号
    connect(m_tcpSocket, &QTcpSocket::disconnected,
            [=]()
            {
                qDebug() << "disconnected";
            });

    //接收数据
    connect(m_tcpSocket, &QTcpSocket::readyRead,
            [=]()
            {
                QByteArray arr = m_tcpSocket->readAll();
                qDebug() << QString(arr.data());
            }
            );
}
