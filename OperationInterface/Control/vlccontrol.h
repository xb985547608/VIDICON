#ifndef VLCCONTROL_H
#define VLCCONTROL_H

#include <QWidget>
#include "operationinterface_global.h"

#ifdef __cplusplus
extern "C"{
#endif

#include <vlc/vlc.h>

#ifdef __cplusplus
}
#endif

#define VLCCONTROLINIT    0x2001
#define VLCCONTROLPLAY    0x2002
#define VLCCONTROLSTOP    0x2003

class OPERATIONINTERFACESHARED_EXPORT VlcControl : public QObject
{
    Q_OBJECT

public:
    enum VlcState{
        VLCPLAY  = 0x01,
        VLCSTOP  = 0x02,
        VLCPAUSE = 0x04
    };
    static VlcControl *getInstance()
    {
        if(_instance == NULL){
            _instance = new VlcControl();
        }
        return _instance;
    }

    ~VlcControl();

    void setCache(int cache) { vlcCache = cache; }
    int  getCache() const { return vlcCache; }

    void setWinId(WId id) { vlcWId = id; }
    WId  getWinId() const{ return vlcWId; }

    void setVolume(int volume);
    int  getVolume() const { return vlcVolume; }

    void setUrl(const QString url) { vlcUrl = url; }
    QString getUrl() const { return vlcUrl; }

    int getDuration();
    int getProgress();
    void setPosition(float pos);
    int getVlcState() const { return vlcState; }

    int init(QString url, WId id);
    int play();
    int pause();
    int stop();

private:
    VlcControl(QObject *parent = Q_NULLPTR);

public slots:
    void handlerVlcControl(int type, QString url, WId id);

private:
    static VlcControl     *_instance;
    libvlc_instance_t     *vlcInstance;
    libvlc_media_t        *vlcMedia;
    libvlc_media_player_t *vlcMediaPlayer;
    QString                vlcUrl;
    int                    vlcCache;
    WId                    vlcWId;
    int                    vlcState;
    int                    vlcVolume;
};

#endif // VLCCONTROL_H
