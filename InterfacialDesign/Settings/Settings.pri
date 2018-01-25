include($$PWD/DrawSubWidget/DrawSubWidget.pri)
include($$PWD/Dialog/Dialog.pri)

FORMS += \
    $$PWD/settingsform.ui

HEADERS += \
    $$PWD/settingswidget.h \
    $$PWD/tabmedia.h \
    $$PWD/tabnetwork.h \
    $$PWD/tabalarm.h \
    $$PWD/tabrecord.h \
    $$PWD/tabsystem.h \
    $$PWD/boxwidget.h \
    $$PWD/tableview.h

SOURCES += \
    $$PWD/settingswidget.cpp \
    $$PWD/tabmedia.cpp \
    $$PWD/tabnetwork.cpp \
    $$PWD/tabalarm.cpp \
    $$PWD/tabrecord.cpp \
    $$PWD/tabsystem.cpp \
    $$PWD/boxwidget.cpp \
    $$PWD/tableview.cpp

RESOURCES += \
    $$PWD/settings.qrc
