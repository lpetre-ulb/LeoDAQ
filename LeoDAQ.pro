#-------------------------------------------------
#
# Project created by QtCreator 2016-08-11T14:21:48
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LeoDAQ
DESTDIR = bin
targetinstall.path = /usr/local/bin/LeoDAQ
targetinstall.files = bin/LeoDAQ

INSTALLS += targetinstall

icon.path = /usr/local/LeoDAQ
icon.files = data/iconLeoDAQ.png
icon.extra = cp data/LeoDAQ.desktop ~/Desktop/LeoDAQ.desktop

INSTALLS += icon

TEMPLATE = app


SOURCES += src/main.cpp \
    src/mainwindow.cpp \
    src/hv.cpp \
    src/scaler.cpp \
    src/tdc.cpp \
    src/qcustomplot.cpp \
    src/histogram.cpp \
    src/hvmodule.cpp \
    src/tdcmodule.cpp \
    src/scalermodule.cpp

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
    include/hvmodule.h \
    include/tdcmodule.h \
    include/scalermodule.h

FORMS += form/mainwindow.ui \
    form/hv.ui \
    form/scaler.ui \
    form/tdc.ui

RESOURCES += data/resources.qrc

QMAKE_CXXFLAGS += -std=c++0x -DLINUX

LIBS += -lCAENVME
