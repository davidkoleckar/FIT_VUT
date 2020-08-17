#-------------------------------------------------
#
# Project created by QtCreator 2017-03-24T20:41:49
#
#-------------------------------------------------

QT       += core gui

QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = calculator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ../math_library.cpp

HEADERS  += mainwindow.h \
    ../math_library.h

FORMS    += mainwindow.ui

RESOURCES += \
    images.qrc

DISTFILES += \
    ../.gitignore \
    ../Doxyfile \
    ../CMakeLists.txt

