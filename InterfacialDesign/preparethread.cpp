#include "preparethread.h"
#include "Protocol/vidiconprotocol.h"
#include "Control/vlccontrol.h"
#include <QDebug>
#include <QTimer>

PrepareThread::PrepareThread(QObject *parent) : QThread(parent)
{
}

void PrepareThread::run()
{
    VlcControl *c =  VlcControl::getInstance();

//    VidiconProtocol::PlayStateParameter *param1 = new VidiconProtocol::PlayStateParameter;
//    param1->htmlid = 45456;
//    param1->StateValue = 0;
//    v->setFastOrSlowPlayState("R00001", *param1);

//    VidiconProtocol::StartPlayingParameter *param = new VidiconProtocol::StartPlayingParameter;
//    param->htmlid = 45456;
//    param->playing = 1;
//    param->Time = QDateTime(QDate(2018, 1, 22), QTime(21, 43, 00));
//    v->setRecordStartPlayingTime("R00001", *param);

//    VidiconProtocol::BackUpQueryParameter param;
//    param.Type = 2;
//    param.Date = "2018-1-22";
//    VidiconProtocol::getInstance()->backUpQuery("R00001", param);
//    sleep(1);
//    v->getVideoEncodingParameter("R00001", 0, 0);
//    v->getVideoEncodingParameter("R00001", 0, 1);
//    v->getVideoEncodingOption("R00001", 0, 0);
//    v->getAudioEncodingParameter("R00001");
//    v->getVideoEncodingOption("R00001", 0, 1);
//    v->getOSDParameter("R00001");
//    v->getNTP("R00001");
//    v->getDeviceInfomation("R00001");
//    v->getImageParameter("R00001");
    exec();
}

