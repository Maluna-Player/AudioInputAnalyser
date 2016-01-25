#-------------------------------------------------
#
# Project created by QtCreator 2016-01-25T10:41:49
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = micro
TEMPLATE = app


SOURCES += main.cpp\
    audioinfo.cpp \
    renderarea.cpp \
    inputtest.cpp

HEADERS  += \
    audioinfo.h \
    renderarea.h \
    inputtest.h

FORMS    += mainwindow.ui
