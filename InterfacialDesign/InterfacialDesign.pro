#-------------------------------------------------
#
# Project created by QtCreator 2017-12-07T19:09:51
#
#-------------------------------------------------

QT       += core gui network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VIDICON
TEMPLATE = app
CONFIG += c++11

MOC_DIR = $$PWD/moc
RCC_DIR = $$PWD/rcc
UI_DIR  = $$PWD/ui
OBJECTS_DIR = $$PWD/obj

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG(debug, debug|release){
    LIBS += $$PWD/debug/OperationInterface.dll
}else{
    LIBS += $$PWD/release/OperationInterface.dll
}

INCLUDEPATH += $$PWD/../OperationInterface/
INCLUDEPATH += $$PWD/../OperationInterface/Control/sdk/include

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    customstyle.cpp \
    selectcurrentipdialog.cpp

HEADERS += \
        mainwindow.h \
    customstyle.h \
    selectcurrentipdialog.h

FORMS += \
        mainwindow.ui \
    selectcurrentipdialog.ui

include($$PWD/Login/Login.pri)
include($$PWD/Preview/Preview.pri)
include($$PWD/Settings/Settings.pri)
include($$PWD/Home/Home.pri)
include($$PWD/Playback/Playback.pri)
include($$PWD/Photo/Photo.pri)
include($$PWD/Download/Download.pri)
include($$PWD/Navigation/Navigation.pri)


