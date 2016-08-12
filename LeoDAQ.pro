#-------------------------------------------------
#
# Project created by QtCreator 2016-08-11T14:21:48
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LeoDAQ
DESTDIR = bin
TEMPLATE = app


SOURCES += src/main.cpp \
    src/mainwindow.cpp \
    src/hv.cpp \
    src/scaler.cpp \
    src/tdc.cpp \
    src/qcustomplot.cpp \
    src/histogram.cpp \
    src/hvmodule.cpp

OBJECTS_DIR = build/.obj
MOC_DIR = build/.moc
RCC_DIR = build/.rcc
UI_DIR = build/.ui

INCLUDEPATH += include

HEADERS  += include/mainwindow.h \
    include/hv.h \
    include/scaler.h \
    include/tdc.h \
    include/qcustomplot.h \
    include/histogram.h \
    include/hvmodule.h

FORMS    += form/mainwindow.ui \
    form/hv.ui \
    form/scaler.ui \
    form/tdc.ui

RESOURCES += data/resources.qrc

QMAKE_CXXFLAGS += -std=c++0x -DLINUX

LIBS += -lCAENVME
