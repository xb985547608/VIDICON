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
    static VlcControl *getInstance()
    {
        if(s_instance == NULL){
            s_instance = new VlcControl();
        }
        return s_instance;
    }

    ~VlcControl();

    void setHost(QString host) { m_host = host; }
    void setPort(QString port) { m_port = port; }
    void setUser(QString u) { m_user = u; }
    void setPasswd(QString p) { m_passwd = p; }

    void setCache(int cache) { m_vlcCache = cache; }
    int  getCache() const { return m_vlcCache; }

    void setWinId(WId id) { m_vlcWId = id; }
    WId  getWinId() const{ return m_vlcWId; }

    void setVolume(int volume);
    int  getVolume() const { return m_vlcVolume; }

    int getDuration();
    int getProgress();
    void setPosition(float pos);
    libvlc_state_t getVlcState() { return libvlc_media_get_state(m_vlcMedia); }

    int init(QString url, WId id);
    int play();
    int pause();
    int stop();

private:
    VlcControl(QObject *parent = Q_NULLPTR);

public slots:
    void handleVlcControl(int type, int subtype, WId id);

private:
    static VlcControl     *s_instance;
    libvlc_instance_t     *m_vlcInstance;
    libvlc_media_t        *m_vlcMedia;
    libvlc_media_player_t *m_vlcMediaPlayer;
    int                    m_vlcCache;
    WId                    m_vlcWId;
    int                    m_vlcVolume;

    QString m_host;
    QString m_port;
    QString m_user;
    QString m_passwd;
};

#endif // VLCCONTROL_H
