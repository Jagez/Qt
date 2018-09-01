#include "filemanagewidget.h"
#include <QDebug>
#include <QAction>
#include <QMenu>
#include <QMessageBox>

FileManageWidget::FileManageWidget(QString filePath, QWidget *parent) : QWidget(parent)
{
    this->FilePath = filePath;
    widget = new QWidget(this);
   // dock = new QDockWidget(tr("dock"), widget);
    treeWidget = new QTreeWidget(this);
    init(FilePath);
    //this->addDockWidget(Qt::LeftDockWidgetArea, this->dock);
    this->scroll();
}

FileManageWidget::~FileManageWidget()
{
//    if (treeWidget != NULL)
//    {
//        delete treeWidget;
//        treeWidget = NULL;
//    }
//    if (textFile != NULL)
//    {
//        delete textFile;
//        textFile = NULL;
//    }
}

void FileManageWidget::init(QString FilePath)
{
    textFile = new QTextEdit(this);


    //自定义上下文菜单
    connect(treeWidget, &QTreeWidget::customContextMenuRequested, this,
            &FileManageWidget::popMenuSlot);
    //item双击事件，触发读取文件
    connect(treeWidget, &QTreeWidget::itemDoubleClicked, this, &FileManageWidget::fileReadSlot);
    //connect(treeWidget, &QTreeWidget::itemChanged, this, &FileManageWidget::saveModifySlot);

/*
    //停靠1
    dock1 = new QDockWidget(tr("停靠1"), widget);
    dock1->setFeatures(QDockWidget::NoDockWidgetFeatures);  //QDockWidget::DockWidgetMovable    设置可移动
    dock1->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);    //设置停靠位置
    QTextEdit *text01 = new QTextEdit(widget);
    text01->setText(tr("可靠左右"));
    dock1->setWidget(text01);
    //this->addDockWidget(Qt::RightDockWidgetArea, dock1);       //将窗口放在右边

    //停靠2
    dock2 = new QDockWidget(tr("停靠2"), widget);
    dock2->setFeatures(QDockWidget::NoDockWidgetFeatures);
    QTextEdit *text02 = new QTextEdit(tr("右上角关闭和浮动按钮"), widget);
    dock2->setWidget(text02);
    //this->addDockWidget(Qt::RightDockWidgetArea, dock2);

    //停靠3
    dock3 = new QDockWidget(tr("停靠3"), widget);
    dock3->setFeatures(QDockWidget::NoDockWidgetFeatures);  //允许全部特性    QDockWidget::AllDockWidgetFeatures
    QTextEdit *text03 = new QTextEdit(tr("12"), widget);
    dock3->setWidget(text03);
    //this->addDockWidget(Qt::RightDockWidgetArea, dock3);

    //停靠4
    dock4 = new QDockWidget(tr("停靠3"), widget);
    dock4->setFeatures(QDockWidget::AllDockWidgetFeatures);  //允许全部特性
    QTextEdit *text04 = new QTextEdit(tr("213"), widget);
    dock4->setWidget(text04);
    //this->addDockWidget(Qt::BottomDockWidgetArea, dock4);


    dock1Action = dock1->toggleViewAction();
        dock1Action->setIcon(QIcon(":/icon/110 (2).png"));
        connect(dock1Action, SIGNAL(toggled(bool)), this, SLOT(setActiveDock1(bool)));

        dock2Action = dock2->toggleViewAction();
        dock2Action->setIcon(QIcon(":/icon/110 (3).png"));
        connect(dock2Action, SIGNAL(toggled(bool)), this, SLOT(setActiveDock2(bool)));

        dock3Action = dock3->toggleViewAction();
        dock3Action->setIcon(QIcon(":/icon/110 (4).png"));
        connect(dock3Action, SIGNAL(toggled(bool)), this, SLOT(setActiveDock3(bool)));
*/
    //connect(tree, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(popMenu(QPoint)));
    //item双击事件，触发读取文件
    //connect(tree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(fileRead(QTreeWidgetItem *, int)));
    //connect(tree, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this, SLOT(saveModify(QTreeWidgetItem *, int)));
}

