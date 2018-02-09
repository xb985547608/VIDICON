#include "vidiconprotocol.h"
#include <QUdpSocket>
#include <QApplication>
#include <QThread>
#include <QEventLoop>

VidiconProtocol *VidiconProtocol::_instance = NULL;
VidiconProtocol::VidiconProtocol(QString host, QString port, QObject *parent) : QObject(parent) ,
    reply(NULL), targetHost(host), targetPort(port), currentState(Leisure), currentType(-1)
{
}

void VidiconProtocol::init()
{
    manager = new QNetworkAccessManager(this);
    urlPrefix = QString("http://%1:%2").arg(targetHost, targetPort);

    connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(handlerReply(QNetworkReply *)));
}

VidiconProtocol::~VidiconProtocol()
{

}

void VidiconProtocol::getDeviceInfomation(QString SessionID)
{
    //配置请求行的URL
    QString urlSuffix = QString("/ISAPI/DeviceInfo?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    //配置请求体
    QString requestBody;

    //配置请求头
    handlerPrePare(request, requestBody);
    currentType = DEVICEINFO;
    //发送POST请求
    reply = manager->post(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::login(QString user, QString passwd)
{
    QString urlSuffix = QString("/ISAPI/Security/Login");
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>\
                                <UserCheck>\
                                    <Username>%1</Username>\
                                    <Password>%2</Password>\
                                </UserCheck>").arg(user).arg(passwd));

    handlerPrePare(request, requestBody);
    reply = manager->post(request, requestBody.toLatin1());
    currentType = LOGIN;
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::logout(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/Security/Logout?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>\
                                <UserCheck>\
                                    <SessionID>%1</SessionID>\
                                </UserCheck>").arg(SessionID));

    handlerPrePare(request, requestBody);
    reply = manager->post(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::getVideoEncodingOption(QString SessionID, int Channel, int StreamType)
{
    QString urlSuffix = QString("/ISAPI/VideoInfo/VideoEncodeOptions?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>\
                                <GetVideoEncodeOptions>\
                                    <Channel>%1</Channel>\
                                    <StreamType>%2</StreamType>\
                                </GetVideoEncodeOptions>").arg(Channel).arg(StreamType));

    handlerPrePare(request, requestBody);
    reply = manager->post(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::getVideoEncodingParameter(QString SessionID, int Channel, int StreamType)
{
    QString urlSuffix = QString("/ISAPI/VideoInfo/VideoConfig?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                               "<VideoParam>"
                                    "<Channel>%1</Channel>"
                                    "<StreamType>%2</StreamType>"
                               "</VideoParam>").arg(Channel).arg(StreamType));

    handlerPrePare(request, requestBody);
    currentType = VIDEOENCODINGPARAM;
    reply = manager->post(request, requestBody.toStdString().data());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::setVideoEncodingParameter(QString SessionID, const VideoEncodingParameter &param)
{
    QString urlSuffix = QString("/ISAPI/VideoInfo/VideoConfig?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>\
                                <VideoParam>\
                                    <Channel>%1</Channel>\
                                    <StreamType>%2</StreamType>\
                                    <VideoCodecType>%3</VideoCodecType>\
                                    <VideoResolutionWidth>%4</VideoResolutionWidth>\
                                    <VideoResolutionHeight>%5</VideoResolutionHeight>\
                                    <VideoQualityControlType>%6</VideoQualityControlType>\
                                    <ConstantBitRate>%7</ConstantBitRate>\
                                    <FixedQuality>%8</FixedQuality>\
                                    <FrameRate>%9</FrameRate>\
                                    <SnapShotImageType>%10</SnapShotImageType>\
                                    <GovLength>%11</GovLength>\
                                </VideoParam>").arg(param.Channel)
                                               .arg(param.StreamType)
                                               .arg(param.VideoCodecType)
                                               .arg(param.VideoResolutionWidth)
                                               .arg(param.VideoResolutionHeight)
                                               .arg(param.VideoQualityControlType)
                                               .arg(param.ConstantBitRate)
                                               .arg(param.FixedQuality)
                                               .arg(param.FrameRate)
                                               .arg(param.SnapShotImageType)
                                               .arg(param.GovLength));

    handlerPrePare(request, requestBody);
//    currentType = RESPONSESTATUS;
    reply = manager->put(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::getAudioEncodingCapability(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/AudioInfo/AudioEncodeOptions?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;

    handlerPrePare(request, requestBody);
    reply = manager->post(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::getAudioEncodingParameter(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/AudioInfo/AudioConfig?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;

    handlerPrePare(request, requestBody);
    currentType = AUDIOENCODINGPARAM;
    reply = manager->post(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::setAudioEncodingParameter(QString SessionID, const AudioEncodingParameter &param)
{
    QString urlSuffix = QString("/ISAPI/AudioInfo/AudioConfig?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>\
                                <AudioConfig>\
                                    <Enabled>%1</Enabled>\
                                    <Encoding>%2</Encoding>\
                                    <Bitrate>%3</Bitrate>\
                                    <SampleRate>%4</SampleRate>\
                                </AudioConfig>").arg(param.Enabled)
                                                .arg(param.Encoding)
                                                .arg(param.Bitrate)
                                                .arg(param.SampleRate));

    handlerPrePare(request, requestBody);
    currentType = RESPONSESTATUS;
    reply = manager->put(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::getROIParameter(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/ROI/ROIInfo?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;

    handlerPrePare(request, requestBody);
    reply = manager->post(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::setROIParameter(QString SessionID, int enabled, int ROIMode, const ROIParameter &param)
{
    QString urlSuffix = QString("/ISAPI/ROI/ROIInfo?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>\
                               <ROIParam>\
                               <Enabled>%1</Enabled>\
                               <ROIMode>%2</ROIMode>\
                               <ZoneSetting_0>\
                                   <AreaID>%3</AreaID>\
                                   <Enabled>%4</Enabled>\
                                   <PosX>%5</PosX>\
                                   <PosY>%6</PosY>\
                                   <Width>%7</Width>\
                                   <Height>%8</Height>\
                               </ZoneSetting_0>\
                               </ROIParam>").arg(enabled)
                                            .arg(ROIMode)
                                            .arg(param.AreaID)
                                            .arg(param.Enabled)
                                            .arg(param.PosX)
                                            .arg(param.PosY)
                                            .arg(param.Width)
                                            .arg(param.Height));

    handlerPrePare(request, requestBody);
    currentType = RESPONSESTATUS;
    reply = manager->put(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::getPrivacyMaskParameter(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/PrivacyMask?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;

    handlerPrePare(request, requestBody);
    reply = manager->post(request, requestBody.toLatin1());
}

void VidiconProtocol::setPrivacyMaskParameter(QString SessionID, const PrivacyMaskParameter *param)
{
    QString urlSuffix = QString("/ISAPI/PrivacyMask?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;

    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>\
                                <PrivacyMask>"));
    for(int i=0; i<4; i++) {
        requestBody.append(QString("<ZoneSetting_%6>\
                                        <Enabled>%1</Enabled>\
                                        <PosX>%2</PosX>\
                                        <PosY>%3</PosY>\
                                        <Width>%4</Width>\
                                        <Height>%5</Height>\
                                    </ZoneSetting_%6>").arg(param[i].Enabled)
                                                       .arg(param[i].PosX)
                                                       .arg(param[i].PosY)
                                                       .arg(param[i].Width)
                                                       .arg(param[i].Height)
                                                       .arg(i));
    }

    requestBody.append("</PrivacyMask>");
    handlerPrePare(request, requestBody);
    currentType = RESPONSESTATUS;
    reply = manager->put(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::getOSDParameter(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/OSD/OSDInfo?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;

    handlerPrePare(request, requestBody);
    currentType = OSDPARAMETER;
    reply = manager->post(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::setOSDParameter(QString SessionID,
                                      const OSDParameter &param1,
                                      const OSDParameter &param2,
                                      const OSDParameter &param3,
                                      const OSDParameter &param4)
{
    QString urlSuffix = QString("/ISAPI/OSD/OSDInfo?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?><OSDInfo>"));

    const OSDParameter param[4] = {param1, param2, param3, param4};
    for(int i=0; i<4; i++){
        requestBody.append(QString("<OSDParam_%1>\
                                        <OSDType>%2</OSDType>\
                                        <Position>\
                                            <Enabled>%3</Enabled>\
                                            <OSDText>%4</OSDText>\
                                            <X>%5</X>\
                                            <Y>%6</Y>\
                                        </Position>\
                                        <DateFormat>%7</DateFormat>\
                                        <FontSize>%8</FontSize>\
                                        <FontColor>%9</FontColor>\
                                    </OSDParam_%1>").arg(i).arg(param[i].OSDType)
                                                           .arg(param[i].Enabled)
                                                           .arg(param[i].OSDText)
                                                           .arg(param[i].x)
                                                           .arg(param[i].y)
                                                           .arg(param[i].DateFormat)
                                                           .arg(param[i].FontSize)
                                                           .arg(param[i].FontColor));
    }

    requestBody.append(QString("</OSDInfo>"));
    handlerPrePare(request, requestBody);
    currentType = RESPONSESTATUS;
    reply = manager->put(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::getBasicParameter(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/Network/NetworkConfig?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;

    handlerPrePare(request, requestBody);
    currentType = TCPIPPARAMETER;
    reply = manager->post(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::setBasicParameter(QString SessionID, const BasicParameter &param)
{
    QString urlSuffix = QString("/ISAPI/Network/NetworkConfig?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>\
                               <NetworkConfig>\
                                   <NetLink>\
                                   <MaxLink>%1</MaxLink>\
                                   <DHCP>%2</DHCP>\
                                   <Name>%3</Name>\
                                   <Ipv4>\
                                       <IpAddr>%4</IpAddr>\
                                       <SubnetMask>%5</SubnetMask>\
                                       <Gateway>%6</Gateway>\
                                       <DNS1>%7</DNS1>\
                                       <DNS2>%8</DNS2>\
                                   </Ipv4>\
                                   <Ipv6>\
                                       <IpAddr>%9</IpAddr>\
                                       <Gateway>%10</Gateway>\
                                       <DNS1>%11</DNS1>\
                                       <DNS2>%12</DNS2>\
                                       <Ipv6PrefixLength>%13</Ipv6PrefixLength>\
                                   </Ipv6>\
                                   <MACAddress><%14</MACAddress>\
                                   </NetLink>\
                               </NetworkConfig>").arg(param.MaxLink)
                                                 .arg(param.DHCP)
                                                 .arg(param.Name)
                                                 .arg(param.ipv4.IpAddr)
                                                 .arg(param.ipv4.SubnetMask)
                                                 .arg(param.ipv4.Gateway)
                                                 .arg(param.ipv4.DNS1)
                                                 .arg(param.ipv4.DNS2)
                                                 .arg(param.ipv6.IpAddr)
                                                 .arg(param.ipv6.Gateway)
                                                 .arg(param.ipv6.DNS1)
                                                 .arg(param.ipv6.DNS2)
                                                 .arg(param.ipv6.Ipv6PrefixLength)
                                                 .arg(param.MACAddress));

    handlerPrePare(request, requestBody);
    currentType = RESPONSESTATUS;
    reply = manager->put(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::getEmailParameter(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/Network/Email?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;

    handlerPrePare(request, requestBody);
    currentType = EMAILPARAMETER;
    reply = manager->post(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::setEmailParameter(QString SessionID, const EmailParameter &param)
{
    QString urlSuffix = QString("/ISAPI/Network/Email?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>\
                                <EmailStatus>\
                                    <Enabled>%1</Enabled>\
                                    <MotionAlarmTitle>%2</MotionAlarmTitle>\
                                    <SensorAlarmTitle>%3</SensorAlarmTitle>\
                                    <VideoBlindAlarmTitle>%4</VideoBlindAlarmTitle>\
                                    <VideoLossAlarmTitle>%5</VideoLossAlarmTitle>\
                                    <SmtpServer>%6</SmtpServer>\
                                    <SmtpUser>%7</SmtpUser>\
                                    <SmtpPassword>%8</SmtpPassword>\
                                    <Sender>%9</Sender>\
                                    <Receiver_1>%10</Receiver_1>\
                                    <Receiver_2>%11</Receiver_2>\
                                    <Receiver_3>%12</Receiver_3>\
                                    <Receiver_4>%13</Receiver_4>\
                                    <SmtpPort>%14</SmtpPort>\
                                </EmailStatus>").arg(param.Enabled)
                                                .arg(param.MotionAlarmTitle)
                                                .arg(param.SensorAlarmTitle)
                                                .arg(param.VideoBlindAlarmTitle)
                                                .arg(param.VideoLossAlarmTitle)
                                                .arg(param.SmtpServer)
                                                .arg(param.SmtpUser)
                                                .arg(param.SmtpPassword)
                                                .arg(param.Sender)
                                                .arg(param.Receiver_1)
                                                .arg(param.Receiver_2)
                                                .arg(param.Receiver_3)
                                                .arg(param.Receiver_4)
                                                .arg(param.SmtpPort));

    handlerPrePare(request, requestBody);
    currentType = RESPONSESTATUS;
    reply = manager->put(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::getFTPParameter(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/Network/FTP?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;

    handlerPrePare(request, requestBody);
    currentType = FTPPARAMETER;
    reply = manager->post(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::setFTPParameter(QString SessionID, const FTPParameter &param)
{
    QString urlSuffix = QString("/ISAPI/Network/FTP?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>\
                                <FTPStatus>\
                                    <Enabled>%1</Enabled>\
                                    <FTPMode>%2</FTPMode>\
                                    <FTPServer>%3</FTPServer>\
                                    <FTPUser>%4</FTPUser>\
                                    <FTPPassword>%5</FTPPassword>\
                                    <UploadDirectory>%6</UploadDirectory>\
                                    <FTPPort>%7</FTPPort>\
                                </FTPStatus>").arg(param.Enabled)
                                              .arg(param.FTPMode)
                                              .arg(param.FTPServer)
                                              .arg(param.FTPUser)
                                              .arg(param.FTPPassword)
                                              .arg(param.UploadDirectory)
                                              .arg(param.FTPPort));

    handlerPrePare(request, requestBody);
    currentType = RESPONSESTATUS;
    reply = manager->put(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::getDDNSParameter(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/Network/DDNS?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;

    handlerPrePare(request, requestBody);
    currentType = DDNSPARAMETER;
    reply = manager->post(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::setDDNSParameter(QString SessionID, const DDNSParameter &param)
{
    QString urlSuffix = QString("/ISAPI/Network/DDNS?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>\
                                <DDNSStatus>\
                                   <Enabled>%1</Enabled>\
                                   <DDNSType>%2</DDNSType>\
                                   <DDNSServerName>%3</DDNSServerName>\
                                   <DDNSName>%4</DDNSName>\
                                   <DDNSUser>%5</DDNSUser>\
                                   <DDNSPassword>%6</DDNSPassword>\
                                </DDNSStatus>").arg(param.Enabled)
                                               .arg(param.DDNSType)
                                               .arg(param.DDNSServerName)
                                               .arg(param.DDNSName)
                                               .arg(param.DDNSUser)
                                               .arg(param.DDNSPassword));

    handlerPrePare(request, requestBody);
    currentType = RESPONSESTATUS;
    reply = manager->put(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::getPPPOEParameter(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/Network/PPPOE?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;

    handlerPrePare(request, requestBody);
    currentType = PPPOEPARAMETR;
    reply = manager->post(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::setPPPOEParameter(QString SessionID, const PPPOEParameter &param)
{
    QString urlSuffix = QString("/ISAPI/Network/PPPOE?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>\
                                <PPPOEStatus>\
                                    <Enabled>%1</Enabled>\
                                    <PPPOEName>%2</PPPOEName>\
                                    <PPPOEPassword>%3</PPPOEPassword>\
                                </PPPOEStatus>").arg(param.Enabled)
                                                .arg(param.PPPOEName)
                                                .arg(param.PPPOEPassword));

    handlerPrePare(request, requestBody);
    currentType = RESPONSESTATUS;
    reply = manager->put(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::getSNMPParameter(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/Network/SNMP?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;

    handlerPrePare(request, requestBody);
    currentType = SNMPPARAMETER;
    reply = manager->post(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::setSNMPParameter(QString SessionID, const SNMPParameter &param)
{
    QString urlSuffix = QString("/ISAPI/Network/SNMP?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>\
                                <SNMPStatus>\
                                    <Enabled>\
                                        <Ver1>%1</Ver1>\
                                        <Ver2>%2</Ver2>\
                                    </Enabled>\
                                    <ReadPublic>%3</ReadPublic>\
                                    <WritePublic>%4</WritePublic>\
                                    <TrapAddress>%5</TrapAddress>\
                                    <SnmpPort>%6</SnmpPort>\
                                    <TrapPort>%7</TrapPort>\
                                </SNMPStatus>").arg(param.EnabledVer1)
                                               .arg(param.EnabledVer2)
                                               .arg(param.ReadPublic)
                                               .arg(param.WritePublic)
                                               .arg(param.TrapAddress)
                                               .arg(param.SnmpPort)
                                               .arg(param.TrapPort));

    handlerPrePare(request, requestBody);
    currentType = RESPONSESTATUS;
    reply = manager->put(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::getP2PParameter(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/Network/P2P?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;

    handlerPrePare(request, requestBody);
    currentType = P2PPARAMETER;
    reply = manager->post(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::setP2PParameter(QString SessionID, const P2PParameter &param)
{
    QString urlSuffix = QString("/ISAPI/Network/P2P?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>\
                                <P2PStatus>\
                                    <Enabled>%1</Enabled>\
                                    <P2PUUID></P2PUUID>\
                                </P2PStatus>").arg(param.Enabled));

    handlerPrePare(request, requestBody);
    currentType = RESPONSESTATUS;
    reply = manager->put(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::getBonjourParameter(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/Network/Bonjour?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;

    handlerPrePare(request, requestBody);
    currentType = BONJOURPARAMETER;
    reply = manager->post(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::setBonjourParameter(QString SessionID, const BonjourParameter &param)
{
    QString urlSuffix = QString("/ISAPI/Network/Bonjour?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>\
                                <BonjourStatus>\
                                    <Enabled>%1</Enabled>\
                                    <Name>%2</Name>\
                                </BonjourStatus>").arg(param.Enabled)
                                                  .arg(param.Name));

    handlerPrePare(request, requestBody);
    currentType = RESPONSESTATUS;
    reply = manager->put(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::getHTTPsParameter(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/Network/HTTPs?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;

    handlerPrePare(request, requestBody);
    currentType = HTTPSPARAMETER;
    reply = manager->post(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::setHTTPsParameter(QString SessionID, const HTTPsParameter &param)
{
    QString urlSuffix = QString("/ISAPI/Network/HTTPs?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>\
                                <HTTPsStatus>\
                                    <Enabled>%1</Enabled>\
                                    <HTTPsPort>%2</HTTPsPort>\
                                </HTTPsStatus>").arg(param.Enabled)
                                                .arg(param.HTTPsPort));

    handlerPrePare(request, requestBody);
    currentType = RESPONSESTATUS;
    reply = manager->put(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::getUPNPParameter(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/Network/UPNP?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;

    handlerPrePare(request, requestBody);
    currentType = UPNPPARAMETER;
    reply = manager->post(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::setUPNPParameter(QString SessionID, const UPNPParameter &param)
{
    QString urlSuffix = QString("/ISAPI/Network/UPNP?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>\
                                <UPNPStatus>\
                                    <Enabled>%1</Enabled>\
                                </UPNPStatus>").arg(param.Enabled));

    handlerPrePare(request, requestBody);
    currentType = RESPONSESTATUS;
    reply = manager->put(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::getOtherParameter(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/Network/ExtServerParam?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;

    handlerPrePare(request, requestBody);
    currentType = OTHERPARAMETER;
    reply = manager->post(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::setOtherParameter(QString SessionID, const OtherParameter &param)
{
    QString urlSuffix = QString("/ISAPI/Network/ExtServerParam?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>\
                                <ExtServer>"));
    OtherBasicParameter *param1 = (OtherBasicParameter *)&param;
    for(int i=0; i<3; i++) {
        requestBody.append(QString("<Option_%4>\
                                        <ServerType>%1</ServerType>\
                                        <Enabled>%2</Enabled>\
                                        <Port>%3</Port>\
                                    </Option_%4>").arg(param1->ServerType)
                                                  .arg(param1->Enabled)
                                                  .arg(param1->Port)
                                                  .arg(i));
        param1++;
    }

    requestBody.append("</ExtServer>");
    handlerPrePare(request, requestBody);
    currentType = RESPONSESTATUS;
    reply = manager->put(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::getRemoteRecordingPlan(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/RecFile/VideoPlan?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;

    handlerPrePare(request, requestBody);
    currentType = SCHEDULEPARAMETER;
    reply = manager->post(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::setRemoteRecordingPlan(QString SessionID, const RemoteRecordingPlan &param)
{
    QString urlSuffix = QString("/ISAPI/RecFile/VideoPlan?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>\
                                <RemoteVideoPlan>\
                                <Enabled>%1</Enabled>").arg(param.Enabled));

    for(int i=0; i<7; i++){
        if(param.weeksStateMap[i] != Qt::Checked)
            continue;
        requestBody.append(QString("<WeekDay_%1>").arg(i));
        for(int j=0; j<4; j++){
            requestBody.append(QString("<Section_%1>\
                                            <PlanTimeEnabled>%2</PlanTimeEnabled>\
                                            <BeginTime>%3</BeginTime>\
                                            <EndTime>%4</EndTime>\
                                        </Section_%1>").arg(j)
                                                      .arg(param.Plans[i][j].PlanTimeEnabled)
                                                      .arg(param.Plans[i][j].BeginTime.toString("HH:mm:ss"))
                                                      .arg(param.Plans[i][j].EndTime.toString("HH:mm:ss")));
        }

        requestBody.append(QString("</WeekDay_%1>").arg(i));
    }
    requestBody.append("</RemoteVideoPlan>");

    handlerPrePare(request, requestBody);
    currentType = RESPONSESTATUS;
    reply = manager->put(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::getSDCardStatusQuery(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/Sdcard/QuerySDCardStat?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;

    handlerPrePare(request, requestBody);
    currentType = SDCARDPARAMETER;
    reply = manager->post(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::setSDCardOperation(QString SessionID, int OperType)
{
    QString urlSuffix = QString("/ISAPI/Sdcard/SDOperation?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>\
                                <SDOperation>\
                                    <OperType>%1</OperType>\
                                </SDOperation>").arg(OperType));

    handlerPrePare(request, requestBody);
    currentType = RESPONSESTATUS;
    reply = manager->put(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::getSDStorageParameter(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/Sdcard/SDStorage?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;

    handlerPrePare(request, requestBody);
    currentType = SDSTORAGEPARAMETER;
    reply = manager->post(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::setSDStorageParameter(QString SessionID, const SDStorageParameter &param)
{
    QString urlSuffix = QString("/ISAPI/Sdcard/SDStorage?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>\
                                <SDStorage>\
                                    <OperType>%1</OperType>\
                                   <RecordSelect>%2</RecordSelect>\
                                   <RecordMode>%3</RecordMode>\
                                   <RecordTime>%4</RecordTime>\
                                </SDStorage>").arg(param.OperType)
                                              .arg(param.RecordSelect)
                                              .arg(param.RecordMode)
                                              .arg(param.RecordTime));

    handlerPrePare(request, requestBody);
    currentType = RESPONSESTATUS;
    reply = manager->put(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::getSnapshotPlanParameter(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/Snapshot?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;

    handlerPrePare(request, requestBody);
    currentType = SNAPSHOTPARAMETER;
    reply = manager->post(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::setSnapshotPlanParameter(QString SessionID, const SnapshotPlanParameter &param)
{
    QString urlSuffix = QString("/ISAPI/Snapshot?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>\
                                <SnapshotChannel>\
                                <Enabled>%1</Enabled>\
                                <SnapIntervalTime>%2</SnapIntervalTime>").arg(param.Enabled).arg(param.SnapIntervalTime));

    for(int i=0; i<7; i++){
        if(param.weeksStateMap[i] != Qt::Checked)
            continue;
        requestBody.append(QString("<WeekDay_%1>").arg(i));
        for(int j=0; j<4; j++){
            requestBody.append(QString("<Section_%1>\
                                            <PlanTimeEnabled>%2</PlanTimeEnabled>\
                                            <BeginTime>%3</BeginTime>\
                                            <EndTime>%4</EndTime>\
                                        </Section_%1>").arg(j)
                                                      .arg(param.Plans[i][j].PlanTimeEnabled)
                                                      .arg(param.Plans[i][j].BeginTime.toString("HH:mm:ss"))
                                                      .arg(param.Plans[i][j].EndTime.toString("HH:mm:ss")));
        }

        requestBody.append(QString("</WeekDay_%1>").arg(i));
    }

    requestBody.append(QString("</SnapshotChannel>"));

    handlerPrePare(request, requestBody);
    currentType = RESPONSESTATUS;
    reply = manager->put(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::getMotionDetectionParameter(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/Event/MotionDetectionParam?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;

    handlerPrePare(request, requestBody);
    currentType = MOTIONALARAPARAMETER;
    reply = manager->post(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::setMotionDetectionParameter(QString SessionID, const MotionDetectionParameter &param)
{
    QString urlSuffix = QString("/ISAPI/Event/MotionDetectionParam?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>\
                                <MotionDetectionParam>\
                                    <Enabled>%1</Enabled>\
                                    <AlarmOutput>%2</AlarmOutput>\
                                    <VideoOutput>%3</VideoOutput>\
                                    <AlarmDuration>%4</AlarmDuration>\
                                    <AlarmThreshold>%5</AlarmThreshold>\
                                    <PreRecTime>%6</PreRecTime>\
                                    <DelayRecTime>%7</DelayRecTime>\
                                    <Sensitivity>%8</Sensitivity>").arg(param.Enabled)
                                                                   .arg(param.AlarmOutput)
                                                                   .arg(param.VideoOutput)
                                                                   .arg(param.AlarmDuration)
                                                                   .arg(param.AlarmThreshold)
                                                                   .arg(param.PreRecTime)
                                                                   .arg(param.DelayRecTime)
                                                                   .arg(param.Sensitivity));

    for(int i=0; i<7; i++){
        if(param.weeksStateMap[i] != Qt::Checked || param.onlyRegion)
            continue;
        requestBody.append(QString("<WeekDay_%1>").arg(i));
        for(int j=0; j<4; j++){
            requestBody.append(QString("<Section_%1>\
                                            <PlanTimeEnabled>%2</PlanTimeEnabled>\
                                            <BeginTime>%3</BeginTime>\
                                            <EndTime>%4</EndTime>\
                                        </Section_%1>").arg(j)
                                                       .arg(param.Plans[i][j].PlanTimeEnabled)
                                                       .arg(param.Plans[i][j].BeginTime.toString("HH:mm:ss"))
                                                       .arg(param.Plans[i][j].EndTime.toString("HH:mm:ss")));
        }
        requestBody.append(QString("</WeekDay_%1>").arg(i));
    }

    requestBody.append(QString("<ZoneSetting><AreaMask>%1</AreaMask></ZoneSetting>\
                                </MotionDetectionParam>").arg(param.AreaMask));

    handlerPrePare(request, requestBody);
    currentType = RESPONSESTATUS;
    reply = manager->put(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::getSensorAlarmParameter(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/Event/SensorAlarmParam?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;

    handlerPrePare(request, requestBody);
    currentType = SENSORALARMPARAMETER;
    reply = manager->post(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::setSensorAlarmParameter(QString SessionID, const SensorAlarmParameter &param)
{
    QString urlSuffix = QString("/ISAPI/Event/SensorAlarmParam?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>\
                                <SensorList>\
                                    <Enabled>%1</Enabled>\
                                    <AlarmOutput>%2</AlarmOutput>\
                                    <VideoOutput>%3</VideoOutput>\
                                    <AlarmDuration>%4</AlarmDuration>\
                                    <SensorType>%5</SensorType>").arg(param.Enabled)
                                                                 .arg(param.AlarmOutput)
                                                                 .arg(param.VideoOutput)
                                                                 .arg(param.AlarmDuration)
                                                                 .arg(param.SensorType));

    for(int i=0; i<7; i++){
        if(param.weeksStateMap[i] != Qt::Checked)
            continue;
        requestBody.append(QString("<WeekDay_%1>").arg(i));
        for(int j=0; j<4; j++){
            requestBody.append(QString("<Section_%1>\
                                            <PlanTimeEnabled>%2</PlanTimeEnabled>\
                                            <BeginTime>%3</BeginTime>\
                                            <EndTime>%4</EndTime>\
                                        </Section_%1>").arg(j)
                                                       .arg(param.Plans[i][j].PlanTimeEnabled)
                                                       .arg(param.Plans[i][j].BeginTime.toString("HH:mm:ss"))
                                                       .arg(param.Plans[i][j].EndTime.toString("HH:mm:ss")));
        }

        requestBody.append(QString("</WeekDay_%1>").arg(i));
    }

    requestBody.append(QString("</SensorList>"));

    handlerPrePare(request, requestBody);
    currentType = RESPONSESTATUS;
    reply = manager->put(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::getVideoBlindAlarmParameter(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/Event/VideoBlindAlarmParam?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;

    handlerPrePare(request, requestBody);
    currentType = BLINDALARMPARAMETER;
    reply = manager->post(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::setVideoBlindAlarmParameter(QString SessionID, const VideoBlindAlarmParameter &param)
{
    QString urlSuffix = QString("/ISAPI/Event/VideoBlindAlarmParam?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>\
                                <VideoBlindAlarmParam>\
                                    <Enabled>%1</Enabled>\
                                    <AlarmOutput>%2</AlarmOutput>\
                                    <VideoOutput>%3</VideoOutput>\
                                    <AlarmDuration>%4</AlarmDuration>\
                                    <Sensitivity>%5</Sensitivity>").arg(param.Enabled)
                                                                   .arg(param.AlarmOutput)
                                                                   .arg(param.VideoOutput)
                                                                   .arg(param.AlarmDuration)
                                                                   .arg(param.Sensitivity));

    for(int i=0; i<7; i++){
        if(param.weeksStateMap[i] != Qt::Checked)
            continue;
        requestBody.append(QString("<WeekDay_%1>").arg(i));
        for(int j=0; j<4; j++){
            requestBody.append(QString("<Section_%1>\
                                            <PlanTimeEnabled>%2</PlanTimeEnabled>\
                                            <BeginTime>%3</BeginTime>\
                                            <EndTime>%4</EndTime>\
                                        </Section_%1>").arg(j)
                                                       .arg(param.Plans[i][j].PlanTimeEnabled)
                                                       .arg(param.Plans[i][j].BeginTime.toString("HH:mm:ss"))
                                                       .arg(param.Plans[i][j].EndTime.toString("HH:mm:ss")));
        }

        requestBody.append(QString("</WeekDay_%1>").arg(i));
    }

    requestBody.append(QString("</VideoBlindAlarmParam>"));

    handlerPrePare(request, requestBody);
    currentType = RESPONSESTATUS;
    reply = manager->put(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::getPullAlarmRequest(QString SessionID, int channel)
{
    QString urlSuffix = QString("/ISAPI/Alarm/PullMsg?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>\
                                <PullMsg>\
                                    <Channel>%1</Channel>\
                                </PullMsg>").arg(channel));

    handlerPrePare(request, requestBody);
    currentType = PULLMESSAGE;
    reply = manager->post(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::getMotionDetectionChanged(QString SessionID, int MotionArea)
{
    QString urlSuffix = QString("/ISAPI/Event/MotionChange?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>\
                                <MotionChange>\
                                    <MotionArea>%1</MotionArea>\
                                </MotionChange>").arg(MotionArea));

    handlerPrePare(request, requestBody);
    reply = manager->post(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::getImageParameter(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/Preview/ImageConfig?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;

    handlerPrePare(request, requestBody);
    currentType = IMAGEPARAMETER;
    reply = manager->post(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::setImageParameter(QString SessionID, const ImageParameter &param)
{
    QString urlSuffix = QString("/ISAPI/Preview/ImageConfig?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>\
                                <ImageChannel>\
                                    <ImageFlip>\
                                        <Mirror>%1</Mirror>\
                                        <Turn>%2</Turn>\
                                    </ImageFlip>\
                                    <Vision>\
                                        <VisionMode>%3</VisionMode>\
                                    </Vision>\
                                    <Exposure>\
                                        <ExposureMode>%4</ExposureMode>\
                                        <Shutter>%5</Shutter>\
                                    </Exposure>\
                                    <IrcutFilter>\
                                        <IrcutFilterMode>%6</IrcutFilterMode>\
                                        <HighLowLevel>%7</HighLowLevel>\
                                        <BeginTime>%8</BeginTime>\
                                        <EndTime>%9</EndTime>\
                                    </IrcutFilter>\
                                    <AntiFlash>\
                                        <AntiFlashMode>%10</AntiFlashMode>\
                                    </AntiFlash>\
                                    <BLC>\
                                        <BLCMode>%11</BLCMode>\
                                        <BLCIntensity>%12</BLCIntensity>\
                                        <WDRIntensity>%13</WDRIntensity>\
                                        <HLCIntensity>%14</HLCIntensity>\
                                        <DWDRIntensity>%15</DWDRIntensity>\
                                    </BLC>\
                                    <NoiseReduce>\
                                        <NoiseReduceMode>%16</NoiseReduceMode>\
                                    </NoiseReduce>\
                                    <LDC>\
                                        <LDCEnabled>%17</LDCEnabled>\
                                    </LDC>\
                                    <Color>\
                                        <BrightnessLevel>%18</BrightnessLevel>\
                                        <ContrastLevel>%19</ContrastLevel>\
                                        <SaturationLevel>%20</SaturationLevel>\
                                        <HueLevel>%21</HueLevel>\
                                        <Sharpness>%22</Sharpness>\
                                    </Color>\
                                </ImageChannel>").arg(param.Mirror)
                                                 .arg(param.Turn)
                                                 .arg(param.VisionMode)
                                                 .arg(param.ExposureMode)
                                                 .arg(param.Shutter)
                                                 .arg(param.IrcutFilterMode)
                                                 .arg(param.HighLowLevel)
                                                 .arg(param.BeginTime)
                                                 .arg(param.EndTime)
                                                 .arg(param.AntiFlashMode)
                                                 .arg(param.BLCMode)
                                                 .arg(param.BLCIntensity)
                                                 .arg(param.WDRIntensity)
                                                 .arg(param.HLCIntensity)
                                                 .arg(param.DWDRIntensity)
                                                 .arg(param.NoiseReduceMode)
                                                 .arg(param.LDCEnabled)
                                                 .arg(param.BrightnessLevel)
                                                 .arg(param.ContrastLevel)
                                                 .arg(param.SaturationLevel)
                                                 .arg(param.HueLevel)
                                                 .arg(param.Sharpness));
    handlerPrePare(request, requestBody);
    currentType = RESPONSESTATUS;
    reply = manager->put(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::getNTP(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/System/NTP?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;

    handlerPrePare(request, requestBody);
    currentType = NTPPARAMETER;
    reply = manager->post(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::setNTP(QString SessionID, const NTPParameter &param)
{
    QString urlSuffix = QString("/ISAPI/System/NTP?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>\
                                <NTPStatus>\
                                    <TimeZone>\
                                        <TZ>%1</TZ>\
                                    </TimeZone>\
                                    <IsUpdateTime>%2</IsUpdateTime>\
                                    <Enabled>%3</Enabled>\
                                    <UTCDateTime>%4</UTCDateTime>\
                                    <NTPServer>%5</NTPServer>\
                                </NTPStatus>").arg(param.TZ)
                                              .arg(param.IsUpdateTime)
                                              .arg(param.Enabled)
                                              .arg(param.UTCDateTime)
                                              .arg(param.NTPServer));

    handlerPrePare(request, requestBody);
    currentType = RESPONSESTATUS;
    reply = manager->put(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::setRestoreDefaultParameter(QString SessionID, int Enabled)
{
    QString urlSuffix = QString("/ISAPI/System/RestoreDefault?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;
    requestBody.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\
                        <RestoreDefault>\
                            <Enabled>%1</Enabled>\
                        </RestoreDefault>").arg(Enabled);

    handlerPrePare(request, requestBody);
    reply = manager->put(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::setDeviceReboot(QString SessionID, int Enabled)
{
    QString urlSuffix = QString("/ISAPI/System/Reboot?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;
    requestBody.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\
                        <Reboot>\
                            <Enabled>%1</Enabled>\
                        </Reboot>").arg(Enabled);

    handlerPrePare(request, requestBody);
    currentType = RESPONSESTATUS;
    reply = manager->put(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::setFormat(QString SessionID, int param)
{
    QString urlSuffix = QString("/ISAPI/System/Format?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;
    requestBody.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\
                        <Format>\
                            <Param>%1</Param>\
                        </Format>").arg(param);

    handlerPrePare(request, requestBody);
    currentType = RESPONSESTATUS;
    reply = manager->put(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::PTZControl(QString SessionID, int Zoom, int Focus)
{
    QString urlSuffix = QString("/ISAPI/PTZ/PtzMotorsCtr?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;
    requestBody.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\
                        <PTZMotorsCtr>\
                            <Zoom>%1</Zoom>\
                            <Focus>%2</Focus>\
                        </PTZMotorsCtr>").arg(Zoom).arg(Focus);

    handlerPrePare(request, requestBody);
    reply = manager->put(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::getLPRSystemVersion(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/LPR/LPRVersion?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;

    handlerPrePare(request, requestBody);
    reply = manager->post(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::getLPRParameter(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/LPR/LPRParam?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;

    handlerPrePare(request, requestBody);
    reply = manager->post(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::setLPRParameter(QString SessionID, const VidiconProtocol::LPRParameter &param1, const VidiconProtocol::AreaPoint &param2)
{
    QString urlSuffix = QString("/ISAPI/LPR/LPRParam?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>\
                                <LPRParam>\
                                    <Enabled>%1</Enabled>\
                                    <MinPlateWidth>%2</MinPlateWidth>\
                                    <MaxPlateWidth>%3</MaxPlateWidth>\
                                    <MinSymbolCount>%4</MinSymbolCount>\
                                    <MaxSymbolCount>%5</MaxSymbolCount>\
                                    <TTLMilliSeconds>%6</TTLMilliSeconds>\
                                    <ResultDelayMilliSeconds>%7</ResultDelayMilliSeconds>").arg(param1.Enabled)
                                                                                           .arg(param1.MinPlateWidth)
                                                                                           .arg(param1.MaxPlateWidth)
                                                                                           .arg(param1.MinSymbolCount)
                                                                                           .arg(param1.MaxSymbolCount)
                                                                                           .arg(param1.TTLMilliSeconds)
                                                                                           .arg(param1.ResultDelayMilliSeconds));

    for(int i=0; i<1; i++){
        requestBody.append(QString("<ZoneSetting_%1>").arg(i));

        requestBody.append(QString("<AreaID><!-- req, xs: int --></AreaID>\
                                    <TopLeft>\
                                        <X>%1</X>\
                                        <Y>%2</Y>\
                                    </TopLeft>\
                                    <TopRight>\
                                        <X>%3</X>\
                                        <Y>%4</Y>\
                                    </TopRight>\
                                    <BottomRight>\
                                        <X>%5</X>\
                                        <Y>%6</Y>\
                                    </BottomRight>\
                                    <BottomLeft>\
                                        <X>%7</X>\
                                        <Y>%8</Y>\
                                    </BottomLeft>").arg(param2.TopLeft.x())     .arg(param2.TopLeft.y())
                                                   .arg(param2.TopRight.x())    .arg(param2.TopRight.y())
                                                   .arg(param2.BottomRight.x()) .arg(param2.BottomRight.y())
                                                   .arg(param2.BottomLeft.x())  .arg(param2.BottomLeft.y()));

        requestBody.append(QString("</ZoneSetting_%1>").arg(i));
    }

    requestBody.append(QString("</LPRParam>"));

    handlerPrePare(request, requestBody);
    reply = manager->put(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::resetLPRParameter(QString SessionID, int Enabled)
{
    QString urlSuffix = QString("/ISAPI/LPR/LPRResetParam?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;
    requestBody.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\
                        <LPRResetParam>\
                            <Enabled>%1</Enabled>\
                        </LPRResetParam>").arg(Enabled);

    handlerPrePare(request, requestBody);
    reply = manager->put(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::getInformationOfLatestRecognizedPlates(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/LPR/LPROutput?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;
    requestBody.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\
                        <LPROutput>\
                            <LastCarID>%1</LastCarID>\
                        </LPROutput>").arg("");

    handlerPrePare(request, requestBody);
    reply = manager->post(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::backUpQuery(QString SessionID, const VidiconProtocol::BackUpQueryParameter &param)
{
    QString urlSuffix = QString("/ISAPI/Record/BackUp?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>\
                        <QueryBackUp>\
                            <Type>%1</Type>\
                            <Time>%2</Time>\
                        </QueryBackUp>").arg(param.Type)
                                        .arg(param.Date.toString("yyyy-MM-dd")));

    handlerPrePare(request, requestBody);
    switch(param.Type) {
    case 0: {
        currentType = QUERYVIDEOTIMEDAY;
        break;
    }
    case 1: {
        currentType = QUERYVIDEONAMEDAY;
        break;
    }
    case 2: {
        currentType = QUERYPICTURENAMEDAY;
        break;
    }
    case 6: {
        currentType = QUERYFILEMONTH;
        break;
    }
    }
    reply = manager->put(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::setRecordStartPlayingTime(QString SessionID, const VidiconProtocol::StartPlayingParameter &param)
{
    QString urlSuffix = QString("/ISAPI/Record/TimePos?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>\
                                <TimePos>\
                                    <htmlid>%1</htmlid>\
                                    <playing>%2</playing>\
                                    <Time>%3</Time>\
                                </TimePos>").arg(param.htmlid)
                                   .arg(param.playing)
                                   .arg(param.Time.toString("yyyy-MM-ddTHH:mm:ss")));

    handlerPrePare(request, requestBody);
    currentType = STARTPLAYING;
    reply = manager->put(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::setFastOrSlowPlayState(QString SessionID, const VidiconProtocol::PlayStateParameter &param)
{
    QString urlSuffix = QString("/ISAPI/Record/RecordRunState?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>\
                                <RecordRunState>\
                                    <htmlid>%1</htmlid>\
                                    <StateValue>%2</StateValue>\
                                </RecordRunState>").arg(param.htmlid)
                                                   .arg(param.StateValue));

    handlerPrePare(request, requestBody);
    currentType = PLAYSTATE;
    reply = manager->post(request, requestBody.toLatin1());
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);
    connect(timeout, &ReplyTimeout::timeout, this, &VidiconProtocol::handlerTimeout);
}

void VidiconProtocol::downloadFile(QString fileName)
{
    QString urlSuffix = QString("/record/%1").arg(fileName);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix + urlSuffix));

    handlerPrePare(request, "");
    currentType = DOWNLOAD;
    reply = manager->get(request);
}

void VidiconProtocol::handlerTimeout()
{
}

void VidiconProtocol::handlerPrePare(QNetworkRequest &request, QString RequestBody)
{
    request.setRawHeader(QByteArray("Accept"),           QByteArray("text/plain, */*; q=0.01"));
    request.setRawHeader(QByteArray("X-Requested-With"), QByteArray("XMLHttpRequest"));
    request.setRawHeader(QByteArray("Referer"),          QByteArray(QString("http://%1/BaseInforPage.html").arg(targetHost).toLatin1()));
    request.setRawHeader(QByteArray("Accept-Language"),  QByteArray("zh-CN"));
    request.setRawHeader(QByteArray("Accept-Encoding"),  QByteArray("gzip, deflate"));
    request.setRawHeader(QByteArray("User-Agent"),       QByteArray("Mozilla/5.0 (Windows NT 10.0; WOW64; Trident/7.0; rv:11.0) like Gecko"));
    request.setRawHeader(QByteArray("Host"),             QByteArray(targetHost.toLatin1()));
    request.setRawHeader(QByteArray("Content-Length"),   QByteArray(QString::number(RequestBody.length()).toLatin1()));
    request.setRawHeader(QByteArray("Connection"),       QByteArray("Keep-Alive"));
    request.setRawHeader(QByteArray("Cache-Control"),    QByteArray("no-cache"));
    request.setRawHeader(QByteArray("Content-Type"),     QByteArray("application/xml"));

    while(currentState == Busy) {
        qApp->processEvents();
    }
    currentState = Busy;
}

void VidiconProtocol::handlerReply(QNetworkReply *reply)
{
    if(reply->error() != QNetworkReply::NoError) {
        qDebug() << "#VidiconProtocol# hanndlerReply,"
                 << "StatusCode:" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()
                 << "ErrorType:" << reply->error();
        emit signalSendData(NETWORKERROR, QByteArray());
    }else {
        qDebug("#VidiconProtocol# hanndlerReply, response content start............");
        QByteArray bytes = reply->readAll();
        qDebug() << bytes.toStdString().data();
        if(currentType != -1){
            qDebug() << "#VidiconProtocol# hanndlerReply, send signal ParameterType:" << currentType;
            emit signalSendData(currentType, bytes);

        }
        qDebug("#VidiconProtocol# hanndlerReply, response content end  ............");
    }
    currentState = Leisure;
    currentType = -1;
}


void VidiconProtocol::handlerSetParameter(int type, void *param, QString SessionID)
{
    qDebug() << "#VidiconProtocol# handlerSetParameter type:" << type;
    switch (type) {
    case VIDEOENCODINGPARAM: {
        VideoEncodingParameter *temp = static_cast<VideoEncodingParameter *>(param);
        setVideoEncodingParameter(SessionID, *temp);
        delete temp;
        break;
    }
    case AUDIOENCODINGPARAM: {
        AudioEncodingParameter *temp = static_cast<AudioEncodingParameter *>(param);
        setAudioEncodingParameter(SessionID, *temp);
        delete temp;
        break;
    }
    case OSDPARAMETER: {
        OSDParameter *temp = static_cast<OSDParameter *>(param);
        setOSDParameter(SessionID, temp[0], temp[1], temp[2], temp[3]);
        delete []temp;
        break;
    }
    case NTPPARAMETER: {
        NTPParameter *temp = static_cast<NTPParameter *>(param);
        setNTP(SessionID, *temp);
        delete temp;
        break;
    }
    case PRIVACYPARAMETER: {
        PrivacyMaskParameter *temp = static_cast<PrivacyMaskParameter *>(param);
        setPrivacyMaskParameter(SessionID, temp);
        delete []temp;
        break;
    }
    case IMAGEPARAMETER: {
        ImageParameter *temp = static_cast<ImageParameter *>(param);
        setImageParameter(SessionID, *temp);
        delete temp;
        break;
    }
    case TCPIPPARAMETER: {
        BasicParameter *temp = static_cast<BasicParameter *>(param);
        setBasicParameter(SessionID, *temp);
        delete temp;
        break;
    }
    case OTHERPARAMETER: {
        OtherParameter *temp = static_cast<OtherParameter *>(param);
        setOtherParameter(SessionID, *temp);
        delete temp;
        break;
    }
    case MOTIONALARAPARAMETER: {
        MotionDetectionParameter *temp = static_cast<MotionDetectionParameter *>(param);
        setMotionDetectionParameter(SessionID, *temp);
        delete temp;
        break;
    }
    case BLINDALARMPARAMETER: {
        VideoBlindAlarmParameter *temp = static_cast<VideoBlindAlarmParameter *>(param);
        setVideoBlindAlarmParameter(SessionID, *temp);
        delete temp;
        break;
    }
    case SENSORALARMPARAMETER: {
        SensorAlarmParameter *temp = static_cast<SensorAlarmParameter *>(param);
        setSensorAlarmParameter(SessionID, *temp);
        delete temp;
        break;
    }
    case SCHEDULEPARAMETER: {
        RemoteRecordingPlan *temp = static_cast<RemoteRecordingPlan *>(param);
        setRemoteRecordingPlan(SessionID, *temp);
        delete temp;
        break;
    }
    case SDSTORAGEPARAMETER: {
        SDStorageParameter *temp = static_cast<SDStorageParameter *>(param);
        setSDStorageParameter(SessionID, *temp);
        delete temp;
        break;
    }
    case SNAPSHOTPARAMETER: {
        SnapshotPlanParameter *temp = static_cast<SnapshotPlanParameter *>(param);
        setSnapshotPlanParameter(SessionID, *temp);
        delete temp;
        break;
    }
    case FORMATSDCARD: {
        setSDCardOperation(SessionID, 1);
        break;
    }
    case REBOOT: {
        setDeviceReboot(SessionID, 1);
        break;
    }
    case RECOVEDEFAULT: {
        setRestoreDefaultParameter(SessionID, 1);
        break;
    }
    case BACKQUERY: {
        BackUpQueryParameter *temp = static_cast<BackUpQueryParameter *>(param);
        backUpQuery(SessionID, *temp);
        delete temp;
        break;
    }
    case STARTPLAYING: {
        StartPlayingParameter *temp = static_cast<StartPlayingParameter *>(param);
        setRecordStartPlayingTime(SessionID, *temp);
        delete temp;
        break;
    }
    case PLAYSTATE: {
        PlayStateParameter *temp = static_cast<PlayStateParameter *>(param);
        setFastOrSlowPlayState(SessionID, *temp);
        delete temp;
        break;
    }
    default:
        qDebug() << "#VidiconProtocol# handlerSetParameter ignore signal, type:" << type;
        break;
    }
}

void VidiconProtocol::handlerGetParameter(int type, int StreamType, int Channel, QString SessionID)
{
    qDebug() << "#VidiconProtocol# handlerGetParameter type:" << type;

    switch (type) {
    case VIDEOENCODINGPARAM: {
        getVideoEncodingParameter(SessionID, Channel, StreamType);
        break;
    }
    case AUDIOENCODINGPARAM: {
        getAudioEncodingParameter(SessionID);
        break;
    }
    case IMAGEPARAMETER: {
        getImageParameter(SessionID);
        break;
    }
    case OSDPARAMETER: {
        getOSDParameter(SessionID);
        break;
    }
    case TCPIPPARAMETER: {
        getBasicParameter(SessionID);
        break;
    }
    case OTHERPARAMETER: {
        getOtherParameter(SessionID);
        break;
    }
    case PPPOEPARAMETR: {
        getPPPOEParameter(SessionID);
        break;
    }
    case DDNSPARAMETER: {
        getDDNSParameter(SessionID);
        break;
    }
    case EMAILPARAMETER: {
        getEmailParameter(SessionID);
        break;
    }
    case FTPPARAMETER: {
        getFTPParameter(SessionID);
        break;
    }
    case BONJOURPARAMETER: {
        getBonjourParameter(SessionID);
        break;
    }
    case SNMPPARAMETER: {
        getSNMPParameter(SessionID);
        break;
    }
    case UPNPPARAMETER: {
        getUPNPParameter(SessionID);
        break;
    }
    case HTTPSPARAMETER: {
        getHTTPsParameter(SessionID);
        break;
    }
    case P2PPARAMETER: {
        getP2PParameter(SessionID);
        break;
    }
    case RTSPPARAMETER: {
        getOtherParameter(SessionID);
        break;
    }
    case MOTIONALARAPARAMETER: {
        getMotionDetectionParameter(SessionID);
        break;
    }
    case SENSORALARMPARAMETER: {
        getSensorAlarmParameter(SessionID);
        break;
    }
    case BLINDALARMPARAMETER: {
        getVideoBlindAlarmParameter(SessionID);
        break;
    }
    case SCHEDULEPARAMETER: {
        getRemoteRecordingPlan(SessionID);
        break;
    }
    case SDSTORAGEPARAMETER: {
        getSDStorageParameter(SessionID);
        break;
    }
    case SDCARDPARAMETER: {
        getSDCardStatusQuery(SessionID);
        break;
    }
    case SNAPSHOTPARAMETER: {
        getSnapshotPlanParameter(SessionID);
        break;
    }
    case NTPPARAMETER: {
        getNTP(SessionID);
        break;
    }
    case DEVICEINFO: {
        getDeviceInfomation(SessionID);
        break;
    }
    case PULLMESSAGE: {
        getPullAlarmRequest(SessionID, 0);
        break;
    }
    default:
        break;
    }
}
