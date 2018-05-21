#-------------------------------------------------
#
# Project created by QtCreator 2017-12-07T20:21:31
#
#-------------------------------------------------

QT += widgets network xml

TARGET = OperationInterface
TEMPLATE = lib

DEFINES += OPERATIONINTERFACE_LIBRARY

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#CONFIG(debug, debug|release){
#    DESTDIR = ../InterfacialDesign/debug/
#}else{
#    DESTDIR = ../InterfacialDesign/release/
#}
DESTDIR = $$PWD/../Bin
MOC_DIR = $$PWD/moc
RCC_DIR = $$PWD/rcc
UI_DIR  = $$PWD/ui
OBJECTS_DIR = $$PWD/obj

SOURCES += \
    settings.cpp \
    parsexml.cpp \
    searchdevicethread.cpp \

HEADERS += \
    settings.h \
    operationinterface_global.h \
    parsexml.h \
    searchdevicethread.h \
    util.h \
    authenticator.h

INCLUDEPATH += $PWD

unix {
    target.path = /usr/lib
    INSTALLS += target
}

include($$PWD/Protocol/Protocol.pri)
include($$PWD/Control/Control.pri)
include($$PWD/Network/Network.pri)
#include($$PWD/Logger/Logger.pri)
