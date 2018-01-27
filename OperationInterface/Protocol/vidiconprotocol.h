#ifndef VIDICONPROTOCOL_H
#define VIDICONPROTOCOL_H

#include <QObject>
#include <QByteArray>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QString>
#include <QPoint>
#include <QTime>
#include <QTimer>
#include <QDebug>
#include "operationinterface_global.h"

#define VIDEOENCODINGPARAM   1024
#define AUDIOENCODINGPARAM   1025
#define OSDPARAMETER         1026
#define NTPPARAMETER         1027
#define DEVICEINFO           1028
#define PRIVACYPARAMETER     1029
#define IMAGEPARAMETER       1030
#define TCPIPPARAMETER       1031
#define OTHERPARAMETER       1032
#define PPPOEPARAMETR        1033
#define DDNSPARAMETER        1034
#define EMAILPARAMETER       1035
#define FTPPARAMETER         1036
#define BONJOURPARAMETER     1037
#define SNMPPARAMETER        1038
#define UPNPPARAMETER        1039
#define HTTPSPARAMETER       1040
#define P2PPARAMETER         1041
#define RTSPPARAMETER        1042
#define MOTIONALARAPARAMETER 1043
#define BLINDALARMPARAMETER  1044
#define SENSORALARMPARAMETER 1045
#define SCHEDULEPARAMETER    1046
#define SDSTORAGEPARAMETER   1047
#define SNAPSHOTPARAMETER    1048
#define DESTINATIONPARAMETER 1049
#define NASPARAMETER         1050
#define SDCARDPARAMETER      1051
#define FORMATSDCARD         1052
#define REBOOT               1053
#define RECOVEDEFAULT        1054
#define QUERYVIDEONAMEDAY    1055
#define QUERYVIDEOTIMEDAY    1056
#define QUERYFILEMONTH       1057
#define QUERYPICTURENAMEDAY  1058
#define BACKQUERY            1059
#define PULLMESSAGE          1060
#define STARTPLAYING         1061
#define PLAYSTATE            1062

#define RESPONSESTATUS       0x8888

#define TIMEOUTMSEC          2000

class OPERATIONINTERFACESHARED_EXPORT VidiconProtocol : public QObject
{
    Q_OBJECT
    enum State{
        Leisure,
        Busy
    };
public:    
    static VidiconProtocol *getInstance()
    {
        if(NULL == _instance){
            _instance = new VidiconProtocol("192.168.0.66", "80");
        }
        return _instance;
    }
    ~VidiconProtocol();

    void init();

private:
    static VidiconProtocol *_instance;
    VidiconProtocol(QString host, QString port, QObject *parent = 0);

public:
    typedef struct ResponseStatus{
        QString RequestURL;
        int     StatusCode;
        QString StatusString;
    }ResponseStatus;

    typedef struct DeviceInfo{
        QString DeviceName;
        int DeviceID;
        QString DeviceModel;
        QString SoftwareVer;
        QString FactoryInfo;
    }DeviceInfo;
    void getDeviceInfomation(QString SessionID);
    void login(QString user, QString passwd);
    void logout(QString SessionID);

    /*********************Media Settings*********************/

    //channel:   非0：鱼眼模式   0：普通模式
    //streamtype： 0：mian  1:sub
    void getVideoEncodingOption(QString SessionID, int Channel, int StreamType);

    typedef struct VideoEncodingParameter{
        int     Channel;
        int     StreamType;
        QString VideoCodecType;
        int     VideoResolutionWidth;
        int     VideoResolutionHeight;
        QString VideoQualityControlType;
        int     ConstantBitRate;
        int     FixedQuality;
        int     FrameRate;
        QString SnapShotImageType;
        int     GovLength;
    }VideoEncodingParameter;
    void getVideoEncodingParameter(QString SessionID, int Channel, int StreamType);
    void setVideoEncodingParameter(QString SessionID, const VideoEncodingParameter &param);

    void getAudioEncodingCapability(QString SessionID);

    typedef struct AudioEncodingParameter{
        int     Enabled;
        QString Encoding;
        int     Bitrate;
        int     SampleRate;
    }AudioEncodingParameter;
    void getAudioEncodingParameter(QString SessionID);
    void setAudioEncodingParameter(QString SessionID, const AudioEncodingParameter &param);

    typedef struct ROIParameter{
        int AreaID;
        int Enabled;
        int PosX;
        int PosY;
        int Width;
        int Height;
    }ROIParameter;
    //ROI：Region of interest
    void getROIParameter(QString SessionID);
    void setROIParameter(QString SessionID, int enabled, int ROIMode, const ROIParameter &param);

