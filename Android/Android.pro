#-------------------------------------------------
#
# Project created by QtCreator 2016-12-04T20:57:06
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Android
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    loginwidget.cpp \
    network.cpp \
    electivecourseswidget.cpp \
    popularitywidget.cpp \
    licensewidget.cpp

HEADERS  += mainwindow.h \
    loginwidget.h \
    network.h \
    electivecourseswidget.h \
    popularitywidget.h \
    licensewidget.h

CONFIG += mobility
MOBILITY =

include("D:\All\Code\QT\module\asynchronous\asynchronous.pri")

DISTFILES += \
    android-sources/AndroidManifest.xml

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android-sources
