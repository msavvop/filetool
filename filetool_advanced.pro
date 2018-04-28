#-------------------------------------------------
#
# Project created by QtCreator 2016-05-20T19:07:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = filetool
TEMPLATE = app


SOURCES += main.cpp\
        filetool.cpp \
    pastedialog.cpp \
    thread.cpp \
    overwritedialog.cpp \
    undoredo.cpp

HEADERS  += filetool.h \
    pastedialog.h \
    thread.h \
    overwritedialog.h \
    undoredo.h

FORMS    += filetool.ui \
    pastedialog.ui \
    overwritedialog.ui

RESOURCES += \
    filetool.qrc


TRANSLATIONS = filetool_el.ts

