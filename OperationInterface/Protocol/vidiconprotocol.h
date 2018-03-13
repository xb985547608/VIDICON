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

#define VIDEOENCODING       1024
#define AUDIOENCODING       1025
#define OSD                 1026
#define NTP                 1027
#define GETDEVICEINFO       1028
#define PRIVACY             1029
#define IMAGE               1030
#define TCPIP               1031
#define OTHER               1032
#define PPPOE               1033
#define DDNS                1034
#define EMAIL               1035
#define FTP                 1036
#define BONJOUR             1037
#define SNMP                1038
#define UPNP                1039
#define HTTPS               1040
#define P2P                 1041
#define RTSP                1042
#define MOTION              1043
#define BLIND               1044
#define SENSOR              1045
#define SCHEDULE            1046
#define SDSTORAGE           1047
#define SNAPSHOT            1048
#define DESTINATION         1049
#define NAS                 1050
#define SDCARD              1051
#define FORMATSDCARD        1052
#define REBOOT              1053
#define RECOVEDEFAULT       1054
#define QUERYVIDEONAMEDAY   1055
#define QUERYVIDEOTIMEDAY   1056
#define QUERYFILEMONTH      1057
#define QUERYPICTURENAMEDAY 1058
#define BACKQUERY           1059
#define PULLMESSAGE         1060
#define STARTPLAYING        1061
#define PLAYSTATE           1062
#define LOGIN               1063
#define DOWNLOAD            1064
#define CURRENTPLAYINGTIME  1065
#define USERCONFIG          1066
#define ADDUSER             1067
#define DELETEUSER          1068

#define RESPONSESTATUS      8888
#define NETWORKERROR        8889

#define TIMEOUTMSEC         2000

class ReplyTimeout;

class OPERATIONINTERFACESHARED_EXPORT NetworkAccessManager : public QNetworkAccessManager
{
public:
    explicit NetworkAccessManager(QObject *parent = NULL) : QNetworkAccessManager(parent) {}

    QNetworkReply *post(const QNetworkRequest &request, const QByteArray &data);
    QNetworkReply *put(const QNetworkRequest &request, const QByteArray &data);

    const QString &getErrorMsg() const { return errorMsg; }
    void resetErrorMsg() { errorMsg.clear(); }

private:
    QString errorMsg;
};

class OPERATIONINTERFACESHARED_EXPORT VidiconProtocol : public QObject
{
    Q_OBJECT
    enum State{
        Leisure,
        Busy
    };
public:    
    static VidiconProtocol *getInstance(QString ipAddr = "", int port = -1)
    {
        if(NULL == _instance){
            _instance = new VidiconProtocol(ipAddr, QString::number(port));
        }
        return _instance;
    }
    ~VidiconProtocol();

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

    //登录、登出操作
    Q_INVOKABLE void login(QString user, QString passwd);
    Q_INVOKABLE void logout(QString SessionID);

    /*********************Media Settings*********************/

    typedef struct VideoEncoding {
        int     Channel;
        int     StreamType;
    } VideoEncoding;
    void getVideoEncodingOption(QString SessionID, const VideoEncoding &param);

    //视频相关参数获取or设置
    typedef struct VideoEncodingParameter {
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
    } VideoEncodingParameter;
    void getVideoEncodingParameter(QString SessionID, const VideoEncoding &param);
    void setVideoEncodingParameter(QString SessionID, const VideoEncodingParameter &param);

    void getAudioEncodingCapability(QString SessionID);

    //音频相关参数获取or设置
    typedef struct AudioEncodingParameter{
        int     Enabled;
        QString Encoding;
        int     Bitrate;
        int     SampleRate;
    }AudioEncodingParameter;
    void getAudioEncodingParameter(QString SessionID);
    void setAudioEncodingParameter(QString SessionID, const AudioEncodingParameter &param);

    //ROI相关参数获取or设置
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

    //隐私区域获取or设置
    typedef struct PrivacyMaskParameter{
        int Enabled;
        int PosX;
        int PosY;
        int Width;
        int Height;
    }PrivacyMaskParameter;
    void getPrivacyMaskParameter(QString SessionID);
    void setPrivacyMaskParameter(QString SessionID, const PrivacyMaskParameter *param);

    //OSD相关参数获取or设置
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
        QMap<int, TimeParameter> TimeParamMap;
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
    typedef struct PlayingTimeParameter{
        int htmlid;
        int Playend;
    }PlayingTimeParameter;
    Q_INVOKABLE void getCurrentPlayingTime(int htmlid, QString SessionID);

    /*********************User Management*********************/

    typedef struct UserConfigInfo {
        QString UserName;
        QString PassWord;
        int Privilege;
    } UserConfigInfo;
    void getUserConfig(QString SessionID);
    void setUserConfig(QString SessionID, const UserConfigInfo &info);

    void addUser(QString SessionID, const UserConfigInfo &info);
    void delUser(QString SessionID, const UserConfigInfo &info);

signals:
    void signalReceiveData(int type, QByteArray data);

public slots:
    void init();
    void handlePrePare(QNetworkRequest &request, QString RequestBody);
    void handleReply(QNetworkReply *reply);
    void handleSetParameter(int type, void *param, QString SessionID);
    void handleGetParameter(int type, void *param, QString SessionID);

private:    
    NetworkAccessManager *manager;
    QNetworkReply *reply;
    QString urlPrefix;
    QString targetHost;
    QString targetPort;
    int currentState;
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
            qDebug() << "#ReplyTimeout# Network connect timeout...";
            reply->abort();
            reply->deleteLater();
            emit timeout();
        }
    }
};
#endif // VIDICONPROTOCOL_H
