#include <QDebug>
#include "vlccontrol.h"
#include <QThread>
#include "util.h"

VlcControl *VlcControl::s_instance = NULL;
VlcControl::VlcControl(QObject *parent) : QObject(parent),
    m_vlcInstance(NULL),
    m_vlcMedia(NULL),
    m_vlcMediaPlayer(NULL),
    m_vlcCache(300),
    m_vlcVolume(50),
    m_host("192.168.0.66"),
    m_port("554"),
    m_user("admin"),
    m_passwd("admin")
{

}

void VlcControl::handleVlcControl(int type, int subtype, WId id)
{
    switch(type) {
    case VLCCONTROLINIT: {
        QString url = QString("rtsp://%4:%5@%1:%2/H264?channel=0&subtype=%3&unicast=true&proto=Onvif")
                .arg(m_host)
                .arg(m_port)
                .arg(subtype)
                .arg(m_user)
                .arg(m_passwd);

        stop();
        init(url, id);
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
    if(m_vlcMediaPlayer != NULL){
        libvlc_media_player_release(m_vlcMediaPlayer);
    }
    if(m_vlcMedia != NULL){
        libvlc_media_release(m_vlcMedia);
    }
    if(m_vlcInstance != NULL){
        libvlc_release(m_vlcInstance);
    }
}

void VlcControl::setVolume(int volume)
{
    m_vlcVolume = volume;
    if(m_vlcMediaPlayer != NULL){
        if(libvlc_audio_set_volume(m_vlcMediaPlayer, m_vlcVolume) == -1) {
            qDebug("#VlcControl# setVolume Error, volume out of range");
        }
    }
}

int VlcControl::getDuration()
{
    if(m_vlcMediaPlayer == NULL){
        return -1;
    }else{
        return libvlc_media_player_get_length(m_vlcMediaPlayer);
    }
}

int VlcControl::getProgress()
{
    if(m_vlcMediaPlayer == NULL){
        return -1;
    }else{
        return libvlc_media_player_get_time(m_vlcMediaPlayer);
    }
}

void VlcControl::setPosition(float pos)
{
    if(m_vlcMediaPlayer == NULL){
        return ;
    }else{
        libvlc_media_player_set_position(m_vlcMediaPlayer, pos);
    }
}

int VlcControl::init(QString url, WId id)
{
    if(m_vlcMediaPlayer != NULL){
        libvlc_media_player_release(m_vlcMediaPlayer);
    }
    if(m_vlcMedia != NULL){
        libvlc_media_release(m_vlcMedia);
    }
    if(m_vlcInstance == NULL) {
        m_vlcInstance = libvlc_new(0, NULL);
        if(m_vlcInstance == NULL) {
            m_vlcInstance = libvlc_new(0, NULL);
            if(m_vlcInstance == NULL) {
                qDebug("#VlcControl# init Error, libvlc_new Instance Error");
                return -1;
            }
        }
    }

    m_vlcWId = id;

    //根据url创建一个媒体对象
    m_vlcMedia = libvlc_media_new_location(m_vlcInstance, url.toStdString().data());
    if(m_vlcMedia == NULL) {
        qDebug("#VlcControl# init Error, libvlc_media_new_location return NULL");
        return -1;
    }
    //创建媒体播放器
    m_vlcMediaPlayer = libvlc_media_player_new_from_media(m_vlcMedia);
    if(m_vlcMediaPlayer == NULL) {
        qDebug("#VlcControl# init Error, libvlc_media_player_new_from_media return NULL");
        return -1;
    }
    //设置显示窗口
    libvlc_media_player_set_hwnd(m_vlcMediaPlayer, (void *)m_vlcWId);

    setVolume(m_vlcVolume);
    return 0;
}

int VlcControl::play()
{
    int ret = 0;
    if (m_vlcMedia == NULL)
        return -1;

    if (libvlc_media_get_state(m_vlcMedia) != libvlc_Playing) {
        if (m_vlcMediaPlayer != NULL) {
            QString para = "network-caching=" + QString::number(m_vlcCache);
            libvlc_media_add_option(m_vlcMedia, para.toStdString().data());

            ret = libvlc_media_player_play(m_vlcMediaPlayer);
            if(ret == -1) {
                qDebug("#VlcControl# play Error---------------------------");
            }
        }
    } else {
        qDebug() << "#VlcControl# playing.......";
    }
    return ret;
}

int VlcControl::pause()
{
    if(m_vlcMediaPlayer != NULL){
        libvlc_media_player_pause(m_vlcMediaPlayer);
    }else{
        return -1;
    }
    return 0;
}

int VlcControl::stop()
{
    if(m_vlcMediaPlayer != NULL){
        libvlc_media_player_stop(m_vlcMediaPlayer);
    }else{
        return -1;
    }
    return 0;
}
