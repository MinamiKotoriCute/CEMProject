#-------------------------------------------------
#
# Project created by QtCreator 2016-12-04T19:42:04
#
#-------------------------------------------------

QT       += network sql

QT       -= gui

TARGET = WebCrawler
TEMPLATE = lib

DEFINES += WEBCRAWLER_LIBRARY

SOURCES += webcrawler.cpp \
    coursesearch.cpp \
    learningprotfolio.cpp \
    coursesearchanalyzer.cpp

HEADERS += webcrawler.h\
        webcrawler_global.h \
    coursesearch.h \
    learningprotfolio.h \
    coursesearchanalyzer.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DESTDIR += ../bin

include("D:\All\Code\QT\module\webautomaticindexer\webautomaticindexer.pri")
include("D:\All\Code\QT\module\regularexpression\regularexpression.pri")
include("D:\All\Code\QT\module\expansion\expansion.pri")
include("D:\Users\Joey\Documents\QtProject\CMEProject\course\course.pri")

INCLUDEPATH += ../Database

LIBS+=  ../bin/Database.lib
