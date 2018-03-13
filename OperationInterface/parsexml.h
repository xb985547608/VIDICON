#ifndef PARSEXML_H
#define PARSEXML_H

#include <QObject>
#include "Protocol/vidiconprotocol.h"
#include "operationinterface_global.h"
#include "searchdevicethread.h"

class OPERATIONINTERFACESHARED_EXPORT ParseXML : public QObject
{
    Q_OBJECT
private:
    static ParseXML *_instance;
    explicit ParseXML(QObject *parent = nullptr);
public:
    static ParseXML *getInstance(){
        if(_instance == NULL){
            _instance = new ParseXML;
        }
        return _instance;
    }

    bool parseVideoEncodingParameter(VidiconProtocol::VideoEncodingParameter *param, QByteArray data);
    bool parseAudioEncodingParameter(VidiconProtocol::AudioEncodingParameter *param, QByteArray data);
    bool parseOSDParameter(VidiconProtocol::OSDParameter *param, QByteArray data);
    bool parseNTPParameter(VidiconProtocol::NTPParameter *param, QByteArray data);
    bool parseDeviceInfo(VidiconProtocol::DeviceInfo *param, QByteArray data);
    bool parseImageParameter(VidiconProtocol::ImageParameter *param, QByteArray data);
    bool parseBasicParameter(VidiconProtocol::BasicParameter *param, QByteArray data);
    bool parseOtherParameter(VidiconProtocol::OtherParameter *param, QByteArray data);
    bool parsePPPOEParameter(VidiconProtocol::PPPOEParameter *param, QByteArray data);
    bool parseDDNSParameter(VidiconProtocol::DDNSParameter *param, QByteArray data);
    bool parseEmailParameter(VidiconProtocol::EmailParameter *param, QByteArray data);
    bool parseFTPParameter(VidiconProtocol::FTPParameter *param, QByteArray data);
    bool parseBonjourParameter(VidiconProtocol::BonjourParameter *param, QByteArray data);
    bool parseSNMPParameter(VidiconProtocol::SNMPParameter *param, QByteArray data);
    bool parseUPNPParameter(VidiconProtocol::UPNPParameter *param, QByteArray data);
    bool parseHTTPsParameter(VidiconProtocol::HTTPsParameter *param, QByteArray data);
    bool parseP2PParameter(VidiconProtocol::P2PParameter *param, QByteArray data);
    bool parseMotionParameter(VidiconProtocol::MotionDetectionParameter *param, QByteArray data);
    bool parseBlindParameter(VidiconProtocol::VideoBlindAlarmParameter *param, QByteArray data);
    bool parseSensorParameter(VidiconProtocol::SensorAlarmParameter *param, QByteArray data);
    bool parseScheduleParameter(VidiconProtocol::RemoteRecordingPlan *param, QByteArray data);
    bool parseSnapshotParameter(VidiconProtocol::SnapshotPlanParameter *param, QByteArray data);
    bool parseSDCardStatusParameter(VidiconProtocol::SDCardStatus *param, QByteArray data);
    bool parseSDStorageParameter(VidiconProtocol::SDStorageParameter *param, QByteArray data);
    bool parseBackUpQueryParameter(VidiconProtocol::BackUpQueryParameter *param, QByteArray data);
    bool parsePlayingTimeParameter(VidiconProtocol::PlayingTimeParameter *param, QByteArray data);
    bool parseUserConfgInfo(QList<VidiconProtocol::UserConfigInfo> &param, QByteArray data);

    bool parsePullMsg(VidiconProtocol::PullMessage *param, QByteArray data);
    bool parseResponseStatus(VidiconProtocol::ResponseStatus *param, QByteArray data);
    bool parseDiscoveryDevice(SearchDeviceThread::DeviceInfo *param, QByteArray data);

signals:

public slots:
};

#endif // PARSEXML_H