    typedef struct PrivacyMaskParameter{
        int Enabled;
        int PosX;
        int PosY;
        int Width;
        int Height;
    }PrivacyMaskParameter;
    void getPrivacyMaskParameter(QString SessionID);
    void setPrivacyMaskParameter(QString SessionID, const PrivacyMaskParameter *param);

    typedef struct OSDParameter{
        int     OSDType;        //0: Channel name；1: Date and time；2:Stream size and frame rate；3: Show focus
        int     Enabled;        //0: Disabled；1: Enabled
        int     DateFormat;     //0: YYYY-MM-DD;1: MM-DD-YYYY;2: DD-MM-YYYY
        QString OSDText;        //OSD text
        int     x;              //Position coordinates(0-1000)
        int     y;              //Position coordinates(0-1000)
        int     FontSize;       //Font size
        QString FontColor;      //Font color (#000000 - #FFFFFF)
    }OSDParameter;
    void getOSDParameter(QString SessionID);
    void setOSDParameter(QString SessionID,
                         const OSDParameter &param1,
                         const OSDParameter &param2,
                         const OSDParameter &param3,
                         const OSDParameter &param4);

    /*********************Network*********************/

    typedef struct BasicParameterIPV4{
        QString IpAddr;//IP地址
        QString SubnetMask;//子网掩码
        QString Gateway;//网关
        QString DNS1;
        QString DNS2;
    }BasicParameterIPV4;
    typedef struct BasicParameterIPV6{
        QString IpAddr;
        QString Gateway;
        QString DNS1;
        QString DNS2;
        int     Ipv6PrefixLength;
    }BasicParameterIPV6;
    typedef struct BasicParameter{
        int MaxLink;
        int DHCP;
        QString Name;
        BasicParameterIPV4 ipv4;
        BasicParameterIPV6 ipv6;
        QString MACAddress;
    }BasicParameter;
    void getBasicParameter(QString SessionID);
    void setBasicParameter(QString SessionID, const BasicParameter &param);

    typedef struct EmailParameter{
        int     Enabled;
        QString MotionAlarmTitle;
        QString SensorAlarmTitle;
        QString VideoBlindAlarmTitle;
        QString VideoLossAlarmTitle;
        QString SmtpServer;
        QString SmtpUser;
        QString SmtpPassword;
        QString Sender;
        QString Receiver_1;
        QString Receiver_2;
        QString Receiver_3;
        QString Receiver_4;
        int     SmtpPort;
    }EmailParameter;
    void getEmailParameter(QString SessionID);
    void setEmailParameter(QString SessionID, const EmailParameter &param);

    typedef struct FTPParameter{
        int     Enabled;
        int     FTPMode;
        QString FTPServer;
        QString FTPUser;
        QString FTPPassword;
        QString UploadDirectory;
        int     FTPPort;
    }FTPParameter;
    void getFTPParameter(QString SessionID);
    void setFTPParameter(QString SessionID, const FTPParameter &param);

    typedef struct DDNSParameter{
        int     Enabled;
        QString DDNSType;
        QString DDNSServerName;
        QString DDNSName;
        QString DDNSUser;
        QString DDNSPassword;
    }DDNSParameter;
    void getDDNSParameter(QString SessionID);
    void setDDNSParameter(QString SessionID, const DDNSParameter &param);

    //PPPoE(Pointto Point Protocol over Ethernet,基于以太网的点对点协议)类似拨号上网
    typedef struct PPPOEParameter{
        int     Enabled;
        QString PPPOEName;
        QString PPPOEPassword;
    }PPPOEParameter;
    void getPPPOEParameter(QString SessionID);
    void setPPPOEParameter(QString SessionID, const PPPOEParameter &param);

    typedef struct SNMPParameter{
        int     EnabledVer1;
        int     EnabledVer2;
        QString ReadPublic;
        QString WritePublic;
        QString TrapAddress;
        int     TrapPort;
        int     SnmpPort;
    }SNMPParameter;
    void getSNMPParameter(QString SessionID);
    void setSNMPParameter(QString SessionID, const SNMPParameter &param);

    typedef struct P2PParameter{
        int     Enabled;
        QString P2PUUID;
    }P2PParameter;
    void getP2PParameter(QString SessionID);
    void setP2PParameter(QString SessionID, const P2PParameter &param);

    typedef struct BonjourParameter{
        int     Enabled;
        QString Name;
    }BonjourParameter;
    void getBonjourParameter(QString SessionID);
    void setBonjourParameter(QString SessionID, const BonjourParameter &param);

