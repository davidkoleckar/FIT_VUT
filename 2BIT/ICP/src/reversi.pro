#-------------------------------------------------
#
# Project created by QtCreator 2016-05-02T13:48:33
#
#-------------------------------------------------

QT       += core gui
CONFIG += c++11


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = reversi
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    help.cpp \
    load.cpp \
    src/Board.cpp \
    src/BoardField.cpp \
    src/Disk.cpp \
    src/Game.cpp \
    src/Player.cpp \
    src/ReversiRules.cpp \
    guigame.cpp \
    src/newGame.cpp

HEADERS  += mainwindow.h \
    help.h \
    load.h \
    include/Board.h \
    include/BoardField.h \
    include/BorderField.h \
    include/Disk.h \
    include/Field.h \
    include/Game.h \
    include/Player.h \
    include/ReversiRules.h \
    include/Rules.h \
    guigame.h \
    include/newGame.h

FORMS    += mainwindow.ui \
    help.ui \
    load.ui \
    guigame.ui

RESOURCES += \
    image.qrc
