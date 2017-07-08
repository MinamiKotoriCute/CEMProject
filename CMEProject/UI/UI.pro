#-------------------------------------------------
#
# Project created by QtCreator 2016-12-04T17:38:16
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CME
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

DESTDIR += ../bin

INCLUDEPATH += ../NetworkServer\
        ../Database\
        ../WebCrawler\
        ../Control

LIBS+=  ../bin/NetworkServer.lib\
        ../bin/Database.lib\
        ../bin/WebCrawler.lib\
        ../bin/Control.lib

include("D:\Users\Joey\Documents\QtProject\CMEProject\course\course.pri")