    typedef struct HTTPsParameter{
        int Enabled;
        int HTTPsPort;
    }HTTPsParameter;
    void getHTTPsParameter(QString SessionID);
    void setHTTPsParameter(QString SessionID, const HTTPsParameter &param);

    typedef struct UPNPParameter{
        int     Enabled;
    }UPNPParameter;
    void getUPNPParameter(QString SessionID);
    void setUPNPParameter(QString SessionID, const UPNPParameter &param);

    typedef struct OtherBasicParameter{
        int     ServerType;
        int     Enabled;
        int     Port;
    }OtherBasicParameter;
    typedef struct OtherParameter{
        OtherBasicParameter type1;
        OtherBasicParameter type2;
        OtherBasicParameter type3;
    }OtherParameter;
    void getOtherParameter(QString SessionID);
    void setOtherParameter(QString SessionID, const OtherParameter &param);

    /*********************Storage*********************/

    typedef struct Plan{
        int     PlanTimeEnabled;
        QTime   BeginTime;
        QTime   EndTime;
    }Plan;
    typedef struct RemoteRecordingPlan{
        int     Enabled;
        Plan    **Plans;
        QMap<int, int> weeksStateMap;
    }RemoteRecordingPlan;
    void getRemoteRecordingPlan(QString SessionID); //远端记录计划
    void setRemoteRecordingPlan(QString SessionID, const RemoteRecordingPlan &param);

    typedef struct SDCardStatus{
        int     TotoalSizeKByte;
        int     UsedKByte;
        int     AvailableKByte;
    }SDCardStatus;
    void getSDCardStatusQuery(QString SessionID); //询问SD卡的状态

    void setSDCardOperation(QString SessionID, int OperType); //格式化SD卡

    typedef struct SDStorageParameter{
        int     OperType;
        int     RecordSelect;
        int     RecordMode;
        int     RecordTime;
    }SDStorageParameter;
    void getSDStorageParameter(QString SessionID);
    void setSDStorageParameter(QString SessionID, const SDStorageParameter &param);

    typedef struct SnapshotPlanParameter{
        int     Enabled;
        int     SnapIntervalTime;
        Plan    **Plans;
        QMap<int, int> weeksStateMap;
    }SnapshotPlanParameter;
    void getSnapshotPlanParameter(QString SessionID);
    void setSnapshotPlanParameter(QString SessionID, const SnapshotPlanParameter &param);

    /*********************Event*********************/

    typedef struct MotionDetectionParameter{
        int     Enabled;
        int     AlarmOutput;
        int     VideoOutput;
        int     AlarmDuration;
        int     AlarmThreshold;
        int     PreRecTime;
        int     DelayRecTime;
        int     Sensitivity;
        Plan    **Plans;
        QString AreaMask;
        QMap<int, int> weeksStateMap;
        bool    onlyRegion;
    }MotionDetectionParameter;
    void getMotionDetectionParameter(QString SessionID);//运动检测
    void setMotionDetectionParameter(QString SessionID, const MotionDetectionParameter &param);

    typedef struct SensorAlarmParameter{
        int     Enabled;
        int     AlarmOutput;
        int     VideoOutput;
        int     AlarmDuration;
        int     SensorType;
        Plan    **Plans;
        QMap<int, int> weeksStateMap;
    }SensorAlarmParameter;
    void getSensorAlarmParameter(QString SessionID);//传感器报警
    void setSensorAlarmParameter(QString SessionID, const SensorAlarmParameter &param);

    typedef struct VideoBlindAlarmParameter{
        int     Enabled;
        int     AlarmOutput;
        int     VideoOutput;
        int     AlarmDuration;
        int     Sensitivity;
        Plan    **Plans;
        QMap<int, int> weeksStateMap;
    }VideoBlindAlarmParameter;
    void getVideoBlindAlarmParameter(QString SessionID);//视频盲报警
    void setVideoBlindAlarmParameter(QString SessionID, const VideoBlindAlarmParameter &param);

    typedef struct AlarmParameter{
        QString AlarmType;
        QTime TriggeredTime;
    }AlarmParameter;
    typedef struct PullMessage{
        int Channel;
        int MotionAlarmEnabled;
        int SensorAlarmEnabled;
        int VideoBlindAlarmEnabled;
        int AlarmCount;
        AlarmParameter *msgs;
    }PullMessage;
    void getPullAlarmRequest(QString SessionID, int channel);//获取警报

