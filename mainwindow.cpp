#include "mainwindow.h"
#include "dbconnectdlg.h"
#include "conversiondlg.h"
#include "stackewidgetddlg.h"
#include "filemanagewidget.h"
#include "filetransfer.h"
#include "customwidget.h"
#include "gscanplugin.h"
#include "plottertest.h"
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QMessageBox>
#include <QCloseEvent>
#include <QStringList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QDebug>
#include <QtUiTools/QUiLoader>                  //QUiLoader loader;
#include <QSqlError>
#include <QSqlRecord>
#include <QFileDialog>
#include <QStringListIterator>
#include <QSettings>
#include <QMdiSubWindow>
#include <QTcpSocket>
#include <QSplitter>

/**C++调用C函数
 *   两种方式：
 *  ①直接包含x.c文件
 *  ②包含x.h的函数声明文件   这样会找不到对应xx.cpp文件
 *     因为默认是C++头文件
 *     所以需要使用       extern "C"{}
 */
//①
//#include "File.c"

//②
//extern "C" 声明为C头文件，对应.c文件为不是.c++
extern "C"
{
    #include "File.h"
}
//VS引入库的方式
#pragma comment(lib, "user32.lib")
//#import "C:\Users\MISAKI\Documents\QTWorkspace\GOSICK\extern\zlib\prebuilt\zlib1.dll"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //设置窗口标题
    this->setWindowTitle("GOSICK");

    //隐藏边框
    //this->setWindowFlags(Qt::FramelessWindowHint | windowFlags());

    //设置图标
    this->setWindowIcon(QIcon(":/icon/icon/recycle.png"));

    m_mdiArea = NULL;

    //先创建动作再创建菜单栏
    createAction();
    createMenu();

//    const char *c = {"ss"};
//    const char ch[1024] = "dasdf";

    //this->readSettings();               //读取设置
}

MainWindow::~MainWindow()
{
    if (db.isOpen())
        m_sql.disConnectSql(&db);
}

void MainWindow::init()
{

}

