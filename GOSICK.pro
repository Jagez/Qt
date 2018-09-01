#-------------------------------------------------
#
# Project created by QtCreator 2018-07-06T21:19:05
#
#-------------------------------------------------

QT       += core gui sql network designer
# 为了兼容Qt4
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
#windows    使用第三方库
LIBS += -LC:/Software/mysql-5.5.50-winx64/lib -llibmysql \
        -LC:/Software/opencv/build/x64/vc14/lib -lopencv_world340d
INCLUDEPATH += C:/Software/opencv/build/include/opencv \
            C:/Software/opencv/build/include/opencv2 \
            C:/Software/opencv/build/include/
#添加静态库生成的
#INCLUDEPATH += C:/Users/MISAKI/Documents/QTWorkspace/GOSICK
#win32:CONFIG(debug, debug|release): {}
#else:win32:CONFIG(release, debug|release): {}
#else:unix: {}

#LIBS += -llibmysql
#linux      使用第三方库
#LIBS += -lmysqlclient

# VS编译器使用下面方式获取管理员权限
#QMAKE_LFLAGS += /MANIFESTUAC:\"level=\'requireAdministrator\' uiAccess=\'false\'\"

TARGET = GOSICK
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    dbconnectdlg.cpp \
    conversiondlg.cpp \
    stackewidgetddlg.cpp \
    mysqlqt.cpp \
    filemanagewidget.cpp \
    filetexteditwidget.cpp \
    mysqllib.cpp \
    filetransfer.cpp \
    file.c \
    gqtcpserver.cpp \
    filetransferthread.cpp \
    tcpserverdlg.cpp \
    gqtcpsocket.cpp \
    customwidget.cpp \
    gscanplugin.cpp \
    plotterwidget.cpp \
    plottertest.cpp

HEADERS += \
        mainwindow.h \
    dbconnectdlg.h \
    conversiondlg.h \
    stackewidgetddlg.h \
    mysqlqt.h \
    filemanagewidget.h \
    filetexteditwidget.h \
    mysqllib.h \
    filetransfer.h \
    file.h \
    gqtcpserver.h \
    filetransferthread.h \
    tcpserverdlg.h \
    gqtcpsocket.h \
    customwidget.h \
    gscanplugin.h \
    plotterwidget.h \
    plottertest.h

RESOURCES += \
    icon.qrc

#Linux下gdb不显示qDebug内容可以添加此配置，显示终端
#CONFIG += console
CONFIG += c++11 warn_off thread plugin

#DESTDIR 生成目标的路径
//DESTDIR = $(QTDIR)/command/
#DESTDIR_TARGET
