#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiArea>
#include <QMenu>
#include <QAction>
#include <QTableView>
#include <QSqlTableModel>
#include "mysqlqt.h"
#include "mysqllib.h"
#define ARRSUB_MAXFILECOUNT 5

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();


private:
    void init();                //初始化
    void createAction();        //创建动作
    void createMenu();          //创建菜单栏
    void showSqlViewModel(QString tb, QStringList *filters);    //显示sql可视化操作

private:
    //MySQLLib dbCon;
    MySQLQt m_sql;

public:
    void updateRecentFiles();
    QString strippedName(const QString &fullFileName);
    void writeSettings();       //写入配置
    void readSettings();        //读取配置

public:
    /**
     * @brief 菜单栏
     */
    QMdiArea *m_mdiArea;
    QMenu *m_adminMenu;           //管理菜单栏
    QMenu *m_fileMenu;            //文件菜单栏
    QMenu *m_editMenu;            //编辑菜单栏
    QMenu *m_dataMenu;            //数据菜单栏
    QMenu *m_windonMenu;          //窗口菜单栏
    QMenu *m_develMenu;           //开发菜单栏
    QMenu *m_lessonMenu;          //课程菜单栏
    QMenu *m_toolMenu;            //工具菜单栏
    QMenu *m_helpMenu;            //帮助菜单栏

    /**
     * @brief 菜单栏的动作
     */
    //管理
    QAction *m_loginAction;             //登录
    QAction *m_fileAction;              //文件管理
    QAction *m_logoutAction;            //退出登录
    QAction *m_exitAction;              //退出程序
    //文件
    QAction *m_newFileAction;           //新建文件
    QAction *m_openFileAction;          //打开文件
    QAction *m_fileSaveAction;          //保存当前文件
    QAction *m_fileSaveAllAction;       //保存所有文件
    QAction *m_recentFileAction[ARRSUB_MAXFILECOUNT];    //最近文件
    //编辑
    QAction *m_undoAction;              //撤消
    QAction *m_copyAction;              //复制
    QAction *m_pasteAction;             //黏贴
    //数据
    QAction *m_addCellAction;           //添加基站
    QAction *m_selectCellAction;        //查询基站
    QAction *m_scriptAction;            //执行脚本
    QAction *m_isEditTriggersAction;    //是否可以修改
    QAction *m_commitAction;            //提交修改
    QAction *m_addRowAction;            //添加空行
    QAction *m_delSeleAction;           //删除选中的行
    //帮助
    QAction *m_helpAction;              //帮助
    QAction *m_abortAction;             //关于
    //开发
    QAction *m_toolAction;              //开发工具
    QAction *m_designAction;            //设计
    QAction *m_stackeAction;            //栈
    QAction *m_appAction;               //app
    QAction *m_conversionAction;        //单位换算
    QAction *m_CplusplusAction;         //cpp
    QAction *m_CAction;                 //c
    //窗口
    QAction *m_cascadeAction;           //层叠
    QAction *m_tileAction;              //展开
    QAction *m_closeSubWinAction;       //关闭当前窗口
    QAction *m_closeAllAction;          //关闭所有窗口
    QAction *m_FindTextAction;          //查找
    //工具
    QAction *m_isCheckedRecentFileAction;            //是否显示最近文件

    QStringList m_strListRecentFiles;    //最近文件
    bool isSqlViewModel;                //是否处于sql可视化视图操作
    QTableView *m_sqlView;
    QSqlTableModel *m_sqlModel;
    QSqlDatabase db;


public slots:
    /**
     * @brief 菜单栏动作槽函数
     */
    void on_login();
    void on_logout();
    void on_fileManage();
    void on_exit();
    void on_newFile();
    void on_openFile();
    void on_saveFile();
    void on_saveAllFile();
    void on_findText();
    void on_undo();
    void on_copy();
    void on_paste();
    void on_addCell();
    void on_selectCell();
    void on_script();
    void on_cascadeWindows();
    void on_tileWindows();
    void on_closeActiveWindow();
    void on_closeAllWindows();
    void on_toolAppShow();
    void on_toolConversion();
    void on_designStack();
    void on_help();
    void on_abort();
    void on_cpp();
    void on_c();
private slots:
    void on_openRecentFile();
    void on_commit();
    void on_isEdit();
    void on_addNullRow();
    void on_delSeleRow();

private:
    void closeEvent(QCloseEvent *event);

private:
    bool isLogin;                               //是否已登录
    QDockWidget *m_treeDock;                    //文件树

protected:
    void paintEvent(QPaintEvent *event);        //只要有一个像素点发生改变都会自动调用重绘事件
    //键盘事件
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    //[override virtual protected] bool QWidget::event(QEvent *event);
    bool event(QEvent *event);
};

#endif // MAINWINDOW_H