void MainWindow::createAction()
{

    //管理
    m_loginAction = new QAction(QString::fromLocal8Bit("登录"), this);
    m_loginAction->setShortcut(tr("Ctrl+L"));
    m_loginAction->setStatusTip(tr("Login"));
    m_loginAction->setIcon(QIcon(":/icon/icon/sliderThumb.png"));  //设置小图标
    connect(m_loginAction, &QAction::triggered, this, &MainWindow::on_login);


    //QToolBar  工具栏
    QToolBar *toolBar = addToolBar(tr("&File"));//创建工具栏，添加File工具
    if (NULL != m_loginAction) toolBar->addAction(m_loginAction);

    //状态栏
    QStatusBar *statusbar = statusBar();//创建状态栏，在窗口下面添加显示动作对象的提示信息
    if (NULL != m_loginAction) statusbar->addAction(m_loginAction);

    m_logoutAction = new QAction(QString::fromLocal8Bit("注销"), this);
    m_logoutAction->setShortcut(tr("Ctrl+Q"));
    m_logoutAction->setStatusTip(tr("Log out"));
    m_logoutAction->setEnabled(false);        //设置行为不可用
    connect(m_logoutAction, &QAction::triggered, this, &MainWindow::on_logout);


    m_fileAction = new QAction(QString::fromLocal8Bit("文件管理"), this);
    m_fileAction->setShortcut(tr("Ctrl+Q"));
    m_fileAction->setStatusTip(tr("Create File Management"));
    m_fileAction->setIcon(QIcon(":/icon/file1.jpg"));
    m_logoutAction->setEnabled(false);        //设置行为不可用
    connect(m_fileAction, &QAction::triggered, this, &MainWindow::on_fileManage);

    m_exitAction = new QAction(QString::fromLocal8Bit("退出"), this);
    m_exitAction->setShortcut(tr("Ctrl+K"));
    m_exitAction->setStatusTip(tr("Exit"));
    connect(m_exitAction, &QAction::triggered, this, &MainWindow::on_exit);

    m_newFileAction = new QAction(QString::fromLocal8Bit("新建"), this);
    m_newFileAction->setShortcut(tr("Ctrl+N"));
    m_newFileAction->setStatusTip(tr("Create File & Open"));
    m_newFileAction->setIcon(QIcon(":/icon/new.jpg"));
    connect(m_newFileAction, &QAction::triggered, this, &MainWindow::on_newFile);
    m_openFileAction = new QAction(QString::fromLocal8Bit("打开"), this);
    m_openFileAction->setShortcut(QKeySequence::Open);
    m_openFileAction->setStatusTip(tr("Open File"));
    connect(m_openFileAction, &QAction::triggered, this, &MainWindow::on_openFile);
    m_fileSaveAction = new QAction(QString::fromLocal8Bit("保存"), this);
    m_fileSaveAction->setShortcut(tr("Ctrl+N"));
    m_fileSaveAction->setStatusTip(tr("Save Moditfied File"));
    m_fileSaveAction->setIcon(QIcon(":/icon/save.jpg"));
    connect(m_fileSaveAction, &QAction::triggered, this, &MainWindow::on_saveFile);
    m_fileSaveAllAction = new QAction(QString::fromLocal8Bit("保存全部"), this);
    m_fileSaveAllAction->setShortcut(tr("Ctrl+N"));
    m_fileSaveAllAction->setStatusTip(tr("Save All File"));
    connect(m_fileSaveAllAction, &QAction::triggered, this, &MainWindow::on_saveAllFile);
    m_FindTextAction = new QAction(QString::fromLocal8Bit("查找"), this);
    m_FindTextAction->setShortcut(tr("Ctrl+F"));
    m_FindTextAction->setStatusTip(tr("Find Text"));
    m_FindTextAction->setEnabled(false);
    connect(m_FindTextAction, &QAction::triggered, this, &MainWindow::on_findText);

    m_undoAction = new QAction(QString::fromLocal8Bit("撤消"), this);
    m_undoAction->setShortcut(tr("Ctrl+N"));
    m_undoAction->setStatusTip(tr(""));
    m_undoAction->setEnabled(false);
    m_undoAction->setIcon(QIcon(":/icon/undo.jpg"));
    connect(m_undoAction, &QAction::triggered, this, &MainWindow::on_undo);
    m_copyAction = new QAction(QString::fromLocal8Bit("复制"), this);
    m_copyAction->setShortcut(tr("Ctrl+N"));
    m_copyAction->setIcon(QIcon(":/icon/copy.jpg"));
    m_copyAction->setEnabled(false);
    connect(m_copyAction, &QAction::triggered, this, &MainWindow::on_copy);
    m_pasteAction = new QAction(QString::fromLocal8Bit("粘贴"), this);
    m_pasteAction->setShortcut(tr("Ctrl+N"));
    m_pasteAction->setEnabled(false);
    connect(m_pasteAction, &QAction::triggered, this, &MainWindow::on_paste);

    m_addCellAction = new QAction(QString::fromLocal8Bit("添加基站"), this);
    m_addCellAction->setShortcut(tr("Ctrl+N"));
    m_addCellAction->setEnabled(false);        //设置行为不可用
    connect(m_addCellAction, &QAction::triggered, this, &MainWindow::on_addCell);

    m_selectCellAction = new QAction(QString::fromLocal8Bit("查询基站"), this);
    m_selectCellAction->setShortcut(tr("Ctrl+N"));
    m_selectCellAction->setEnabled(false);        //设置行为不可用
    connect(m_selectCellAction, &QAction::triggered, this, &MainWindow::on_selectCell);

    m_scriptAction = new QAction(QString::fromLocal8Bit("执行脚本"), this);
    m_scriptAction->setShortcut(tr("Ctrl+N"));
    m_scriptAction->setEnabled(false);        //设置行为不可用
    connect(m_scriptAction, &QAction::triggered, this, &MainWindow::on_script);
    m_isEditTriggersAction = new QAction(QString::fromLocal8Bit("允许编辑"), this);
    m_isEditTriggersAction->setEnabled(false);
    m_isEditTriggersAction->setCheckable(true);
    m_isEditTriggersAction->setChecked(false);
    connect(m_isEditTriggersAction, &QAction::triggered, this, &MainWindow::on_isEdit);
    m_commitAction = new QAction(QString::fromLocal8Bit("提交修改"), this);
    m_commitAction->setEnabled(false);
    connect(m_commitAction, &QAction::triggered, this, &MainWindow::on_commit);
    m_addRowAction = new QAction(QString::fromLocal8Bit("添加空行"), this);
    m_addRowAction->setEnabled(false);
    connect(m_addRowAction, &QAction::triggered, this, &MainWindow::on_addNullRow);
    m_delSeleAction = new QAction(QString::fromLocal8Bit("删除选中的行"), this);
    m_delSeleAction->setEnabled(false);
    connect(m_delSeleAction, &QAction::triggered, this, &MainWindow::on_delSeleRow);



    m_cascadeAction = new QAction(QString::fromLocal8Bit("层叠"), this);
    m_cascadeAction->setShortcut(tr("Ctrl+Y"));
    m_cascadeAction->setEnabled(false);
    connect(m_cascadeAction, &QAction::triggered, this, &MainWindow::on_cascadeWindows);

    m_tileAction = new QAction(QString::fromLocal8Bit("并列"), this);
    m_tileAction->setShortcut(tr("C"));
    m_tileAction->setEnabled(false);
    connect(m_tileAction, &QAction::triggered, this, &MainWindow::on_tileWindows);
    m_closeSubWinAction = new QAction(QString::fromLocal8Bit("关闭当前窗口"), this);
    m_closeSubWinAction->setEnabled(false);
    connect(m_closeSubWinAction, &QAction::triggered, this, &MainWindow::on_closeActiveWindow);
    m_closeAllAction = new QAction(QString::fromLocal8Bit("关闭全部窗口"), this);
    m_closeAllAction->setShortcut(tr("P"));
    m_closeAllAction->setEnabled(false);
    connect(m_closeAllAction, &QAction::triggered, this, &MainWindow::on_closeAllWindows);


    m_toolAction = new QAction(QString::fromLocal8Bit("工具"), this);
    m_toolAction->setShortcut(tr("I"));
    m_appAction = new QAction(tr("APP"),this);
    m_appAction->setShortcut(tr("K"));
    connect(m_appAction, &QAction::triggered, this, &MainWindow::on_toolAppShow);
    m_conversionAction = new QAction(QString::fromLocal8Bit("单位换算"), this);
    m_conversionAction->setShortcut(tr("ctrl+T"));
    connect(m_conversionAction, &QAction::triggered, this, &MainWindow::on_toolConversion);

    m_designAction = new QAction(QString::fromLocal8Bit("设计"), this);
    m_designAction->setShortcut(tr("J"));
    m_stackeAction = new QAction(QString::fromLocal8Bit("QDockWidget-堆栈窗口"), this);
    m_stackeAction->setShortcut(tr("H"));
    connect(m_stackeAction, &QAction::triggered, this, &MainWindow::on_designStack);



    m_CplusplusAction = new QAction(tr("C++"), this);
    m_CplusplusAction->setShortcut(tr("Ctrl+H"));
    connect(m_CplusplusAction, &QAction::triggered, this, &MainWindow::on_cpp);
    m_CAction = new QAction(tr("C"), this);
    m_CAction->setShortcut(tr("Ctrl+H"));
    connect(m_CAction, &QAction::triggered, this, &MainWindow::on_c);

    m_isCheckedRecentFileAction = new QAction(tr("recentFile"), this);
    m_isCheckedRecentFileAction->setCheckable(true);        //设置动作是否可选  √
    m_isCheckedRecentFileAction->setChecked(false);         //设置动作为选中

    m_abortAction = new QAction(QString::fromLocal8Bit("关于"), this);
    m_abortAction->setShortcut(tr("Ctrl+B"));
    connect(m_abortAction, &QAction::triggered, this, &MainWindow::on_abort);
    m_helpAction = new QAction(QString::fromLocal8Bit("帮助"), this);
    m_helpAction->setShortcut(tr("Ctrl+H"));
    connect(m_helpAction, &QAction::triggered, this, &MainWindow::on_help);
}

