#ifndef FILEMANAGEWIDGET_H
#define FILEMANAGEWIDGET_H

#define FILEBTNCOUTN 3
#define FILECOUNT 8182
#include <QWidget>
#include <QTreeWidget>
#include <QDockWidget>
#include <QTextEdit>
#include <QListView>
#include <QToolButton>
#include <QFileSystemModel>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QPoint>

class FileManageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FileManageWidget(QString filePath, QWidget *parent = nullptr);
    ~FileManageWidget();
    void init(QString FilePath);

public:
    QTextEdit *textEditCenterWidget;                    //中心文本编辑控件
    int i;
    int flag;
    //QDockWidget *dock;

public:
    void fileTree();
    void scroll();
    QTreeWidget *getTreeWidget();
    QTextEdit *getTextFile();
    void addItem(QFileInfo *mfi, QTreeWidgetItem ***item);

private:
    QString m_strCurrFile;                     //当前读取的文件
    QTextEdit *textFile;
    QTabWidget *tabWidget;      //maincenterWidget
    QTabWidget *centertabWidget;

    QTreeWidget *treeWidget;                            //文件树控件
    QString FilePath;
    QFileSystemModel dirModel;
    QTreeView resTree;
    QListView currListView;
    QToolButton *fileBtn[FILECOUNT];
    QToolButton *dirBtn[FILECOUNT];
    QScrollArea *pScroll;
    QWidget *dockWidgetContents;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    void setActiveDock1(bool);
    void setActiveDock2(bool);
    void setActiveDock3(bool);


    //MainWindow *childWidget[FILEBTNCOUTN];

private:
    void foreachFile(QString Path);
    void addChild(QString dirPath, QTreeWidgetItem *item);
    void viewFileTree(QString Path);
    void groupFile(QString Path);
protected:
    //void QAbstractItemView::closeEditor(QWidget *editor, QAbstractItemDelegate::EndEditHint hint);
    void closeEditor(QWidget *editor, QAbstractItemDelegate::EndEditHint hint);

signals:

public slots:

    void popMenuSlot(const QPoint&);                               //上下文菜单
    void fileReadSlot(bool);   //双击打开文件
    void saveModifySlot(bool); //保存文件重命名的修改
    void fileDelSlot(bool);
    void fileRename(bool);
    void unfoldSlot(bool);
    void replicateSlot(bool);


};

#endif // FILEMANAGEWIDGET_H
