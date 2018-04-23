/**************************************************************************
 *  Copyright: Sunniwell
 *  Author: xiaobin
 *  Date: 2018-03-28
 *  Description: 使用HTTP与摄像机进行通信，对数据的打包和转发
 *************************************************************************/

#ifndef VIDICONPROTOCOL_H
#define VIDICONPROTOCOL_H

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QByteArray>
#include <QString>
#include <QPoint>
#include <QTime>
#include <QTimer>
#include <QDebug>
#include <QMutex>

#include "operationinterface_global.h"

#define TIMEOUTMSEC 3000

struct ResponseStatus {
    QString RequestURL;
    int     StatusCode;
    QString StatusString;
};
Q_DECLARE_METATYPE(ResponseStatus)

struct DeviceInfo {
    QString DeviceName;
    int DeviceID;
    QString DeviceModel;
    QString SoftwareVer;
    QString FactoryInfo;
};
Q_DECLARE_METATYPE(DeviceInfo)

struct VideoBasic {
    int     Channel;
    int     StreamType;
};
Q_DECLARE_METATYPE(VideoBasic)

struct VideoEncodingParameter {
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
};
Q_DECLARE_METATYPE(VideoEncodingParameter)

struct AudioEncodingParameter {
    int     Enabled;
    QString Encoding;
    int     Bitrate;
    int     SampleRate;
};
Q_DECLARE_METATYPE(AudioEncodingParameter)

struct ROIParameter {
    int AreaID;
    int Enabled;
    int PosX;
    int PosY;
    int Width;
    int Height;
};
Q_DECLARE_METATYPE(ROIParameter)

struct PrivacyMaskParameter {
    int Enabled;
    int PosX;
    int PosY;
    int Width;
    int Height;
};
Q_DECLARE_METATYPE(PrivacyMaskParameter)

struct OSDParameter {
    int     OSDType;        //0: Channel name；1: Date and time；2:Stream size and frame rate；3: Show focus
    int     Enabled;        //0: Disabled；1: Enabled
    QString OSDText;        //OSD text
    int     x;              //Position coordinates(0-1000)
    int     y;              //Position coordinates(0-1000)
    int     DateFormat;     //0: YYYY-MM-DD;1: MM-DD-YYYY;2: DD-MM-YYYY
    int     FontSize;       //Font size
    QString FontColor;      //Font color (#000000 - #FFFFFF)
};
Q_DECLARE_METATYPE(OSDParameter)

struct BasicParameterIPV4 {
    QString IpAddr;//IP地址
    QString SubnetMask;//子网掩码
    QString Gateway;//网关
    QString DNS1;
    QString DNS2;
};
Q_DECLARE_METATYPE(BasicParameterIPV4)

struct BasicParameterIPV6 {
    QString IpAddr;
    QString Gateway;
    QString DNS1;
    QString DNS2;
    int     Ipv6PrefixLength;
};
Q_DECLARE_METATYPE(BasicParameterIPV6)

struct BasicParameter {
    int MaxLink;
    int DHCP;
    QString Name;
    BasicParameterIPV4 ipv4;
    BasicParameterIPV6 ipv6;
    QString MACAddress;
};
Q_DECLARE_METATYPE(BasicParameter)

struct EmailParameter {
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
};
Q_DECLARE_METATYPE(EmailParameter)

struct FTPParameter {
    int     Enabled;
    int     FTPMode;
    QString FTPServer;
    QString FTPUser;
    QString FTPPassword;
    QString UploadDirectory;
    int     FTPPort;
};
Q_DECLARE_METATYPE(FTPParameter)

struct DDNSParameter {
    int     Enabled;
    QString DDNSType;
    QString DDNSServerName;
    QString DDNSName;
    QString DDNSUser;
    QString DDNSPassword;
};
Q_DECLARE_METATYPE(DDNSParameter)

