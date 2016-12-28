#-------------------------------------------------
#
# Project created by QtCreator 2016-11-28T16:01:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += /Zc:strictStrings

TARGET = fixHypackRawData
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

RESOURCES += \
    resource.qrc
RC_FILE += logo.rc
