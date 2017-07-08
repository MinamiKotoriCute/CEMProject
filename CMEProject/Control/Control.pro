#-------------------------------------------------
#
# Project created by QtCreator 2016-12-05T10:02:05
#
#-------------------------------------------------

QT       -= gui

QT       += network sql

TARGET = Control
TEMPLATE = lib

DEFINES += CONTROL_LIBRARY

SOURCES += control.cpp \
    collectthread.cpp

HEADERS += control.h\
        control_global.h \
    collectthread.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DESTDIR += ../bin

INCLUDEPATH += ../NetworkServer\
        ../Database\
        ../WebCrawler

LIBS+=  ../bin/NetworkServer.lib\
        ../bin/Database.lib\
        ../bin/WebCrawler.lib

include("D:\All\Code\QT\module\expansion\expansion.pri")
include("D:\Users\Joey\Documents\QtProject\CMEProject\course\course.pri")
