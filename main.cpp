#include "mainwindow.h"
#include "opencv2/opencv.hpp"
#include <QApplication>
#include <QSplashScreen>
#include <QScrollArea>
#include <QScrollBar>
#include <QClipboard>
using namespace cv;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
/*
 *  OpenCV test OK!
 *
    Mat picture = imread("C:/Users/MISAKI/Documents/QTWorkspace/GOSICK/test.jpg");

    imshow("MyTest", picture);
    waitKey(20180224);
*/
    //启动页面   加载。。。。时需要一些开场动画过渡
    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap(QPixmap("C:/Users/MISAKI/Documents/QTWorkspace/GOSICK/icon/1.jpg")));
    splash->show();
    Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;
    splash->showMessage(QObject::tr("Setting up the main mainwindow..."), topRight, Qt::red);

    MainWindow w;
    //此属性保存应用程序在关闭最后一个窗口时是否隐式退出。默认true
    //a.setQuitOnLastWindowClosed(false);
    //设置窗口大小
    w.setFixedSize(QSize(1200, 800));

    w.show();
    //设置背景为透明，需要和setWindowFlags(Qt::FramelessWindowHint| windowFlags())一起使用
    //w.setAttribute(Qt::WA_TranslucentBackground);

    //没指定父对象，完成时需手动delete
    splash->finish(&w);
    delete splash;

    //剪切板  平台相关
    //Returns the object for interacting with the clipboard.
    QClipboard *clipboard =  QApplication::clipboard();
    //Copies text into the clipboard as plain text.
    //void QClipboard::setText(const QString &text, QClipboard::Mode mode = Clipboard);
    //QClipboard::Clipboard     indicates that data should be stored and retrieved from the global clipboard.
    //QClipboard::Selection     indicates that data should be stored and retrieved from the global mouse selection.
    //Support for Selection is provided only on systems with a global mouse selection (e.g. X11).
    //indicates that data should be stored and retrieved from the Find buffer. This mode is used for holding search strings on macOS.
    clipboard->setText("sdd");

    //Returns a list of paths that the application will search when dynamically loading libraries.
    //the default installation directory for plugins is INSTALL/plugins, where INSTALL is the directory where Qt was installed
    QStringList strList =  QCoreApplication::libraryPaths();

    /**
      QScrollArea 滚动区域
      QScrollArea继承了QAbstractScrollArea，QTextEdit和QAbstractItemView，QMidArea也是继承了它，这些类不需要使用QScrollArea
      */
    QScrollArea area;
    //area.setWidget(widget);
    //void QWidget::setBackgroundRole(QPalette::ColorRole role)
    //area.viewport()->setBackgroundRole(QPalette::Dark); //area.viewport()获取显示区
    //area.show();    //[slot] void QWidget::show()
    //area.horizontalScrollBar()->setStyleSheet(str);     //设置滚动条的样式

    //Qt的类型转换
    /*Returns the given object cast to type T if the object is of type T (or of a subclass);
     *The class T must inherit (directly or indirectly) QObject and be declared with the Q_OBJECT macro.
    */
    //QObject *object = new QTimer;
    //qobject_cast<QTimer*>(object);
    return a.exec();
}