void MainWindow::createMenu()
{
    //管理
    //this->menuBar();        //对于QT来讲，一个mian窗口只有一个menuBar
    //adminMenu是个菜单返回的指针
    m_adminMenu = menuBar()->addMenu(QString::fromLocal8Bit("管理"));
    m_adminMenu->addAction(m_loginAction);           //使用adminMenu菜单指针
    m_adminMenu->addAction(m_logoutAction);
    m_adminMenu->addSeparator();                  //添加分割符
    m_adminMenu->addAction(m_fileAction);
    m_adminMenu->addSeparator();
    m_adminMenu->addAction(m_exitAction);

    //文件
    m_fileMenu = menuBar()->addMenu(QString::fromLocal8Bit("文件"));
    m_fileMenu->addAction(m_newFileAction);
    m_fileMenu->addAction(m_openFileAction);
    m_fileMenu->addAction(m_fileSaveAction);
    m_fileMenu->addAction(m_fileSaveAllAction);
    m_fileMenu->addSeparator();                  //添加分割符
    //右击会显示m_fileMenu的所有动作 QWidget::actions()
    //m_fileMenu->setContextMenuPolicy(Qt::ActionsContextMenu);

    //编辑
    m_editMenu = menuBar()->addMenu(QString::fromLocal8Bit("编辑"));
    m_editMenu->addAction(m_undoAction);
    m_editMenu->addAction(m_copyAction);
    m_editMenu->addAction(m_pasteAction);
    m_editMenu->addAction(m_FindTextAction);

    //数据
    m_dataMenu = menuBar()->addMenu(QString::fromLocal8Bit("数据"));
    m_dataMenu->addAction(m_addCellAction);
    m_dataMenu->addAction(m_selectCellAction);
    m_dataMenu->addSeparator();                  //添加分割符
    m_dataMenu->addAction(m_scriptAction);
    m_dataMenu->addSeparator();
    m_dataMenu->addAction(m_commitAction);
    m_dataMenu->addAction(m_isEditTriggersAction);
    m_dataMenu->addAction(m_addRowAction);
    m_dataMenu->addAction(m_delSeleAction);

    //窗口
    m_windonMenu = menuBar()->addMenu(QString::fromLocal8Bit("窗口"));
    m_windonMenu->addAction(m_cascadeAction);
    m_windonMenu->addAction(m_tileAction);
    m_windonMenu->addSeparator();                  //添加分割符
    m_windonMenu->addAction(m_closeSubWinAction);
    m_windonMenu->addAction(m_closeAllAction);

    //开发
    m_develMenu = menuBar()->addMenu(QString::fromLocal8Bit("devp"));
    m_develMenu->addAction(m_toolAction);
    QMenu *toolMenu = new QMenu(this);
    //toolMenu->addAction(toolAction);
    toolMenu->addAction(m_appAction);
    toolMenu->addAction(m_conversionAction);
    m_toolAction->setMenu(toolMenu);
    m_develMenu->addAction(m_designAction);
    QMenu *designMenu = new QMenu(this);
    designMenu->addAction(m_stackeAction);
    m_designAction->setMenu(designMenu);

    //课程
    m_lessonMenu = menuBar()->addMenu(QString::fromLocal8Bit("课程"));
    m_lessonMenu->addAction(m_CplusplusAction);
    m_lessonMenu->addSeparator();
    m_lessonMenu->addAction(m_CAction);

    //工具
    m_toolMenu = menuBar()->addMenu(QString::fromLocal8Bit("工具"));
    m_toolMenu->addAction(m_isCheckedRecentFileAction);

    //帮助
    m_helpMenu = menuBar()->addMenu(QString::fromLocal8Bit("帮助"));
    m_helpMenu->addAction(m_helpAction);
    m_helpMenu->addSeparator();
    m_helpMenu->addAction(m_abortAction);


}

