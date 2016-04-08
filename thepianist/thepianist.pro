#-------------------------------------------------
#
# Project created by QtCreator 2013-11-04T20:24:59
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = thepianist
TEMPLATE = app

include(QMidi/QMidi.pri)

SOURCES += \
    main.cpp\
    mainwindow.cpp \
    pianoroll.cpp \
    midiplayer.cpp

HEADERS += \
    mainwindow.h \
    pianoroll.h \
    midiplayer.h

FORMS += mainwindow.ui

RESOURCES += \
    thepianist.qrc