struct PPPOEParameter {
    int     Enabled;
    QString PPPOEName;
    QString PPPOEPassword;
};
Q_DECLARE_METATYPE(PPPOEParameter)

struct SNMPParameter {
    int     EnabledVer1;
    int     EnabledVer2;
    QString ReadPublic;
    QString WritePublic;
    QString TrapAddress;
    int     TrapPort;
    int     SnmpPort;
};
Q_DECLARE_METATYPE(SNMPParameter)

struct P2PParameter {
    int     Enabled;
    QString P2PUUID;
};
Q_DECLARE_METATYPE(P2PParameter)

struct BonjourParameter {
    int     Enabled;
    QString Name;
};
Q_DECLARE_METATYPE(BonjourParameter)

struct HTTPsParameter {
    int Enabled;
    int HTTPsPort;
};
Q_DECLARE_METATYPE(HTTPsParameter)

struct UPNPParameter {
    int     Enabled;
};
Q_DECLARE_METATYPE(UPNPParameter)

struct OtherBasicParameter {
    int     ServerType;
    int     Enabled;
    int     Port;
};
Q_DECLARE_METATYPE(OtherBasicParameter)

struct OtherParameter {
    OtherBasicParameter type1;
    OtherBasicParameter type2;
    OtherBasicParameter type3;
};
Q_DECLARE_METATYPE(OtherParameter)

struct PlanBasic {
    int     PlanTimeEnabled;
    QTime   BeginTime;
    QTime   EndTime;
};
Q_DECLARE_METATYPE(PlanBasic)

struct RemoteRecordingPlan {
    int     Enabled;
    PlanBasic    **Plans;
    QMap<int, int> weeksStateMap;
};
Q_DECLARE_METATYPE(RemoteRecordingPlan)

struct SDCardStatus {
    int     TotoalSizeKByte;
    int     UsedKByte;
    int     AvailableKByte;
};
Q_DECLARE_METATYPE(SDCardStatus)

struct SDStorageParameter {
    int     OperType;
    int     RecordSelect;
    int     RecordMode;
    int     RecordTime;
};
Q_DECLARE_METATYPE(SDStorageParameter)

struct SnapshotPlanParameter {
    int     Enabled;
    int     SnapIntervalTime;
    PlanBasic    **Plans;
    QMap<int, int> weeksStateMap;
};
Q_DECLARE_METATYPE(SnapshotPlanParameter)

struct MotionDetectionParameter {
    int     Enabled;
    int     AlarmOutput;
    int     VideoOutput;
    int     AlarmDuration;
    int     AlarmThreshold;
    int     PreRecTime;
    int     DelayRecTime;
    int     Sensitivity;
    PlanBasic    **Plans;
    QString AreaMask;
    QMap<int, int> weeksStateMap;
    bool    onlyRegion;
};
Q_DECLARE_METATYPE(MotionDetectionParameter)

struct SensorAlarmParameter {
    int     Enabled;
    int     AlarmOutput;
    int     VideoOutput;
    int     AlarmDuration;
    int     SensorType;
    PlanBasic    **Plans;
    QMap<int, int> weeksStateMap;
};
Q_DECLARE_METATYPE(SensorAlarmParameter)

struct VideoBlindAlarmParameter {
    int     Enabled;
    int     AlarmOutput;
    int     VideoOutput;
    int     AlarmDuration;
    int     Sensitivity;
    PlanBasic    **Plans;
    QMap<int, int> weeksStateMap;
};
Q_DECLARE_METATYPE(VideoBlindAlarmParameter)

struct AlarmParameter {
    QString AlarmType;
    QTime TriggeredTime;
};
Q_DECLARE_METATYPE(AlarmParameter)

struct PullMessage {
    int Channel;
    int MotionAlarmEnabled;
    int SensorAlarmEnabled;
    int VideoBlindAlarmEnabled;
    int AlarmCount;
    AlarmParameter *msgs;
};
Q_DECLARE_METATYPE(PullMessage)

