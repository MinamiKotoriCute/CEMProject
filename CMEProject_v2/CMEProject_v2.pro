#-------------------------------------------------
#
# Project created by QtCreator 2016-12-21T09:56:06
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CMEProject_v2
TEMPLATE = app


SOURCES += main.cpp \
    UI/mainwindow.cpp \
    Control/control.cpp \
    NetworkServer/networkserver.cpp \
    Control/controlthread.cpp \
    UI/overviewwidget.cpp \
    WebCrawler/webcrawler.cpp \
    WebCrawler/coursesearch.cpp \
    Control/collectthread.cpp \
    WebCrawler/learningprotfolio.cpp \
    Database/database.cpp \
    UI/iowidget.cpp \
    WebCrawler/electivesystem.cpp

HEADERS += \
    UI/mainwindow.h \
    Control/control.h \
    NetworkServer/networkserver.h \
    Control/controlthread.h \
    UI/overviewwidget.h \
    WebCrawler/webcrawler.h \
    WebCrawler/coursesearch.h \
    Control/collectthread.h \
    WebCrawler/learningprotfolio.h \
    Database/database.h \
    UI/iowidget.h \
    WebCrawler/electivesystem.h

include("D:\All\Code\QT\module\webautomaticindexer\webautomaticindexer.pri")
include("D:\All\Code\QT\module\regularexpression\regularexpression.pri")
include("D:\All\Code\QT\module\expansion\expansion.pri")