void MainWindow::showSqlViewModel(QString tb, QStringList *filters)
{
    if (m_mdiArea == NULL)
    {
        qDebug() << "m_mdiArea == NULL :"<< __FILE__ <<": Line:" << __LINE__;
        return;
    }
    //1.
    //QStandardItemModel *model = new QStandardItemModel(5, 4);
    m_sqlModel = new QSqlTableModel(this, db);
    m_sqlModel->setTable(tb);
    m_sqlModel->select();
    m_sqlModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    //设置查询条件
    int i = 0;
    for (i; i < filters->count(); i++)
    {
        m_sqlModel->setFilter(filters->at(i));
    }
    //sqlModel->setData(1,1);       //可能对一些缓存有用，例如提交失败，还可以从数据中获取数据
    m_sqlView = new QTableView;     //没有父对象，必须手动销毁 setAttribute(Qt::WA_DeleteOnClose)
    m_sqlView->setAttribute(Qt::WA_DeleteOnClose);    //关闭时自动销毁
    m_sqlView->setModel(m_sqlModel);
    //qDebug() << sqlModel->lastError();

/*
    //2.设置字段名
    model->setHeaderData(0, Qt::Horizontal, "姓名");
    model->setHeaderData(1, Qt::Horizontal, "sex");
    model->setHeaderData(2, Qt::Horizontal, "serv");
    model->setHeaderData(3, Qt::Horizontal, "db");

    //3.设置单元格值
    model->setData(model->index(0, 0, QModelIndex()), "张三");
    model->setData(model->index(0, 1, QModelIndex()), "male");
    model->setData(model->index(0, 2, QModelIndex()), "123.123.133.11");
    model->setData(model->index(0, 3, QModelIndex()), "san_db");

    model->setData(model->index(1, 0, QModelIndex()), "乙女");
    model->setData(model->index(2, 0, QModelIndex()), "misaki");
    model->setData(model->index(3, 0, QModelIndex()), "misaki");
    model->setData(model->index(4, 0, QModelIndex()), "xiao");

    //3.
    m_sqlView = new QTableView;
    m_sqlView->setAttribute(Qt::WA_DeleteOnClose);      //关闭时自动销毁，如果view有model，model会
    m_mdiArea->addSubWindow(m_sqlView);          //把它作为子窗口

    m_sqlView->setStyleSheet("border-image: url('5.png'')");

    //4.
    //之前把要设置的属性设置好
    m_sqlView->setModel(model);     //数据来自model如果没有设置view不会显示，view1继承自widget
    m_sqlView->show();
*/
    //m_mdiArea->activeSubWindow();         //获取当前活动的窗口
    //m_mdiArea->activeSubWindow()->resize(width() -100, height() - 100);
    QMdiSubWindow *subWindow = m_mdiArea->addSubWindow(m_sqlView, Qt::WindowFlags());
    subWindow->setWindowTitle(tb);
    subWindow->setAutoFillBackground(true);
    m_sqlView->show();
}

void MainWindow::updateRecentFiles()
{
//    QStringListIterator ite(m_strListRecentFiles);
//    while (true == ite.hasNext())
//    {
//        if(false == QFile::exists(ite.next()))
//        {
//            ite.remove();
//        }
//    }

//    int i = 0;
//    for (i; i < ARRSUB_MAXFILECOUNT; ++i)
//    {
//        if (i < m_strListRecentFiles.count())
//        {
//            QString text = tr("&%1 %2").arg(i + 1).arg(this->strippedName(m_strListRecentFiles[i]));
//            m_recentFileAction[i]->setText(text);
//            //存储文件的全名
//            m_recentFileAction[i]->setData(m_strListRecentFiles[i]);
//            m_recentFileAction[i]->setVisible(true);
//        }
//        else
//        {
//            m_recentFileAction[i]->setVisible(false);
//        }
//    }

}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

