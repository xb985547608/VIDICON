include($$PWD/DrawSubWidget/DrawSubWidget.pri)

FORMS += \
    $$PWD/settingsform.ui

HEADERS += \
    $$PWD/settingswidget.h \
    $$PWD/mediawidget.h \
    $$PWD/networkwidget.h \
    $$PWD/alarmwidget.h \
    $$PWD/recordwidget.h \
    $$PWD/systemwidget.h \
    $$PWD/userinfoview.h \
    $$PWD/boxview.h \
    $$PWD/waitingshade.h

SOURCES += \
    $$PWD/settingswidget.cpp \
    $$PWD/mediawidget.cpp \
    $$PWD/networkwidget.cpp \
    $$PWD/alarmwidget.cpp \
    $$PWD/recordwidget.cpp \
    $$PWD/systemwidget.cpp \
    $$PWD/userinfoview.cpp \
    $$PWD/boxview.cpp \
    $$PWD/waitingshade.cpp

RESOURCES += \
    $$PWD/settings.qrc
