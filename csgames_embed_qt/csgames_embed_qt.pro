#-------------------------------------------------
#
# Project created by QtCreator 2013-12-21T14:38:12
#
#-------------------------------------------------

QT       += core gui

include($$PWD/QtSerialPort/qt4support/helper.pri)
include($$PWD/QtSerialPort/serialport.pri)

TARGET = csgames2014
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