/**
 * @brief MainWindow::writeSettings
 *
 * QString 根据平台特性存储应用程序的设置，Windows中使用注册表，
 * Uinx中存储在文件中，在MaxOS上使用Core Foundation Preference API
 * 支持的数据类型：QString bool double QStringList或者是其他QVariant支持的类型
 */

void MainWindow::writeSettings()
{
    QSettings settings(tr("Software Inc."), tr("GOSICK"));
    settings.beginGroup("mainWindow");
    //几何信息（位置和大小）
    settings.setValue("geometry", geometry());
    //最近打开的文件
    settings.setValue("recentFiles", m_strListRecentFiles);
    //是否显示网格
    //settings.setValue("showGrid", showGridAction()->isChecked());
    //是否自动计算属性
    //settings.setValue("autoReclac", autoReclacAction()->isChecked());
    settings.setValue("size", size());
    settings.setValue("state", saveState());
    settings.endGroup();
}

void MainWindow::readSettings()
{
    QSettings settings(tr("Software Inc."), tr("GOSICK"));
    settings.beginGroup("mainWindow");
    //Qt提供了QWidget::setGeomtry()函数作为QWidget::geomtry()的补充，但在X11上由于窗口管理器多样的原因需要
    //使用move()he resize().
    QRect rect = settings.value("geomtry", QRect(200, 200, 400, 400)).toRect();
    this->move(rect.topLeft());
    this->resize(rect.size());
    m_strListRecentFiles = settings.value("recentFiles").toStringList();
    //更新recentFilesAction
    resize(settings.value("size").toSize());
    restoreState(settings.value("state").toByteArray());
    settings.endGroup();

}


void MainWindow::on_login()
{
    //设置多文档形式
    m_mdiArea = new QMdiArea(this);
    this->setCentralWidget(m_mdiArea);    //设置中心控件
    m_mdiArea->setBackground(Qt::NoBrush);

    //以样式形式
    m_mdiArea->setStyleSheet("background-image:url(:/icon/icon/1.jpg);"
                             "background-repeat:no-repeat;"
                             "background-attachment:local;"
                             "width:800;height:800");
    //m_mdiArea->setBackgroundRole(2);

    //当子窗口的范围超过父窗口的显示时，父窗口自动添加滚动条
    m_mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);   //横向
    m_mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);     //纵向
    //midArea->setTabPosition(QTabWidget::North);

   DBConnectDlg dlg;       //栈变量，函数执行玩就不存在了，       //模式就这样定义
                    //所以如果是这种方式定义，在非模式下就会一闪而过地执行
    //LoginDlg *dlg = new LoginDlg;       //实例化对话框      //非模式就这样定义
   //dlg->show();          //非模式

    dlg.exec();         //产生一个模式对话框

    //QMessageBox::information(this, "提示", "onclick_login()已执行");


    if (false == dlg.isCancel)
    {
        statusBar()->showMessage("Connecting...", 2000);
//        int flag = dbCon.connect_sql(dlg.m_host.toStdString().data(),
 //                                       dlg.m_user.toStdString().data(),
//                                     dlg.m_passwd.toStdString().data(),
//                                     dlg.m_dbName.toStdString().data());
        QSqlError sqlErr;
        bool flag = m_sql.connectSql(db, dlg.m_host, dlg.m_dbName,
                                     dlg.m_user, dlg.m_passwd, &sqlErr);
        //qDebug() << db.isOpen();
        if (false == flag)
        {
            statusBar()->showMessage("Connect To Database Failed!", 5000);
            QMessageBox::information(this, QString::fromLocal8Bit("登录失败"),
                                     db.lastError().text(), QMessageBox::Ok);
            DBConnectDlg dlg;
            dlg.exec();
        }
        else
        {
//            QSqlQuery query(db);
//            query.exec("select * from jage_notice;");
//            qDebug() << query.lastError();
            //状态栏显示信息，持续2秒
            statusBar()->showMessage("Connect To Database Successful!", 5000);
            QMessageBox::information(this, "", QString::fromLocal8Bit("登录成功"));
            QStringList listFilter;
            listFilter << "notice_id > 2" << "notice_from_id = '1'";
            this->showSqlViewModel("jage_notice", &listFilter);                 //显示视图
            isLogin = true;

            m_logoutAction->setEnabled(true);         //退出登录
            m_scriptAction->setEnabled(true);         //执行脚本
            m_addCellAction->setEnabled(true);        //添加基站
            m_selectCellAction->setEnabled(true);     //
            m_loginAction->setEnabled(false);         //登录
            m_cascadeAction->setEnabled(true);        //层叠
            m_tileAction->setEnabled(true);           //并列
            m_closeSubWinAction->setEnabled(true);    //关闭当前窗口
            m_closeAllAction->setEnabled(true);       //关闭所有窗口
            m_commitAction->setEnabled(true);         //提交
            m_isEditTriggersAction->setEnabled(true); //是否允许修改
            m_addRowAction->setEnabled(true);         //添加空行
            m_delSeleAction->setEnabled(true);        //删除所选行

            m_copyAction->setEnabled(true);           //复制
            m_pasteAction->setEnabled(true);          //黏贴
            m_FindTextAction->setEnabled(true);       //查找
            m_undoAction->setEnabled(true);           //重做
            m_sqlView->setEditTriggers(QAbstractItemView::NoEditTriggers);
            m_sqlModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
        }


    }


    return;

}

