#-------------------------------------------------
#
# Project created by QtCreator 2016-12-04T17:40:14
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = NetworkServer
TEMPLATE = lib

DEFINES += NETWORKSERVER_LIBRARY

SOURCES += networkserver.cpp \
    socketdata.cpp

HEADERS += networkserver.h\
        networkserver_global.h \
    socketdata.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DESTDIR += ../bin
