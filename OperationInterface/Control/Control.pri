#第三方库 VLC --> 万能播放器
LIBS += -L$$PWD/sdk -lvlc -lvlccore
INCLUDEPATH += $$PWD/sdk/include
#第三方库 CodeMeter --> 加密狗
LIBS += -L$$PWD/CodeMeter/lib -lWibuCm32
INCLUDEPATH += $$PWD/CodeMeter/include

HEADERS += \
    $$PWD/vlccontrol.h \
    $$PWD/cmdongle.h

SOURCES += \
    $$PWD/vlccontrol.cpp \
    $$PWD/cmdongle.cpp
