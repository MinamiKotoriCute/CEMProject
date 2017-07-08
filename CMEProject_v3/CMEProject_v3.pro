#-------------------------------------------------
#
# Project created by QtCreator 2017-02-18T12:23:08
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CMEProject_v3
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    controlthread.cpp \
    serverthread.cpp \
    server.cpp \
    control.cpp \
    overviewwidget.cpp \
    database.cpp \
    basedataobject.cpp

HEADERS  += mainwindow.h \
    controlthread.h \
    serverthread.h \
    server.h \
    control.h \
    overviewwidget.h \
    database.h \
    basedataobject.h

include("D:\All\Code\QT\module\defineheader\defineheader.pri")
