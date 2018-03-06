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

#define MAINSTREAMTYPE    0
#define SUBSTREAMTYPE     1
#define BACKUPSTREAMTYPE  3

class OPERATIONINTERFACESHARED_EXPORT VlcControl : public QObject
{
    Q_OBJECT

public:
    enum VlcState{
        VLCPLAY  = 0x01,
        VLCSTOP  = 0x02,
        VLCPAUSE = 0x04
    };
    static VlcControl *getInstance(QString host = "", int port = -1)
    {
        if(_instance == NULL){
            _instance = new VlcControl(host, QString::number(port));
        }
        return _instance;
    }

    ~VlcControl();

    void setHost(QString host) { dstIPAddr = host; }
    void setPort(QString port) { dstPort = port; }
    void setUser(QString u) { user = u; }
    void setPasswd(QString p) { passwd = p; }

    void setCache(int cache) { vlcCache = cache; }
    int  getCache() const { return vlcCache; }

    void setWinId(WId id) { vlcWId = id; }
    WId  getWinId() const{ return vlcWId; }

    void setVolume(int volume);
    int  getVolume() const { return vlcVolume; }

    int getDuration();
    int getProgress();
    void setPosition(float pos);
    int getVlcState() const { return vlcState; }

    int init(QString url, WId id);
    int play();
    int pause();
    int stop();

private:
    VlcControl(QString host, QString port, QObject *parent = Q_NULLPTR);

public slots:
    void handleVlcControl(int type, int subtype, WId id);

private:
    static VlcControl     *_instance;
    libvlc_instance_t     *vlcInstance;
    libvlc_media_t        *vlcMedia;
    libvlc_media_player_t *vlcMediaPlayer;
    int                    vlcCache;
    WId                    vlcWId;
    int                    vlcState;
    int                    vlcVolume;

    QString dstIPAddr;
    QString dstPort;
    QString user;
    QString passwd;
};

#endif // VLCCONTROL_H
