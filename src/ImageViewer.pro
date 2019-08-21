#-------------------------------------------------
#
# Project created by QtCreator 2013-02-28T11:34:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImageViewer.exe
TEMPLATE = app
CONFIG += c++11


SOURCES += main.cpp\
        asyncimageloader.cpp \
        imageslistmodel.cpp \
        mainwindow.cpp \
        imageviewer.cpp

HEADERS  += mainwindow.h \
        asyncimageloader.h \
        imageslistmodel.h \
        imageviewer.h

FORMS    += mainwindow.ui
RESOURCES += \
    resources.qrc