//顶层item
/*
 *      遍历目录下的文件，分别将文件放入filestringlist，将目录放入dirstringlist
 *      再次遍历
*/
void FileManageWidget::fileTree()
{
    //最顶层的项用addTopLevelItem来加入，每个项的子项用addChild来加入。
    static int flag = 0;
    //设置字段名
    //treeWidget->setHeaderLabels(QStringList()<<"Key"<<"Value");


    QDir dir(FilePath);
    //treeWidget->setHeaderLabels(QStringList()<<FilePath);
    if (!dir.exists())          //判断路径是否存在
        return;
    //setfilter过滤规则 类型过滤
    dir.setFilter(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs | QDir::NoSymLinks);
    //setSorting设置排序规则 DirsFirst首先放置目录，然后将文件。Name	按名称排序 不区分大小写
    dir.setSorting(QDir::DirsFirst | QDir::Name | QDir::IgnoreCase);
    /*          迭代器
    QStringList filters;
    filters<<QString("*.*")<<QString("*");          //文件名过滤
    dir.setNameFilters(filters);
    //dir.setFilter(QDir::Filters | QDir::Dirs | QDir::NoSymLinks);  //setfilter过滤规则 类型过滤
    QDirIterator dirIterator(FilePath, filters,
                              QDir::Filters | QDir::NoSymLinks,
                              QDirIterator::Subdirectories);
    QStringList stringList;
    while (dirIterator.hasNext())
    {
        dirIterator.next();
        QFileInfo fileInfo = dirIterator.fileInfo();
        //QString absoluteFilePath = fileInfo.absoluteFilePath();
        if (fileInfo.isDir())
        {
            qDebug() << "isdir:";
        }
        stringList.append(fileInfo.fileName());            //QStringList 字符串追加
    }
*/
    QFileInfo mfi(FilePath);
    QTreeWidgetItem *item = new QTreeWidgetItem(QStringList()<<mfi.fileName());
    item->setToolTip(0, mfi.absoluteFilePath());
    item->setIcon(0, QIcon(":/icon/icon/dir.png"));
    treeWidget->addTopLevelItem(item);
    addChild(mfi.absoluteFilePath(), item);
    treeWidget->expandToDepth(0);
//    int i = 0;

//    QTreeWidgetItem *item[496];
//    foreach (QFileInfo mfi, dir.entryInfoList())
//    {

//        item[i] = new QTreeWidgetItem(QStringList()<<mfi.fileName());
//        /*
//        *       事件处理
//        */

//        //item[i]->setCheckState(0, Qt::Checked);
//        treeWidget->addTopLevelItem(item[i]);
//        item[i]->setToolTip(0, mfi.absoluteFilePath());
//        item[i]->setText(1, mfi.absoluteFilePath());    //这里通过设置item的列字符串传递文件的绝对路径
//        //item[i]->setFlags(item[i]->flags() | Qt::ItemIsEditable); //双击可编辑
//        flag = flag + 1;
//        /* 如果是目录则添加子文件或目录 */
//        if (mfi.isDir())
//        {
//            item[i]->setIcon(0, QIcon(":/icon/dir.png"));
//            addChild(mfi.absoluteFilePath(), item[i]);
//            item[i]->setText(2, "0");     //设置第三列字符串， 标识是否为文件
//        }
//        else
//        {
//            item[i]->setIcon(0, QIcon(":/icon/file.png"));
//            item[i]->setText(2, "1");     //设置第三列字符串， 标识是否为文件
//        }
//        i++;
//    }
    //verticalLayout->addWidget(treeWidget);
}