struct ImageParameter {
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
    QTime   BeginTime;
    QTime   EndTime;
    int     NoiseReduceMode;
    int     LDCEnabled;
    int     BrightnessLevel;
    int     ContrastLevel;
    int     SaturationLevel;
    int     HueLevel;
    int     Sharpness;
};
Q_DECLARE_METATYPE(ImageParameter)

struct NTPParameter {
    QString TZ;             //GMT+08:00
    int     IsUpdateTime;
    int     Enabled;
    QString UTCDateTime;    //1970-01-01T00:01:31Z
    QString NTPServer;      //time.windows.com
};
Q_DECLARE_METATYPE(NTPParameter)

struct AreaPoint {
    int     AreaID;
    QPoint  TopLeft;
    QPoint  TopRight;
    QPoint  BottomRight;
    QPoint  BottomLeft;
};
Q_DECLARE_METATYPE(AreaPoint)

struct LPRParameter {
    int Enabled;
    int MinPlateWidth;
    int MaxPlateWidth;
    int MinSymbolCount;
    int MaxSymbolCount;
    int TTLMilliSeconds;
    int ResultDelayMilliSeconds;
};
Q_DECLARE_METATYPE(LPRParameter)

struct TimeParameter{
    QTime StarTime;
    QTime EndTime;
    int RecordType;
};
Q_DECLARE_METATYPE(TimeParameter)

struct BackUpQueryParameter{
    int Type;
    QDate Date;
    QStringList fileList;
    QMap<int, int>    *MonthMap;
    QMap<int, TimeParameter> TimeParamMap;
};
Q_DECLARE_METATYPE(BackUpQueryParameter)

struct UserConfigInfo {
    QString UserName;
    QString PassWord;
    int Privilege;
};
Q_DECLARE_METATYPE(UserConfigInfo)

struct StartPlayingParameter {
    int htmlid;
    int playing;
    QDateTime Time;
};
Q_DECLARE_METATYPE(StartPlayingParameter)

struct PlayStateParameter {
    int htmlid;
    int StateValue;
};
Q_DECLARE_METATYPE(PlayStateParameter)

struct WifiConfigParameter {
    QString ssid;
    QString passwd;
};
Q_DECLARE_METATYPE(WifiConfigParameter)

struct PlayingTimeParameter {
    int htmlid;
    int Playend;
};
Q_DECLARE_METATYPE(PlayingTimeParameter)

class ReplyTimeout;

class OPERATIONINTERFACESHARED_EXPORT NetworkAccessManager : public QNetworkAccessManager
{
public:
    explicit NetworkAccessManager(QObject *parent = NULL) : QNetworkAccessManager(parent) {}

    QNetworkReply *post(const QNetworkRequest &request, const QByteArray &data);
    QNetworkReply *put(const QNetworkRequest &request, const QByteArray &data);

    QString errorMsg() const { return m_errorMsg; }
    void resetErrorMsg() { m_errorMsg.clear(); }

private:
    QString m_errorMsg;
};

class OPERATIONINTERFACESHARED_EXPORT VidiconProtocol : public QObject
{
    Q_OBJECT

public:    
    enum Type {
        NONE,
        VIDEOENCODING,
        AUDIOENCODING,
        OSD,
        NTP,
        GETDEVICEINFO,
        PRIVACY,
        IMAGE,
        TCPIP,
        OTHER,
        PPPOE,
        DDNS,
        EMAIL,
        FTP,
        BONJOUR,
        SNMP,
        UPNP,
        HTTPS,
        P2P,
        RTSP,
        MOTION,
        BLIND,
        SENSOR,
        SCHEDULE,
        SDSTORAGE,
        SNAPSHOT,
        DESTINATION,
        NAS,
        SDCARD,
        FORMATSDCARD,
        REBOOT,
        RECOVEDEFAULT,
        QUERYVIDEONAMEDAY,
        QUERYVIDEOTIMEDAY,
        QUERYFILEMONTH,
        QUERYPICTURENAMEDAY,
        BACKQUERY,
        PULLMESSAGE,
        STARTPLAYING,
        PLAYSTATE,
        LOGIN,
        DOWNLOAD,
        CURRENTPLAYINGTIME,
        USERCONFIG,
        ADDUSER,
        DELETEUSER,
        LOGOUT,
        WIFI,

