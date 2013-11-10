#-------------------------------------------------
#
# Project created by QtCreator 2013-08-28T10:37:10
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = S-Helmet
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    viewermanager.cpp \
    aboutdialog.cpp \
    walkmanipulator.cpp \
    buildscenethread.cpp

HEADERS  += \
    mainwindow.h \
    viewermanager.h \
    aboutdialog.h \
    walkmanipulator.h \
    buildscenethread.h \
    config.h

FORMS    += \
    mainwindow.ui \
    aboutdialog.ui
LIBS+=/usr/local/lib/libosg.so \
/usr/local/lib/libosgQt.so \
/usr/local/lib/libosgDB.so \
/usr/local/lib/libosgViewer.so \
/usr/local/lib/libosgText.so \
/usr/local/lib/libosgGA.so \
/usr/local/lib/libOpenThreads.so \
/usr/local/lib/libosgUtil.so \

RESOURCES += \
    resource.qrc