void FileManageWidget::scroll()
{

    //    dirModel.setRootPath(QDir::currentPath());
    //    dirModel.setReadOnly(true);
    //    resTree.setModel(&dirModel);
    //    resTree.setRootIndex(dirModel.index("./"));
    //    currListView.setModel(&dirModel);
    //    currListView.setRootIndex(dirModel.index("./"));
    //    connect(NULL, SIGNAL(clicked(QModelIndex)), this, SLOT(UpdateCurrentTab(const QModelIndex&)));



//        int fileCount = 0;
//        dock->setFeatures(QDockWidget::AllDockWidgetFeatures);
//        dock->setWidget(treeWidget);



     /*     滚动条
        dockWidgetContents = new QWidget();
        pScroll = new QScrollArea(this);
        dock = new QDockWidget( );   //tr("DockWindow 1"), this

        pScroll->setWidget(dockWidgetContents);       //pScroll->setWidget(dock / widget); 出现bug
        pScroll->setBackgroundRole(QPalette::NoRole);   //Dark
        QGridLayout *gridLayout = new QGridLayout(dock);
        gridLayout->addWidget(pScroll);
        widget = new QWidget();     //dockWidgetContents
        widget->setObjectName(QString::fromUtf8("widget"));
        //widget->setGeometry(QRect(0, 0, 400, 1700));
        //dock->setMinimumSize(QSize(110, 41));     //设置widget最小值
        dock->setMaximumSize(300, 1700);             //设置widget最大值
        dock->setFeatures(QDockWidget::DockWidgetMovable  ); //可移动的特征
        //dock->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        //允许移动的区域
        verticalLayout = new QVBoxLayout(widget);   //原本是widget;
        //verticalLayout->addWidget(pScroll);
        //verticalLayout->setSpacing(15);
        //verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        //verticalLayout->setContentsMargins(0, 0, 0, 0);
        //verticalLayout->addStretch(1);
    */
        //遍历文件
        //foreachFile(FilePath);


        //上下文菜单
        //右键菜单实现的第二中方法
        treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);

        //右键菜单实现第三种方法 使用Qt::ActionsContextMenu显示所有的action
        //treeWidget->setContextMenuPolicy(Qt::ActionsContextMenu);


        //默认右键菜单 第一种方法
        fileTree();

    /*
        pScroll->setWidget(widget);
        dock->setWidget(pScroll);       //将dock内容设置为scroll
    */


}

QTreeWidget *FileManageWidget::getTreeWidget()
{
    if (!treeWidget)    return NULL;
    return treeWidget;
}

QTextEdit *FileManageWidget::getTextFile()
{
    //if (NULL == textFile) return NULL;
    return this->textFile;
}

void FileManageWidget::addItem(QFileInfo *mfi, QTreeWidgetItem ***item)
{

}

void FileManageWidget::foreachFile(QString Path)
{

    //level 目录的等级
    static int level = 0;

    QDir dir(Path);
    foreach (QFileInfo mfi, dir.entryInfoList())
    {
        //是否为目录
        if (!mfi.isFile())
        {

            if (mfi.fileName() == "." || mfi.fileName() == "..")  //排除. ..目录
                continue;
            static int j = 0;
            dirBtn[j] = new QToolButton;
            dirBtn[j]->setObjectName(mfi.fileName());
            dirBtn[j]->setText(mfi.fileName());
            dirBtn[j]->setIcon(QPixmap(":/icon/icon/dir.png"));
            dirBtn[j]->setIconSize(QPixmap(":/icon/icon/dir.png").size());
            dirBtn[j]->setAutoRaise(true);
            dirBtn[j]->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            dirBtn[j]->setToolTip(mfi.absoluteFilePath());

            verticalLayout->addWidget(dirBtn[j]);
            //qDebug() << "entry dir " << mfi.absoluteFilePath();   //绝对路径
            //foreachFile(mfi.absoluteFilePath());
            j++;
        }
        else
        {
            static int i = 0;
            //qDebug() << "p :" << &fileBtn[i];


            fileBtn[i] = new QToolButton;
            fileBtn[i]->setObjectName(mfi.fileName());
            fileBtn[i]->setText(mfi.fileName());
            fileBtn[i]->setIcon(QPixmap(":/icon/icon/file.png"));
            fileBtn[i]->setIconSize(QPixmap(":/icon/icon/file.png").size());
            fileBtn[i]->setAutoRaise(true);
            fileBtn[i]->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            fileBtn[i]->setToolTip(mfi.absoluteFilePath());
            verticalLayout->addWidget(fileBtn[i]);

            //dock->setWidget(fileBtn[i]);
            //childWidget[i] = new MainWindow(fileBtn[i]->icon(), i, fileBtn[i]->text(), this);
            //qDebug() << "File :" << mfi.fileName();
            qDebug() << "总文件数 i :" << i;
            //i++;        //文件数

        }
    }
    level++;            //目录等级自增
}