        RESPONSESTATUS,
        NETWORKERROR,
    };
    Q_ENUM(Type)

    static VidiconProtocol *getInstance()
    {
        if(NULL == s_instance){
            s_instance = new VidiconProtocol();
        }
        return s_instance;
    }
    ~VidiconProtocol();

    inline QString urlPrefix()
    { return QString("http://%1:%2").arg(m_host, m_port); }

    void setHost(QString host)
    { m_host = host; }

    void setPort(QString port)
    { m_port = port; }

private:
    static VidiconProtocol *s_instance;
    VidiconProtocol(QObject *parent = 0);

    void getDeviceInfomation(QString SessionID);

    /*********************Login OR Logout*********************/
    Q_INVOKABLE void login(QString user, QString passwd);
    Q_INVOKABLE void logout(QString SessionID);

    /*********************Media Settings*********************/

    void getVideoEncodingOption(QString SessionID, const VideoBasic &param);

    void getVideoEncodingParameter(QString SessionID, const VideoBasic &param);
    void setVideoEncodingParameter(QString SessionID, const VideoEncodingParameter &param);

    void getAudioEncodingCapability(QString SessionID);

    void getAudioEncodingParameter(QString SessionID);
    void setAudioEncodingParameter(QString SessionID, const AudioEncodingParameter &param);

    void getROIParameter(QString SessionID);
    void setROIParameter(QString SessionID, int enabled, int ROIMode, const ROIParameter &param);

    void getPrivacyMaskParameter(QString SessionID);
    void setPrivacyMaskParameter(QString SessionID, QList<PrivacyMaskParameter> param);

    void getOSDParameter(QString SessionID);
    void setOSDParameter(QString SessionID, const QList<OSDParameter> param);

    /*********************Network*********************/

    void getBasicParameter(QString SessionID);
    void setBasicParameter(QString SessionID, const BasicParameter &param);

    void getEmailParameter(QString SessionID);
    void setEmailParameter(QString SessionID, const EmailParameter &param);

    void getFTPParameter(QString SessionID);
    void setFTPParameter(QString SessionID, const FTPParameter &param);

    void getDDNSParameter(QString SessionID);
    void setDDNSParameter(QString SessionID, const DDNSParameter &param);

    void getPPPOEParameter(QString SessionID);
    void setPPPOEParameter(QString SessionID, const PPPOEParameter &param);

    void getSNMPParameter(QString SessionID);
    void setSNMPParameter(QString SessionID, const SNMPParameter &param);

    void getP2PParameter(QString SessionID);
    void setP2PParameter(QString SessionID, const P2PParameter &param);

    void getBonjourParameter(QString SessionID);
    void setBonjourParameter(QString SessionID, const BonjourParameter &param);

    void getHTTPsParameter(QString SessionID);
    void setHTTPsParameter(QString SessionID, const HTTPsParameter &param);

    void getUPNPParameter(QString SessionID);
    void setUPNPParameter(QString SessionID, const UPNPParameter &param);

    void getOtherParameter(QString SessionID);
    void setOtherParameter(QString SessionID, const OtherParameter &param);

    /*********************Storage*********************/

    void getRemoteRecordingPlan(QString SessionID);
    void setRemoteRecordingPlan(QString SessionID, const RemoteRecordingPlan &param);

    void getSDCardStatusQuery(QString SessionID);

    void setSDCardOperation(QString SessionID, int OperType);

    void getSDStorageParameter(QString SessionID);
    void setSDStorageParameter(QString SessionID, const SDStorageParameter &param);

