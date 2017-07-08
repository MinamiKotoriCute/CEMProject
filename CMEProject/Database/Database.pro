#-------------------------------------------------
#
# Project created by QtCreator 2016-12-04T19:25:59
#
#-------------------------------------------------

QT       += sql

QT       -= gui

TARGET = Database
TEMPLATE = lib

DEFINES += DATABASE_LIBRARY

SOURCES += database.cpp

HEADERS += database.h\
        database_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DESTDIR += ../bin

include("D:\Users\Joey\Documents\QtProject\CMEProject\course\course.pri")