void FileManageWidget::addChild(QString dirPath, QTreeWidgetItem *item)
{

    QDir dir(dirPath);
    if (!dir.exists())          //判断路径是否存在
        return;
    dir.setFilter(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs | QDir::NoSymLinks);  //setfilter过滤规则 类型过滤
    dir.setSorting(QDir::DirsFirst | QDir::Name | QDir::IgnoreCase);      //按目录第一， 名称， 不区分大小写排序
    QTreeWidgetItem *childItem[496];                    //子节点的数组
    int i = 0;
    foreach (QFileInfo mfi, dir.entryInfoList())
    {
        childItem[i] = new QTreeWidgetItem(QStringList()<<mfi.fileName());
        childItem[i]->setToolTip(0, mfi.absoluteFilePath());
        //设置item双击可编辑
        //childItem[i]->setFlags(childItem[i]->flags() | Qt::ItemIsEditable);
        item->addChild(childItem[i]);
        if (mfi.isDir())        //此处可以详细判断是什么类型的文件
        {
            childItem[i]->setIcon(0, QIcon(":/icon/icon/dir.png"));
            this->addChild(mfi.absoluteFilePath(),childItem[i]);          //递归调用
            //childItem[i]->setText(2, "0");
        }
        else
        {
            childItem[i]->setIcon(0, QIcon(":/icon/icon/file.png"));
            //childItem[i]->setText(2, "1");
        }
        childItem[i]->setText(1, mfi.absoluteFilePath());   //这里通过设置item的列字符串传递文件的绝对路径
        childItem[i]->setText(2, "1");
        i++;
    }

    /*
    if (mfi.isDir()) // && dir.count() != 0
    {
        if (mfi.fileName() == "." || mfi.fileName() == "..")  //排除. ..目录
            continue;
        //如果是目录，additem
        //递归的终止条件是文件
        //item[i]->addChild();
        item[i]->setIcon(0, QIcon(":/icon/dir.png"));
        QDir childDir(mfi.absoluteFilePath());
        int childCount = 0;
        QTreeWidgetItem *childItem[childDir.entryList().size()];
        foreach (QFileInfo childMfi, childDir.entryInfoList())
        {
            childItem[childCount] = new QTreeWidgetItem(QStringList()<<childMfi.fileName());
            item[i]->addChild(childItem[childCount]);
            childCount++;
        }
        //fileTree(mfi.absoluteFilePath());
    }
    else
    {

        QStringList itemList;       //item字符串
        QTreeWidgetItem *child;     //子item
        QString key, value;
        key = mfi.fileName();
        value = QString::number(0);
        itemList<<key<<value;
        child = new QTreeWidgetItem(itemList);
        item[i]->addChild(child);
        item[i]->setIcon(0, QIcon(":/icon/file.png"));
        qDebug() << "isfile:" << mfi.absoluteFilePath();
    }
    */
}

void FileManageWidget::viewFileTree(QString Path)
{

}

/*
 *      双击显示该文件夹下的所有文件
 *
 */
void FileManageWidget::groupFile(QString Path)
{
    //1、遍历文件
    foreachFile(Path);
    //2、将文件放入toolbutton中
    //addToolButton();
}

void FileManageWidget::setActiveDock1(bool)
{

}

void FileManageWidget::setActiveDock2(bool)
{

}

void FileManageWidget::setActiveDock3(bool)
{

}

/*
 *
 * 使用Qt::CustomContextMenu方式实现右键菜单
 * 发送custonContextMenuRequested信号
 * 首先得设置treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
 */