    void getSnapshotPlanParameter(QString SessionID);
    void setSnapshotPlanParameter(QString SessionID, const SnapshotPlanParameter &param);

    /*********************Event*********************/

    void getMotionDetectionParameter(QString SessionID);
    void setMotionDetectionParameter(QString SessionID, const MotionDetectionParameter &param);

    void getSensorAlarmParameter(QString SessionID);
    void setSensorAlarmParameter(QString SessionID, const SensorAlarmParameter &param);

    void getVideoBlindAlarmParameter(QString SessionID);
    void setVideoBlindAlarmParameter(QString SessionID, const VideoBlindAlarmParameter &param);

    void getPullAlarmRequest(QString SessionID, int channel);

    void getMotionDetectionChanged(QString SessionID, int MotionArea);

    void getImageParameter(QString SessionID);
    void setImageParameter(QString SessionID, const ImageParameter &param);

    /*********************System Settings*********************/

    void getNTP(QString SessionID);
    void setNTP(QString SessionID, const NTPParameter &param);

    void setRestoreDefaultParameter(QString SessionID, int Enabled);

    void setDeviceReboot(QString SessionID, int Enabled);

    void setFormat(QString SessionID, int param);

    /*********************PTZ*********************/

    void PTZControl(QString SessionID, int Zoom, int Focus);

    /*********************LPR*********************/

    void getLPRSystemVersion(QString SessionID);

    void getLPRParameter(QString SessionID);
    void setLPRParameter(QString SessionID, const LPRParameter &param1, const AreaPoint &param2);

    void resetLPRParameter(QString SessionID, int Enabled);

    void getInformation(QString SessionID);

    /*********************Back Up*********************/

    void backUpQuery(QString SessionID, const BackUpQueryParameter &param);

    void setRecordStartPlayingTime(QString SessionID, const StartPlayingParameter &param);

    void setFastOrSlowPlayState(QString SessionID, const PlayStateParameter &param);

    Q_INVOKABLE void getCurrentPlayingTime(int htmlid, QString SessionID);

    /*********************User Management*********************/

    void getUserConfig(QString SessionID);
    void setUserConfig(QString SessionID, const UserConfigInfo &info);

    void addUser(QString SessionID, const UserConfigInfo &info);
    void delUser(QString SessionID, const UserConfigInfo &info);

    void getWifiConfig(QString SessionID);
    void setWifiConfig(QString SessionID, const WifiConfigParameter &param);

signals:
    void signalReceiveData(VidiconProtocol::Type type, QByteArray data);

public slots:
    void init();
    void handlePrePare(QNetworkRequest &request, QString RequestBody);
    void handleReply(QNetworkReply *reply);
    void handleSetParameter(VidiconProtocol::Type type, const QVariant param, QString SessionID);
    void handleGetParameter(VidiconProtocol::Type type, const QVariant param, QString SessionID);

private:    
    NetworkAccessManager *m_manager;
    QNetworkReply *m_reply;
    QMap<QNetworkReply *, Type>m_replyMap;
    QString m_urlPrefix;
    QString m_host;
    QString m_port;
    Type m_currentType;
    bool m_isBusy;
};

/*处理网络超时的情况*/
class ReplyTimeout : public QObject {
    Q_OBJECT
public:
    ReplyTimeout(QNetworkReply *reply, const int timeout) : QObject(reply) {
        Q_ASSERT(reply);
        if (reply && reply->isRunning()) {
            QTimer::singleShot(timeout, this, SLOT(onTimeout()));
        }
    }

signals:
    void timeout();

private slots:
    void onTimeout() {
        QNetworkReply *reply = static_cast<QNetworkReply*>(parent());
        if (reply->isRunning()) {
            qDebug() << "#ReplyTimeout# Network connect timeout...";
            reply->abort();
            reply->deleteLater();
            emit timeout();
        }
    }
};
#endif // VIDICONPROTOCOL_H