void MainWindow::on_logout()
{
    //通过用户点击的按钮确认是否退出
    QMessageBox::StandardButton button = QMessageBox::question(this, "Tip", "Arre you sure?",
                                QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (button == QMessageBox::Yes)
    {
        m_sql.disConnectSql(&db);
        m_addCellAction->setEnabled(false);
        m_selectCellAction->setEnabled(false);
        m_scriptAction->setEnabled(false);
        m_logoutAction->setEnabled(false);

        isLogin = false;
        m_loginAction->setEnabled(true);

        m_mdiArea->closeAllSubWindows();
        m_commitAction->setEnabled(false);
        m_isEditTriggersAction->setEnabled(false);
        m_addRowAction->setEnabled(false);
        m_delSeleAction->setEnabled(false);
        m_tileAction->setEnabled(false);
        m_cascadeAction->setEnabled(false);
        m_closeSubWinAction->setEnabled(false);
        m_closeAllAction->setEnabled(false);
        m_copyAction->setEnabled(false);
        m_pasteAction->setEnabled(false);
        m_FindTextAction->setEnabled(false);
        m_undoAction->setEnabled(false);
    }

    return;
}

void MainWindow::on_fileManage()
{
    FileTransfer *dlg = new FileTransfer(this);
    //dlg.exec();
    dlg->show();
}

void MainWindow::on_exit()
{
    this->close();
    //exit(0);        //直接退出，不会做任何提示
    return;
}

void MainWindow::on_newFile()
{
//    MyDocked md;

//    md.exec();
//    QFileDialog fileopen;
//    fileopen->
//    return;
}

void MainWindow::on_openFile()
{

    QString fileName = QFileDialog::getOpenFileName(
                this,
                tr("open file dialog"),
                QDir::currentPath(),       //打开的目录
                //多个筛选器用;;隔开
                tr("Images(*.png *.xmp *.jpg *.gif);;Text file(*.txt)")
    );
    if (fileName.isEmpty())
    {
        return;
    }


    //左停靠
//    dock = new QDockWidget();
    //设置全部特性不可用，关闭，move，float    NoDockWidgetFeatures
    //设置                    DockWidgetVerticalTitleBar

//    }


    QString rootPath = QDir::rootPath();        //获取根目录 C:
    QString homePath = QDir::homePath();        //C:/Users/MISAKI
    FileManageWidget *fileWidget = new FileManageWidget("C:/Users/MISAKI/workspace", this);
    QTreeWidget *treeWidget = fileWidget->getTreeWidget();
    if (NULL != treeWidget)
    {
        //this->setCentralWidget(treeWidget);

        //QDockWidget  停靠控件
        m_treeDock = new QDockWidget(QString::fromLocal8Bit("目录"));
        m_treeDock->setWidget(treeWidget);
        m_treeDock->setAllowedAreas(Qt::LeftDockWidgetArea);        //设置控件可停靠的位置
        //1、停靠方式
        //this->addDockWidget(Qt::LeftDockWidgetArea, m_treeDock);
        //this->setCentralWidget(fileWidget->getTextFile());
        //2、QSplitter分割控件方式
        QSplitter *splitter = new QSplitter(Qt::Horizontal);
        splitter->addWidget(treeWidget);
        splitter->addWidget(fileWidget->getTextFile());
        splitter->setStretchFactor(1, 1);
        this->setCentralWidget(splitter);
    }






    //QTextEdit *textMenu = new QTextEdit(tr("左"));
    //dock->setWidget(textMenu);
    //this->add
    //setGeometry(10, 30, 50, 700);     该函数是设置几何形 待研究
    //this->addDockWidget(Qt::LeftDockWidgetArea, dock);      //添加DockWidget
    //tabWidget = new QTabWidget();


    //textFile->setText(tr("主窗口"));
    //textFile->ensureCursorVisible();  //如果需要，通过滚动文本编辑确保光标可见。
    //textFile-> fontUnderline();//setFontUnderline().
    //textFile-> isReadOnly();  此属性保留文本编辑是否为只读。在只读文本编辑中，用户只能浏览文本并选择文本;
    //修改文本是不可能的。此属性的默认值为false。
    //访问功能：bool isReadOnly（）const   void setReadOnly（bool ro）
    //textFile-> isUndoRedoEnabled(); 此属性保存是否启用撤消和重做。
    //bool isUndoRedoEnabled（）const   void setUndoRedoEnabled（bool enable）
    //textFile->mergeCurrentCharFormat();  通过在编辑器的光标上调用QTextCursor :: mergeCharFormat，
    //将修饰符中指定的属性合并为当前字符格式。 如果编辑器有选择，则修饰符的属性将直接应用于选择。
    //textFile->setDocument();
    //textFile->textBackgroundColor((26, 26, 26));
    //textFile->textColor(QColor(255, 255, 255, 255));
//    QTextCursor cursor = textFile->textCursor();
//    for ( int i = 0; i < 1000; i++)
//    {
//        cursor.movePosition(QTextCursor::End);
//        cursor.insertText("i");
//        //textCursor().insertPlainText("i");  paste();  append();   追加文本的槽函数
//    }





    //QAction* pAction = qobject_cast<QAction*>(sender());      //qt的强制转换判断
    //Q_ASSERT(pAction);     //如果pAction为false   ASSERT: "b == 0" in file div.cpp, line 7
    //Q_ASSERT()只在debug时管用
/*//////////////////////////////////  QDockWindow 形成的标签页  start///////////////*/

    //tabifyDockWidget(dock1, dock2);         //设置标签页
    //tabifyDockWidget(dock2, dock3);
    //dock2->raise();


/*//////////////////////////////////  QDockWindow 形成的标签页  end///////////////////*/
    /*//////////////////////////////////  QAction 形成的标签页  start/////////////////*/

        //toggleViewAction()函数返回与QDockWindow关联的QAction，可控制QDockWidget关闭，隐藏，显示

//        tabBar = addToolBar(tr("&File"));//创建工具栏，添加File工具
//        tabBar->addAction(dock1Action);
//        tabBar->addAction(dock2Action);
//        tabBar->addAction(dock3Action);


    /*//////////////////////////////////  QAction 形成的标签页  end//////////////////*/
/*

如果我们需要在下一次运行程序时恢复所有可停靠控件和工具栏的位置， 采用QMainWindow's saveState() 和
restoreState()函数，可以使用与保存QSplitter状态相似的代码：

*/

//    QFileDialog *fd = new QFileDialog();
//    fd->setMode(QFiledialog::AnyFile);
//    fd->setFilter( "Images (*.png *.xpm *.jpg)" );        //addFilter()   增加过滤器
//    fd->setViewMode( QFileDialog::Detail );

//    if (fileName.isEmpty())
//    {
//        return;
//    }
//    QStringList lines;
//    QFile file(fileName);
//    if ( file.open(IO_ReadOnly) )
//    {
//        QTextStream stream(&file);
//        QString line;
//        //int n = 1;
//        while ( !stream.eof() )
//        {
//            line = stream.readLine();       //不包括“\n”的一行文本
//            //
//            lines += line;
//        }

//        fileName.close();
//    }
    //QMessageBox::information(this, "", file.toStdString().data());
        return;
}


/**
 * @brief MainWindow::on_saveFile
 * 如果文件已有名字，或在磁盘上打开的，或已经保存过，就调用
 */
void MainWindow::on_saveFile()
{
//    if (true == currFile.isEmpty())
//    {
//        return on_saveAsFile();
//    }
//    else
//    {
//        return on_saveFile(currFile);
//    }
}

void MainWindow::on_saveAllFile()
{

}

void MainWindow::on_findText()
{

}

void MainWindow::on_undo()
{

}

void MainWindow::on_copy()
{

}

void MainWindow::on_paste()
{
//    const QClipboard *clipboard = QApplication::clipboard();
//          const QMimeData *mimeData = clipboard->mimeData();

//          if (mimeData->hasImage()) {
//              setPixmap(qvariant_cast<QPixmap>(mimeData->imageData()));
//          } else if (mimeData->hasHtml()) {
//              setText(mimeData->html());
//              setTextFormat(Qt::RichText);
//          } else if (mimeData->hasText()) {
//              setText(mimeData->text());
//              setTextFormat(Qt::PlainText);
//          } else {
//              setText(tr("Cannot display data"));
//          }
}

void MainWindow::on_addCell()
{
//    InsertMscdlg dlg;
//    dlg.exec();
//    if (dlg.isCancel)
//    {
//        script_msg(dlg.sql.toStdString().data());
//    }
//    return;
}

void MainWindow::on_selectCell()
{
//    SelectMscDlg dlg;
//    dlg.exec();
//    if (dlg.isCancel)
//    {
//        script_msg(dlg.sql.toStdString().data());
//    }
//    return;
}

void MainWindow::on_script()
{
    //showView();
//    ScriptDlg dlg;
//    dlg.exec();         //阻塞， 模式对话框
//    if (dlg.isCancel)
//    {
//        script_msg(dlg.sql.toStdString().data());

//    }

//    return;
}

void MainWindow::on_cascadeWindows()
{
    if (NULL == m_mdiArea)   return;
    m_mdiArea->cascadeSubWindows();
}

void MainWindow::on_tileWindows()
{
    if (NULL == m_mdiArea)   return;
    m_mdiArea->tileSubWindows();
}

void MainWindow::on_closeActiveWindow()
{
    if (NULL == m_mdiArea)   return;
    QMdiSubWindow *subWindow = m_mdiArea->activeSubWindow();
    if (NULL != subWindow)
    {
        subWindow->close();
    }
}

void MainWindow::on_closeAllWindows()
{
    /*
     *  通过向每个窗口发送QCloseEvent关闭所有子窗口。
     * 您可以在关闭子窗口之前从子窗口接收subWindowActivated()信号
     * (如果MDI区域在另一个子窗口关闭时激活子窗口)。
     * 忽略关闭事件的子窗口将保持打开状态。
     *
    */
    if (NULL == m_mdiArea) return;
    int box = QMessageBox::question(this, QString::fromLocal8Bit("提示"),
                          QString::fromLocal8Bit("是否关闭所有窗口？"),
                          QMessageBox::Yes, QMessageBox::No);
    if (box == QMessageBox::Yes)
    {
        m_mdiArea->closeAllSubWindows();
        m_commitAction->setEnabled(false);
        m_addRowAction->setEnabled(false);
        m_delSeleAction->setEnabled(false);
    }
}

void MainWindow::on_toolAppShow()
{

}

void MainWindow::on_toolConversion()
{
    ConversionDlg dlg;
    dlg.exec();
}

void MainWindow::on_designStack()
{
    stackeWidgetdDlg dlg;
    dlg.exec();
}

void MainWindow::on_help()
{
    QMessageBox::information(this, "Tip", "on_help()");
}

void MainWindow::on_abort()
{
    QMessageBox::about(this, "Tip", "on_abort");
}

void MainWindow::on_cpp()
{
    PlotterTest dlg;
    dlg.resize(600, 430);
    dlg.exec();

}

void MainWindow::on_c()
{

}

void MainWindow::on_openRecentFile()
{

//    //先判断是否存盘
//    if ()
//    {
//        QAction *action = static_cast<QAction*>(sender());
//        if (true == action)
//        {
//            on_openFile(action->data().toString());
//        }
    //    }
}

void MainWindow::on_commit()
{
    QMessageBox::StandardButton question = QMessageBox::question(this,
                "commit", "Do you want to commit?", QMessageBox::Yes
                | QMessageBox::No, QMessageBox::Yes);
    bool flag = 0;
    if (question == QMessageBox::Yes)
    {
        flag = m_sqlModel->submitAll();
    }
    else
    {
        m_sqlModel->revertAll();
        flag = m_sqlModel->submitAll();
    }

    if (false == flag)
    {
        QMessageBox::warning(this, "ERROR", m_sqlModel->lastError().text(),
                             QMessageBox::Ok, QMessageBox::Close);
    }
}

void MainWindow::on_isEdit()
{
    if (true == m_isEditTriggersAction->isChecked())
    {
        m_sqlModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
        m_sqlView->setEditTriggers(QAbstractItemView::DoubleClicked);
        m_addRowAction->setEnabled(true);
        m_delSeleAction->setEnabled(true);
    }
    else
    {
        m_sqlView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        m_addRowAction->setEnabled(false);
        m_delSeleAction->setEnabled(false);
    }
}

void MainWindow::on_addNullRow()
{
    if (true == m_isEditTriggersAction->isChecked())
    {
        //获取一条空行 m_sqlModel->record();
        //获取当前行数
        int rowCount = m_sqlModel->rowCount();
        //在第rowCount插入空行
        m_sqlModel->insertRecord(rowCount, m_sqlModel->record());
    }
}

void MainWindow::on_delSeleRow()
{
    //获取选中的行
    QItemSelectionModel *selModel = m_sqlView->selectionModel();
    QModelIndexList list = selModel->selectedRows();
    int i = 0;
    for (i; i < list.size(); i++)
    {
        m_sqlModel->removeRow(list.at(i).row());    //删除第i个的行号
    }
}
/**
 * @brief MainWindow::closeEvent
 * @param event
 *
 * 当最后一个窗口关闭后应用程序终止。
 */
void MainWindow::closeEvent(QCloseEvent *event)
{
    //通过用户点击的按钮确认是否退出
    QMessageBox::StandardButton button = QMessageBox::question(this, QString::fromLocal8Bit("提示"),
                                    QString::fromLocal8Bit("是否退出？"), QMessageBox::Yes
                                    | QMessageBox::No, QMessageBox::No);
    if (button == QMessageBox::Yes)
    {
        //保存当前的配置
        //this->writeSettings();
        event->accept();
    }
    else
    {
        event->ignore();
    }
    return;
}

void MainWindow::paintEvent(QPaintEvent *event)
{

}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_L:
        if (event->modifiers() & Qt::ControlModifier)
        {
            on_login();
            qDebug() << "ctrl+L";
        }
        else if (event->modifiers() & Qt::AltModifier)
        {
            qDebug() << "Alt+L";
        }
        else
        {
            qDebug() << "L";
        }
        break;
    case Qt::Key_End:
        break;
    default:
        QWidget::keyPressEvent(event);      //调用自身
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{

}
/*
 * Tab键和Backtab（Shift+Tab）键很特殊，他们是在控件调用keyPressEvent之前，
 * 由QWidget::event处理。这两个键的作用是把输入焦点转到前一个或者下一个控件上.
*/
bool MainWindow::event(QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Tab)
        {
            qDebug() << "tab";
        }
    }
    //// pass the event on to the parent class
    return QWidget::event(event);
}