void FileManageWidget::popMenuSlot(const QPoint &)
{
    if (!treeWidget) return;
    QTreeWidgetItem* currentItem = treeWidget->currentItem();
    if (currentItem == NULL) return;
    if (currentItem->text(2).toInt())   //如果为文件
    {
        //QAction deleteWell(QString::fromStdWString(L"&删除"), this);
        QAction openWell(QString::fromLocal8Bit("打开文件"), this);
        QAction deleteWell(QString::fromLocal8Bit("删除"), this);
        QAction renameWell(QString::fromLocal8Bit("重命名"), this);
        QAction unfoldWell(QString::fromLocal8Bit("展开全部"), this);
        QAction replicateWell(QString::fromLocal8Bit("折叠全部"), this);
        //在界面上删除item
        connect(&deleteWell, &QAction::triggered, this, &FileManageWidget::fileDelSlot);
        connect(&renameWell, &QAction::triggered, this, &FileManageWidget::fileRename);
        connect(&openWell, &QAction::triggered, this, &FileManageWidget::fileReadSlot);
        connect(&unfoldWell, &QAction::triggered, this, &FileManageWidget::unfoldSlot);
        connect(&replicateWell, &QAction::triggered, this, &FileManageWidget::replicateSlot);
        QMenu menu(treeWidget);
        menu.addAction(&openWell);
        menu.addAction(&renameWell);
        menu.addSeparator();
        menu.addAction(&deleteWell);
        menu.addAction(&replicateWell);
        menu.addAction(&unfoldWell);
        menu.exec(QCursor::pos());      //当前鼠标位置显示

    }
}

void FileManageWidget::fileReadSlot(bool)
{
    QTreeWidgetItem *item = treeWidget->currentItem();
    //设置当前读取的文件
    m_strCurrFile = ( item->text(1).isEmpty() ? item->text(1) : "" );

    QIcon icon1(":/image/1.ico");
//    centertabWidget = new QTabWidget;
//    centertabWidget->addTab(textFile, icon1, "Tab1");
    textFile->setFontPointSize(15);     //设置字体大小
    //textFile->setTextBackgroundColor("red");        //设置字体的背景颜色
    QColor color(84,115,131);
    textFile->setTextColor(color);
    if (item == NULL)   return;
    if (item->text(2).toInt())  //判断是否为文件
    {
        QFile file(item->text(1));
        if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) //以读写文本方式打开文件
        {
            qDebug() << "Can't open the file" << endl;
        }
        //判断文件是否可写
        if (!file.isWritable())
        {
            textFile->setReadOnly(1);       //设置为只读
        }
        textFile->clear();      //清空文本
        //读取编码
//        QTextCodec *codec = QTextCodec::codecForLocale();
//        QTextCodec::setCodecForLocale(codec);
        while (!file.atEnd())
        {
            char buf[1024];
            memset(buf, 0, sizeof(buf));
            qint64 lineLength = file.readLine(buf, sizeof(buf));
            if (lineLength != -1)
            {
                //先读取codecForName(”GBK“), ANSI的简体中文编码转Unicode
                //QString str = codec->toUnicode(buf);
                QString str(buf);
                QTextCursor text = textFile->textCursor();
                text.insertText(str);
                //qDebug() << str;
            }


        }
        file.seek(0);   //将文件指针指向文件头
        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            QString str(line);          //codecForName不设置，这个方式utf8,ANSI, unicode, unicode big endian都可以显示正常
            //qDebug() << str;
        }
    }
}

void FileManageWidget::saveModifySlot(bool)
{
    QTreeWidgetItem *item = this->treeWidget->currentItem();
    QFile file(item->text(1));
    QFileInfo dir(item->text(1));
    file.rename(dir.absolutePath() + "/" + item->text(0));
    item->setText(1, dir.absolutePath() + "/" + item->text(0));
    item->setToolTip(0, dir.absoluteFilePath());
}

void FileManageWidget::fileDelSlot(bool)
{
    QMessageBox::StandardButton button = QMessageBox::question(this, "提示", "确定要删除该文件？",
                    QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (button == QMessageBox::Yes)
    {
        if (!this->treeWidget) return;
        QTreeWidgetItem* item = treeWidget->currentItem();
        QFile file(item->text(1));
        file.remove();
        //删除item
        treeWidget->removeItemWidget(item, treeWidget->currentColumn());
        delete item;
    }
}

void FileManageWidget::fileRename(bool)
{
    QTreeWidgetItem *item = treeWidget->currentItem();
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    treeWidget->editItem(item, 0);
}

void FileManageWidget::unfoldSlot(bool)
{
    if (!this->treeWidget) return;
    treeWidget->expandAll();        //->expandToDepth(int depth);将所有可扩展项目扩展到给定的深度
}

void FileManageWidget::replicateSlot(bool)
{
    if (this->treeWidget == NULL) return;
    this->treeWidget->collapseAll();
}
