#include <QDebug>
#include "vlccontrol.h"
#include <QThread>

VlcControl *VlcControl::_instance = NULL;
VlcControl::VlcControl(QString host, QString port, QObject *parent) : QObject(parent),
    vlcInstance(NULL),
    vlcMedia(NULL),
    vlcMediaPlayer(NULL),
    vlcCache(300),
    vlcState(VLCSTOP),
    vlcVolume(50),
    dstIPAddr(host),
    dstPort(port),
    user("admin"),
    passwd("admin")
{

}

void VlcControl::handlerVlcControl(int type, int subtype, WId id)
{
    switch(type) {
    case VLCCONTROLINIT: {
        QString url = QString("rtsp://%4:%5@%1:%2/H264?channel=0&subtype=%3&unicast=true&proto=Onvif")
                .arg(dstIPAddr)
                .arg(dstPort)
                .arg(subtype)
                .arg(user)
                .arg(passwd);
        stop();
        init(url, id);
        play();
        qDebug() << "#VlcControl# init vlc,start play";
        break;
    }
    case VLCCONTROLPLAY: {
        play();
        break;
    }
    case VLCCONTROLSTOP: {
        stop();
        break;
    }
    }
}

VlcControl::~VlcControl()
{
    if(vlcMediaPlayer != NULL){
        libvlc_media_player_release(vlcMediaPlayer);
    }
    if(vlcMedia != NULL){
        libvlc_media_release(vlcMedia);
    }
    if(vlcInstance != NULL){
        libvlc_release(vlcInstance);
    }
}

void VlcControl::setVolume(int volume)
{
    vlcVolume = volume;
    if(vlcMediaPlayer != NULL){
        if(libvlc_audio_set_volume(vlcMediaPlayer, vlcVolume) == -1) {
            qDebug("#VlcControl# setVolume Error, volume out of range");
        }
    }
}

int VlcControl::getDuration()
{
    if(vlcMediaPlayer == NULL){
        return -1;
    }else{
        return libvlc_media_player_get_length(vlcMediaPlayer);
    }
}

int VlcControl::getProgress()
{
    if(vlcMediaPlayer == NULL){
        return -1;
    }else{
        return libvlc_media_player_get_time(vlcMediaPlayer);
    }
}

void VlcControl::setPosition(float pos)
{
    if(vlcMediaPlayer == NULL){
        return ;
    }else{
        libvlc_media_player_set_position(vlcMediaPlayer, pos);
    }
}

int VlcControl::init(QString url, WId id)
{
    if(vlcMediaPlayer != NULL){
        libvlc_media_player_release(vlcMediaPlayer);
    }
    if(vlcMedia != NULL){
        libvlc_media_release(vlcMedia);
    }
    if(vlcInstance == NULL) {
        vlcInstance = libvlc_new(0, NULL);
        if(vlcInstance == NULL) {
            vlcInstance = libvlc_new(0, NULL);
            if(vlcInstance == NULL) {
                qDebug("#VlcControl# init Error, libvlc_new Instance Error");
                return -1;
            }
        }
    }

    vlcWId = id;

    vlcMedia = libvlc_media_new_location(vlcInstance, url.toStdString().data());
    if(vlcMedia == NULL) {
        qDebug("#VlcControl# init Error, libvlc_media_new_location return NULL");
        return -1;
    }
    vlcMediaPlayer = libvlc_media_player_new_from_media(vlcMedia);
    if(vlcMediaPlayer == NULL) {
        qDebug("#VlcControl# init Error, libvlc_media_player_new_from_media return NULL");
        return -1;
    }
    libvlc_media_player_set_hwnd(vlcMediaPlayer, (void *)vlcWId);
    setVolume(vlcVolume);
    return 0;
}

int VlcControl::play()
{
    int ret = -1;
    if(vlcMediaPlayer != NULL){
        QString para = "network-caching=" + QString::number(vlcCache);
        libvlc_media_add_option(vlcMedia, para.toStdString().data());

        ret = libvlc_media_player_play(vlcMediaPlayer);
        if(ret == 0) {
            vlcState = VLCPLAY;
        }else {
            qDebug("#VlcControl# play Error---------------------------");
        }
    }
    return ret;
}

int VlcControl::pause()
{
    if(vlcMediaPlayer != NULL){
        libvlc_media_player_pause(vlcMediaPlayer);
    }else{
        return -1;
    }
    vlcState = VLCPAUSE;
    return 0;
}

int VlcControl::stop()
{
    if(vlcMediaPlayer != NULL){
        libvlc_media_player_stop(vlcMediaPlayer);
    }else{
        return -1;
    }
    vlcState = VLCSTOP;
    return 0;
}
