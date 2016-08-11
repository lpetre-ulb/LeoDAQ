#-------------------------------------------------
#
# Project created by QtCreator 2016-08-11T14:21:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LeoDAQ
DESTDIR = bin
TEMPLATE = app


SOURCES += src/main.cpp \
    src/mainwindow.cpp \
    src/hv.cpp \
    src/scaler.cpp \
    src/tdc.cpp

OBJECTS_DIR = build/.obj
MOC_DIR = build/.moc
RCC_DIR = build/.rcc
UI_DIR = build/.ui

INCLUDEPATH += include

HEADERS  += include/mainwindow.h \
    include/hv.h \
    include/scaler.h \
    include/tdc.h

FORMS    += form/mainwindow.ui \
    form/hv.ui \
    form/scaler.ui \
    form/tdc.ui
