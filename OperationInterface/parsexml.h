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

    bool parseVideoEncodingParameter(VideoEncodingParameter *param, QByteArray data);
    bool parseAudioEncodingParameter(AudioEncodingParameter *param, QByteArray data);
    bool parseOSDParameter(OSDParameter *param, QByteArray data);
    bool parseNTPParameter(NTPParameter *param, QByteArray data);
    bool parseDeviceInfo(DeviceInfo *param, QByteArray data);
    bool parseImageParameter(ImageParameter *param, QByteArray data);
    bool parseBasicParameter(BasicParameter *param, QByteArray data);
    bool parseOtherParameter(OtherParameter *param, QByteArray data);
    bool parsePPPOEParameter(PPPOEParameter *param, QByteArray data);
    bool parseDDNSParameter(DDNSParameter *param, QByteArray data);
    bool parseEmailParameter(EmailParameter *param, QByteArray data);
    bool parseFTPParameter(FTPParameter *param, QByteArray data);
    bool parseBonjourParameter(BonjourParameter *param, QByteArray data);
    bool parseSNMPParameter(SNMPParameter *param, QByteArray data);
    bool parseUPNPParameter(UPNPParameter *param, QByteArray data);
    bool parseHTTPsParameter(HTTPsParameter *param, QByteArray data);
    bool parseP2PParameter(P2PParameter *param, QByteArray data);
    bool parseMotionParameter(MotionDetectionParameter *param, QByteArray data);
    bool parseBlindParameter(VideoBlindAlarmParameter *param, QByteArray data);
    bool parseSensorParameter(SensorAlarmParameter *param, QByteArray data);
    bool parseScheduleParameter(RemoteRecordingPlan *param, QByteArray data);
    bool parseSnapshotParameter(SnapshotPlanParameter *param, QByteArray data);
    bool parseSDCardStatusParameter(SDCardStatus *param, QByteArray data);
    bool parseSDStorageParameter(SDStorageParameter *param, QByteArray data);
    bool parseBackUpQueryParameter(BackUpQueryParameter *param, QByteArray data);
    bool parsePlayingTimeParameter(PlayingTimeParameter *param, QByteArray data);
    bool parseUserConfgInfo(QList<UserConfigInfo> &param, QByteArray data);

    bool parsePullMsg(PullMessage *param, QByteArray data);
    bool parseResponseStatus(ResponseStatus *param, QByteArray data);
    bool parseDiscoveryDevice(WholeDeviceInfo *param, QByteArray data);

signals:

public slots:
};

#endif // PARSEXML_H
