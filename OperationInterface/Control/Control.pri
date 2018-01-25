LIBS += -L$$PWD/sdk -lvlc -lvlccore
INCLUDEPATH += $$PWD/sdk/include

HEADERS += \
    $$PWD/vlccontrol.h

SOURCES += \
    $$PWD/vlccontrol.cpp