    void getMotionDetectionChanged(QString SessionID, int MotionArea);

    typedef struct ImageParameter{
        int     Mirror;
        int     Turn;
        int     VisionMode;
        int     ExposureMode;
        int     Shutter;
        int     AntiFlashMode;
        int     BLCMode;
        int     WDRIntensity;
        int     BLCIntensity;
        int     HLCIntensity;
        int     DWDRIntensity;
        int     IrcutFilterMode;
        int     HighLowLevel;
        QString BeginTime;
        QString EndTime;
        int     NoiseReduceMode;
        int     LDCEnabled;
        int     BrightnessLevel;
        int     ContrastLevel;
        int     SaturationLevel;
        int     HueLevel;
        int     Sharpness;
    }ImageParameter;
    void getImageParameter(QString SessionID);
    void setImageParameter(QString SessionID, const ImageParameter &param);

    /*********************System Settings*********************/

    typedef struct NTPParameter{
        QString TZ;             //GMT+08:00
        int     IsUpdateTime;
        int     Enabled;
        QString UTCDateTime;    //1970-01-01T00:01:31Z
        QString NTPServer;      //time.windows.com
    }NTPParameter;
    void getNTP(QString SessionID);
    void setNTP(QString SessionID, const NTPParameter &param);

    void setRestoreDefaultParameter(QString SessionID, int Enabled);
    void setDeviceReboot(QString SessionID, int Enabled);
    void setFormat(QString SessionID, int param);

    void PTZControl(QString SessionID, int Zoom, int Focus);

    void getLPRSystemVersion(QString SessionID);

    typedef struct AreaPoint{
        int     AreaID;
        QPoint  TopLeft;
        QPoint  TopRight;
        QPoint  BottomRight;
        QPoint  BottomLeft;
    }AreaPoint;
    typedef struct LPRParameter{
        int Enabled;
        int MinPlateWidth;
        int MaxPlateWidth;
        int MinSymbolCount;
        int MaxSymbolCount;
        int TTLMilliSeconds;
        int ResultDelayMilliSeconds;
    }LPRParameter;
    void getLPRParameter(QString SessionID);
    void setLPRParameter(QString SessionID, const LPRParameter &param1, const AreaPoint &param2);

    void resetLPRParameter(QString SessionID, int Enabled);

    void getInformationOfLatestRecognizedPlates(QString SessionID);

    /*********************Playback*********************/
    typedef struct TimeParameter{
        QTime StarTime;
        QTime EndTime;
        int RecordType;
    }TimeParameter;
    typedef struct BackUpQueryParameter{
        int Type;
        QDate Date;
        QStringList fileList;
        QMap<int, int>    *MonthMap;
        QMap<int, TimeParameter> *TimeParamMap;
    }BackUpQueryParameter;
    void backUpQuery(QString SessionID, const BackUpQueryParameter &param);
    typedef struct StartPlayingParameter{
        int htmlid;
        int playing;
        QDateTime Time;
    }StartPlayingParameter;
    void setRecordStartPlayingTime(QString SessionID, const StartPlayingParameter &param);
    typedef struct PlayStateParameter{
        int htmlid;
        int StateValue;
    }PlayStateParameter;
    void setFastOrSlowPlayState(QString SessionID, const PlayStateParameter &param);

    void deviceDiscover();

signals:
    void signalSendData(int type, QByteArray data);

public slots:
    void handlerTimeout();
    void handlerPrePare(QNetworkRequest &request, QString RequestBody);
    void handlerReply(QNetworkReply *reply);
    void handlerSetParameter(int type, void *param, QString SessionID);
    void handlerGetParameter(int type, int StreamType, int Channel, QString SessionID);

private:    
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QString urlPrefix;
    QString targetHost;
    QString targetPort;
    State currentState;
    int currentType;
};


class ReplyTimeout : public QObject {
    Q_OBJECT
public:
    ReplyTimeout(QNetworkReply *reply, const int timeout) : QObject(reply) {
        Q_ASSERT(reply);
        if (reply && reply->isRunning()) {  // 启动单次定时器
            QTimer::singleShot(timeout, this, SLOT(onTimeout()));
        }
    }

signals:
    void timeout();  // 超时信号

private slots:
    void onTimeout() {  // 处理超时
        QNetworkReply *reply = static_cast<QNetworkReply*>(parent());
        if (reply->isRunning()) {
            qDebug() << "Network connect timeout...";
            reply->abort();
            reply->deleteLater();
            emit timeout();
        }
    }
};
#endif // VIDICONPROTOCOL_H
