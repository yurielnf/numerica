#-------------------------------------------------
#
# Project created by QtCreator 2013-03-02T21:10:21
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = numerica
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

# Adding numerica
include(numerica/numerica.pri)
INCLUDEPATH += $$PWD/numerica
DEPENDPATH += $$PWD/numerica

# Adding boost
INCLUDEPATH+=/winD/install/downloads/boost_1_53_0

SOURCES += \
    main.cpp
