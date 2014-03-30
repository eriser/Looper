#-------------------------------------------------
#
# Project created by QtCreator 2014-03-14T17:55:57
#
#-------------------------------------------------

QT       += core gui multimedia

CONFIG += mobility
MOBILITY += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Looper
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    TrackRecorder.cpp \
    TrackSettings.cpp \
    TrackRecorderStatus.cpp

HEADERS  += MainWindow.hpp \
    TrackRecorder.hpp \
    TrackSettings.hpp \
    TrackRecorderStatus.hpp

FORMS    += MainWindow.ui \
    TrackSettings.ui \
    TrackRecorderStatus.ui
