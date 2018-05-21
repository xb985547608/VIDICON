#include "vidiconprotocol.h"
#include <QUdpSocket>
#include <QApplication>
#include <QThread>
#include <QEventLoop>

QNetworkReply *NetworkAccessManager::post(const QNetworkRequest &request, const QByteArray &data)
{
    QNetworkReply *reply = QNetworkAccessManager::post(request, data);

    //超时控制
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);

    return reply;
}

QNetworkReply *NetworkAccessManager::put(const QNetworkRequest &request, const QByteArray &data)
{
    QNetworkReply *reply = QNetworkAccessManager::put(request, data);

    //超时控制
    ReplyTimeout *timeout = new ReplyTimeout(reply, TIMEOUTMSEC);

    return reply;
}

VidiconProtocol *VidiconProtocol::s_instance = NULL;
VidiconProtocol::VidiconProtocol(QObject *parent) :
    QObject(parent),
    m_reply(NULL),
    m_host("192.168.0.66"),
    m_port("80"),
    m_currentType(NONE),
    m_isBusy(false)
{
}

void VidiconProtocol::init()
{
    m_manager = new NetworkAccessManager(this);
    m_urlPrefix = QString("http://%1:%2").arg(m_host, m_port);

    connect(m_manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(handleReply(QNetworkReply *)));
}

VidiconProtocol::~VidiconProtocol()
{

}

void VidiconProtocol::getDeviceInfomation(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/DeviceInfo?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;

    handlePrePare(request, requestBody);
    m_currentType = GETDEVICEINFO;
    m_reply = m_manager->post(request, requestBody.toLatin1());
}

void VidiconProtocol::login(QString user, QString passwd)
{
    QString urlSuffix = QString("/ISAPI/Security/Login");
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                                "<UserCheck>"
                                    "<Username>%1</Username>"
                                    "<Password>%2</Password>"
                                "</UserCheck>").arg(user).arg(passwd));

    handlePrePare(request, requestBody);
    m_reply = m_manager->post(request, requestBody.toLatin1());
    m_currentType = LOGIN;
    if (m_reply->isOpen())
        m_replyMap.insert(m_reply, m_currentType);
}

void VidiconProtocol::logout(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/Security/Logout?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                                "<UserCheck>"
                                    "<SessionID>%1</SessionID>"
                                "</UserCheck>").arg(SessionID));

    handlePrePare(request, requestBody);
    m_reply = m_manager->post(request, requestBody.toLatin1());
    m_currentType = LOGOUT;
    if (m_reply->isOpen())
        m_replyMap.insert(m_reply, m_currentType);
}

void VidiconProtocol::getVideoEncodingOption(QString SessionID, const VideoBasic &param)
{
    QString urlSuffix = QString("/ISAPI/VideoInfo/VideoEncodeOptions?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                                "<GetVideoEncodeOptions>"
                                    "<Channel>%1</Channel>"
                                    "<StreamType>%2</StreamType>"
                                "</GetVideoEncodeOptions>")
                       .arg(param.Channel)
                       .arg(param.StreamType));

    handlePrePare(request, requestBody);
    m_reply = m_manager->post(request, requestBody.toLatin1());
}

void VidiconProtocol::getVideoEncodingParameter(QString SessionID, const VideoBasic &param)
{
    QString urlSuffix = QString("/ISAPI/VideoInfo/VideoConfig?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                               "<VideoParam>"
                                    "<Channel>%1</Channel>"
                                    "<StreamType>%2</StreamType>"
                               "</VideoParam>")
                       .arg(param.Channel)
                       .arg(param.StreamType));

    handlePrePare(request, requestBody);
    m_currentType = VIDEOENCODING;
    m_reply = m_manager->post(request, requestBody.toStdString().data());
}

void VidiconProtocol::setVideoEncodingParameter(QString SessionID, const VideoEncodingParameter &param)
{
    QString urlSuffix = QString("/ISAPI/VideoInfo/VideoConfig?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                                "<VideoParam>"
                                    "<Channel>%1</Channel>"
                                    "<StreamType>%2</StreamType>"
                                    "<VideoCodecType>%3</VideoCodecType>"
                                    "<VideoResolutionWidth>%4</VideoResolutionWidth>"
                                    "<VideoResolutionHeight>%5</VideoResolutionHeight>"
                                    "<VideoQualityControlType>%6</VideoQualityControlType>"
                                    "<ConstantBitRate>%7</ConstantBitRate>"
                                    "<FixedQuality>%8</FixedQuality>"
                                    "<FrameRate>%9</FrameRate>"
                                    "<SnapShotImageType>%10</SnapShotImageType>"
                                    "<GovLength>%11</GovLength>"
                                "</VideoParam>")
                       .arg(param.Channel)
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

    handlePrePare(request, requestBody);
//    currentType = RESPONSESTATUS;
    m_reply = m_manager->put(request, requestBody.toLatin1());
}

void VidiconProtocol::getAudioEncodingCapability(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/AudioInfo/AudioEncodeOptions?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;

    handlePrePare(request, requestBody);
    m_reply = m_manager->post(request, requestBody.toLatin1());
}

void VidiconProtocol::getAudioEncodingParameter(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/AudioInfo/AudioConfig?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;

    handlePrePare(request, requestBody);
    m_currentType = AUDIOENCODING;
    m_reply = m_manager->post(request, requestBody.toLatin1());
}

void VidiconProtocol::setAudioEncodingParameter(QString SessionID, const AudioEncodingParameter &param)
{
    QString urlSuffix = QString("/ISAPI/AudioInfo/AudioConfig?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                                "<AudioConfig>"
                                    "<Enabled>%1</Enabled>"
                                    "<Encoding>%2</Encoding>"
                                    "<Bitrate>%3</Bitrate>"
                                    "<SampleRate>%4</SampleRate>"
                                "</AudioConfig>").arg(param.Enabled)
                                                .arg(param.Encoding)
                                                .arg(param.Bitrate)
                                                .arg(param.SampleRate));

    handlePrePare(request, requestBody);
    m_currentType = RESPONSESTATUS;
    m_reply = m_manager->put(request, requestBody.toLatin1());
}

void VidiconProtocol::getROIParameter(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/ROI/ROIInfo?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;

    handlePrePare(request, requestBody);
    m_reply = m_manager->post(request, requestBody.toLatin1());
}

void VidiconProtocol::setROIParameter(QString SessionID, int enabled, int ROIMode, const ROIParameter &param)
{
    QString urlSuffix = QString("/ISAPI/ROI/ROIInfo?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                               "<ROIParam>"
                               "<Enabled>%1</Enabled>"
                               "<ROIMode>%2</ROIMode>"
                               "<ZoneSetting_0>"
                                   "<AreaID>%3</AreaID>"
                                   "<Enabled>%4</Enabled>"
                                   "<PosX>%5</PosX>"
                                   "<PosY>%6</PosY>"
                                   "<Width>%7</Width>"
                                   "<Height>%8</Height>"
                               "</ZoneSetting_0>"
                               "</ROIParam>").arg(enabled)
                                            .arg(ROIMode)
                                            .arg(param.AreaID)
                                            .arg(param.Enabled)
                                            .arg(param.PosX)
                                            .arg(param.PosY)
                                            .arg(param.Width)
                                            .arg(param.Height));

    handlePrePare(request, requestBody);
    m_currentType = RESPONSESTATUS;
    m_reply = m_manager->put(request, requestBody.toLatin1());
}

void VidiconProtocol::getPrivacyMaskParameter(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/PrivacyMask?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;

    handlePrePare(request, requestBody);
    m_reply = m_manager->post(request, requestBody.toLatin1());
}

void VidiconProtocol::setPrivacyMaskParameter(QString SessionID, QList<PrivacyMaskParameter> param)
{
    QString urlSuffix = QString("/ISAPI/PrivacyMask?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;

    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                               "<PrivacyMask>"));
    for(int i=0; i<param.count(); i++) {
        requestBody.append(QString("<ZoneSetting_%6>"
                                        "<Enabled>%1</Enabled>"
                                        "<PosX>%2</PosX>"
                                        "<PosY>%3</PosY>"
                                        "<Width>%4</Width>"
                                        "<Height>%5</Height>"
                                    "</ZoneSetting_%6>")
                           .arg(param.at(i).Enabled)
                           .arg(param.at(i).PosX)
                           .arg(param.at(i).PosY)
                           .arg(param.at(i).Width)
                           .arg(param.at(i).Height)
                           .arg(i));
    }

    requestBody.append("</PrivacyMask>");
    handlePrePare(request, requestBody);
    m_currentType = RESPONSESTATUS;
    m_reply = m_manager->put(request, requestBody.toLatin1());
}

void VidiconProtocol::getOSDParameter(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/OSD/OSDInfo?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;

    handlePrePare(request, requestBody);
    m_currentType = OSD;
    m_reply = m_manager->post(request, requestBody.toLatin1());
}

void VidiconProtocol::setOSDParameter(QString SessionID, const QList<OSDParameter> param)
{
    QString urlSuffix = QString("/ISAPI/OSD/OSDInfo?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?><OSDInfo>"));

    for(int i=0; i<param.count(); i++){
        requestBody.append(QString("<OSDParam_%1>"
                                        "<OSDType>%2</OSDType>"
                                        "<Position>"
                                            "<Enabled>%3</Enabled>"
                                            "<OSDText>%4</OSDText>"
                                            "<X>%5</X>"
                                            "<Y>%6</Y>"
                                        "</Position>"
                                        "<DateFormat>%7</DateFormat>"
                                        "<FontSize>%8</FontSize>"
                                        "<FontColor>%9</FontColor>"
                                    "</OSDParam_%1>").arg(i)
                           .arg(param.at(i).OSDType)
                           .arg(param.at(i).Enabled)
                           .arg(param.at(i).OSDText)
                           .arg(param.at(i).x)
                           .arg(param.at(i).y)
                           .arg(param.at(i).DateFormat)
                           .arg(param.at(i).FontSize)
                           .arg(param.at(i).FontColor));
    }

    requestBody.append(QString("</OSDInfo>"));
    handlePrePare(request, requestBody);
    m_currentType = RESPONSESTATUS;
    m_reply = m_manager->put(request, requestBody.toLatin1());
}

void VidiconProtocol::getBasicParameter(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/Network/NetworkConfig?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;

    handlePrePare(request, requestBody);
    m_currentType = TCPIP;
    m_reply = m_manager->post(request, requestBody.toLatin1());
}

void VidiconProtocol::setBasicParameter(QString SessionID, const BasicParameter &param)
{
    QString urlSuffix = QString("/ISAPI/Network/NetworkConfig?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                               "<NetworkConfig>"
                                   "<NetLink>"
                                   "<MaxLink>%1</MaxLink>"
                                   "<DHCP>%2</DHCP>"
                                   "<Name>%3</Name>"
                                   "<Ipv4>"
                                       "<IpAddr>%4</IpAddr>"
                                       "<SubnetMask>%5</SubnetMask>"
                                       "<Gateway>%6</Gateway>"
                                       "<DNS1>%7</DNS1>"
                                       "<DNS2>%8</DNS2>"
                                   "</Ipv4>"
                                   "<Ipv6>"
                                       "<IpAddr>%9</IpAddr>"
                                       "<Gateway>%10</Gateway>"
                                       "<DNS1>%11</DNS1>"
                                       "<DNS2>%12</DNS2>"
                                       "<Ipv6PrefixLength>%13</Ipv6PrefixLength>"
                                   "</Ipv6>"
                                   "<MACAddress><%14</MACAddress>"
                                   "</NetLink>"
                               "</NetworkConfig>").arg(param.MaxLink)
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

    handlePrePare(request, requestBody);
    m_currentType = RESPONSESTATUS;
    m_reply = m_manager->put(request, requestBody.toLatin1());
}

void VidiconProtocol::getEmailParameter(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/Network/Email?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;

    handlePrePare(request, requestBody);
    m_currentType = EMAIL;
    m_reply = m_manager->post(request, requestBody.toLatin1());
}

void VidiconProtocol::setEmailParameter(QString SessionID, const EmailParameter &param)
{
    QString urlSuffix = QString("/ISAPI/Network/Email?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                                "<EmailStatus>"
                                    "<Enabled>%1</Enabled>"
                                    "<MotionAlarmTitle>%2</MotionAlarmTitle>"
                                    "<SensorAlarmTitle>%3</SensorAlarmTitle>"
                                    "<VideoBlindAlarmTitle>%4</VideoBlindAlarmTitle>"
                                    "<VideoLossAlarmTitle>%5</VideoLossAlarmTitle>"
                                    "<SmtpServer>%6</SmtpServer>"
                                    "<SmtpUser>%7</SmtpUser>"
                                    "<SmtpPassword>%8</SmtpPassword>"
                                    "<Sender>%9</Sender>"
                                    "<Receiver_1>%10</Receiver_1>"
                                    "<Receiver_2>%11</Receiver_2>"
                                    "<Receiver_3>%12</Receiver_3>"
                                    "<Receiver_4>%13</Receiver_4>"
                                    "<SmtpPort>%14</SmtpPort>"
                                "</EmailStatus>").arg(param.Enabled)
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

    handlePrePare(request, requestBody);
    m_currentType = RESPONSESTATUS;
    m_reply = m_manager->put(request, requestBody.toLatin1());
}

void VidiconProtocol::getFTPParameter(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/Network/FTP?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;

    handlePrePare(request, requestBody);
    m_currentType = FTP;
    m_reply = m_manager->post(request, requestBody.toLatin1());
}

void VidiconProtocol::setFTPParameter(QString SessionID, const FTPParameter &param)
{
    QString urlSuffix = QString("/ISAPI/Network/FTP?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                                "<FTPStatus>"
                                    "<Enabled>%1</Enabled>"
                                    "<FTPMode>%2</FTPMode>"
                                    "<FTPServer>%3</FTPServer>"
                                    "<FTPUser>%4</FTPUser>"
                                    "<FTPPassword>%5</FTPPassword>"
                                    "<UploadDirectory>%6</UploadDirectory>"
                                    "<FTPPort>%7</FTPPort>"
                                "</FTPStatus>").arg(param.Enabled)
                                              .arg(param.FTPMode)
                                              .arg(param.FTPServer)
                                              .arg(param.FTPUser)
                                              .arg(param.FTPPassword)
                                              .arg(param.UploadDirectory)
                                              .arg(param.FTPPort));

    handlePrePare(request, requestBody);
    m_currentType = RESPONSESTATUS;
    m_reply = m_manager->put(request, requestBody.toLatin1());
}

void VidiconProtocol::getDDNSParameter(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/Network/DDNS?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;

    handlePrePare(request, requestBody);
    m_currentType = DDNS;
    m_reply = m_manager->post(request, requestBody.toLatin1());
}

void VidiconProtocol::setDDNSParameter(QString SessionID, const DDNSParameter &param)
{
    QString urlSuffix = QString("/ISAPI/Network/DDNS?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                                "<DDNSStatus>"
                                   "<Enabled>%1</Enabled>"
                                   "<DDNSType>%2</DDNSType>"
                                   "<DDNSServerName>%3</DDNSServerName>"
                                   "<DDNSName>%4</DDNSName>"
                                   "<DDNSUser>%5</DDNSUser>"
                                   "<DDNSPassword>%6</DDNSPassword>"
                                "</DDNSStatus>").arg(param.Enabled)
                                               .arg(param.DDNSType)
                                               .arg(param.DDNSServerName)
                                               .arg(param.DDNSName)
                                               .arg(param.DDNSUser)
                                               .arg(param.DDNSPassword));

    handlePrePare(request, requestBody);
    m_currentType = RESPONSESTATUS;
    m_reply = m_manager->put(request, requestBody.toLatin1());
}

void VidiconProtocol::getPPPOEParameter(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/Network/PPPOE?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;

    handlePrePare(request, requestBody);
    m_currentType = PPPOE;
    m_reply = m_manager->post(request, requestBody.toLatin1());
}

void VidiconProtocol::setPPPOEParameter(QString SessionID, const PPPOEParameter &param)
{
    QString urlSuffix = QString("/ISAPI/Network/PPPOE?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                                "<PPPOEStatus>"
                                    "<Enabled>%1</Enabled>"
                                    "<PPPOEName>%2</PPPOEName>"
                                    "<PPPOEPassword>%3</PPPOEPassword>"
                                "</PPPOEStatus>").arg(param.Enabled)
                                                .arg(param.PPPOEName)
                                                .arg(param.PPPOEPassword));

    handlePrePare(request, requestBody);
    m_currentType = RESPONSESTATUS;
    m_reply = m_manager->put(request, requestBody.toLatin1());
}

void VidiconProtocol::getSNMPParameter(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/Network/SNMP?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;

    handlePrePare(request, requestBody);
    m_currentType = SNMP;
    m_reply = m_manager->post(request, requestBody.toLatin1());
}

void VidiconProtocol::setSNMPParameter(QString SessionID, const SNMPParameter &param)
{
    QString urlSuffix = QString("/ISAPI/Network/SNMP?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                                "<SNMPStatus>"
                                    "<Enabled>"
                                        "<Ver1>%1</Ver1>"
                                        "<Ver2>%2</Ver2>"
                                    "</Enabled>"
                                    "<ReadPublic>%3</ReadPublic>"
                                    "<WritePublic>%4</WritePublic>"
                                    "<TrapAddress>%5</TrapAddress>"
                                    "<SnmpPort>%6</SnmpPort>"
                                    "<TrapPort>%7</TrapPort>"
                                "</SNMPStatus>").arg(param.EnabledVer1)
                                               .arg(param.EnabledVer2)
                                               .arg(param.ReadPublic)
                                               .arg(param.WritePublic)
                                               .arg(param.TrapAddress)
                                               .arg(param.SnmpPort)
                                               .arg(param.TrapPort));

    handlePrePare(request, requestBody);
    m_currentType = RESPONSESTATUS;
    m_reply = m_manager->put(request, requestBody.toLatin1());
}

void VidiconProtocol::getP2PParameter(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/Network/P2P?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;

    handlePrePare(request, requestBody);
    m_currentType = P2P;
    m_reply = m_manager->post(request, requestBody.toLatin1());
}

void VidiconProtocol::setP2PParameter(QString SessionID, const P2PParameter &param)
{
    QString urlSuffix = QString("/ISAPI/Network/P2P?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                                "<P2PStatus>"
                                    "<Enabled>%1</Enabled>"
                                    "<P2PUUID></P2PUUID>"
                                "</P2PStatus>").arg(param.Enabled));

    handlePrePare(request, requestBody);
    m_currentType = RESPONSESTATUS;
    m_reply = m_manager->put(request, requestBody.toLatin1());
}

void VidiconProtocol::getBonjourParameter(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/Network/Bonjour?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;

    handlePrePare(request, requestBody);
    m_currentType = BONJOUR;
    m_reply = m_manager->post(request, requestBody.toLatin1());
}

void VidiconProtocol::setBonjourParameter(QString SessionID, const BonjourParameter &param)
{
    QString urlSuffix = QString("/ISAPI/Network/Bonjour?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                                "<BonjourStatus>"
                                    "<Enabled>%1</Enabled>"
                                    "<Name>%2</Name>"
                                "</BonjourStatus>").arg(param.Enabled)
                                                  .arg(param.Name));

    handlePrePare(request, requestBody);
    m_currentType = RESPONSESTATUS;
    m_reply = m_manager->put(request, requestBody.toLatin1());
}

void VidiconProtocol::getHTTPsParameter(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/Network/HTTPs?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;

    handlePrePare(request, requestBody);
    m_currentType = HTTPS;
    m_reply = m_manager->post(request, requestBody.toLatin1());
}

void VidiconProtocol::setHTTPsParameter(QString SessionID, const HTTPsParameter &param)
{
    QString urlSuffix = QString("/ISAPI/Network/HTTPs?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                                "<HTTPsStatus>"
                                    "<Enabled>%1</Enabled>"
                                    "<HTTPsPort>%2</HTTPsPort>"
                                "</HTTPsStatus>").arg(param.Enabled)
                                                .arg(param.HTTPsPort));

    handlePrePare(request, requestBody);
    m_currentType = RESPONSESTATUS;
    m_reply = m_manager->put(request, requestBody.toLatin1());
}

void VidiconProtocol::getUPNPParameter(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/Network/UPNP?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;

    handlePrePare(request, requestBody);
    m_currentType = UPNP;
    m_reply = m_manager->post(request, requestBody.toLatin1());
}

void VidiconProtocol::setUPNPParameter(QString SessionID, const UPNPParameter &param)
{
    QString urlSuffix = QString("/ISAPI/Network/UPNP?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                                "<UPNPStatus>"
                                    "<Enabled>%1</Enabled>"
                                "</UPNPStatus>").arg(param.Enabled));

    handlePrePare(request, requestBody);
    m_currentType = RESPONSESTATUS;
    m_reply = m_manager->put(request, requestBody.toLatin1());
}

void VidiconProtocol::getOtherParameter(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/Network/ExtServerParam?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;

    handlePrePare(request, requestBody);
    m_currentType = OTHER;
    m_reply = m_manager->post(request, requestBody.toLatin1());
}

void VidiconProtocol::setOtherParameter(QString SessionID, const OtherParameter &param)
{
    QString urlSuffix = QString("/ISAPI/Network/ExtServerParam?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                                "<ExtServer>"));
    OtherBasicParameter *param1 = (OtherBasicParameter *)&param;
    for(int i=0; i<3; i++) {
        requestBody.append(QString("<Option_%4>"
                                        "<ServerType>%1</ServerType>"
                                        "<Enabled>%2</Enabled>"
                                        "<Port>%3</Port>"
                                    "</Option_%4>").arg(param1->ServerType)
                                                  .arg(param1->Enabled)
                                                  .arg(param1->Port)
                                                  .arg(i));
        param1++;
    }

    requestBody.append("</ExtServer>");
    handlePrePare(request, requestBody);
    m_currentType = RESPONSESTATUS;
    m_reply = m_manager->put(request, requestBody.toLatin1());
}

void VidiconProtocol::getRemoteRecordingPlan(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/RecFile/VideoPlan?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;

    handlePrePare(request, requestBody);
    m_currentType = SCHEDULE;
    m_reply = m_manager->post(request, requestBody.toLatin1());
}

void VidiconProtocol::setRemoteRecordingPlan(QString SessionID, const RemoteRecordingPlan &param)
{
    QString urlSuffix = QString("/ISAPI/RecFile/VideoPlan?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                                "<RemoteVideoPlan>"
                                "<Enabled>%1</Enabled>").arg(param.Enabled));

    for(int i=0; i<7; i++){
        if(param.weeksStateMap[i] != Qt::Checked)
            continue;
        requestBody.append(QString("<WeekDay_%1>").arg(i));
        for(int j=0; j<4; j++){
            requestBody.append(QString("<Section_%1>"
                                            "<PlanTimeEnabled>%2</PlanTimeEnabled>"
                                            "<BeginTime>%3</BeginTime>"
                                            "<EndTime>%4</EndTime>"
                                        "</Section_%1>").arg(j)
                                                      .arg(param.Plans[i][j].PlanTimeEnabled)
                                                      .arg(param.Plans[i][j].BeginTime.toString("HH:mm:ss"))
                                                      .arg(param.Plans[i][j].EndTime.toString("HH:mm:ss")));
        }

        requestBody.append(QString("</WeekDay_%1>").arg(i));
    }
    requestBody.append("</RemoteVideoPlan>");

    handlePrePare(request, requestBody);
    m_currentType = RESPONSESTATUS;
    m_reply = m_manager->put(request, requestBody.toLatin1());
}

void VidiconProtocol::getSDCardStatusQuery(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/Sdcard/QuerySDCardStat?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;

    handlePrePare(request, requestBody);
    m_currentType = SDCARD;
    m_reply = m_manager->post(request, requestBody.toLatin1());
}

void VidiconProtocol::setSDCardOperation(QString SessionID, int OperType)
{
    QString urlSuffix = QString("/ISAPI/Sdcard/SDOperation?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                                "<SDOperation>"
                                    "<OperType>%1</OperType>"
                                "</SDOperation>").arg(OperType));

    handlePrePare(request, requestBody);
    m_currentType = RESPONSESTATUS;
    m_reply = m_manager->put(request, requestBody.toLatin1());
}

void VidiconProtocol::getSDStorageParameter(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/Sdcard/SDStorage?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;

    handlePrePare(request, requestBody);
    m_currentType = SDSTORAGE;
    m_reply = m_manager->post(request, requestBody.toLatin1());
}

void VidiconProtocol::setSDStorageParameter(QString SessionID, const SDStorageParameter &param)
{
    QString urlSuffix = QString("/ISAPI/Sdcard/SDStorage?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                                "<SDStorage>"
                                    "<OperType>%1</OperType>"
                                   "<RecordSelect>%2</RecordSelect>"
                                   "<RecordMode>%3</RecordMode>"
                                   "<RecordTime>%4</RecordTime>"
                                "</SDStorage>").arg(param.OperType)
                                              .arg(param.RecordSelect)
                                              .arg(param.RecordMode)
                                              .arg(param.RecordTime));

    handlePrePare(request, requestBody);
    m_currentType = RESPONSESTATUS;
    m_reply = m_manager->put(request, requestBody.toLatin1());
}

void VidiconProtocol::getSnapshotPlanParameter(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/Snapshot?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;

    handlePrePare(request, requestBody);
    m_currentType = SNAPSHOT;
    m_reply = m_manager->post(request, requestBody.toLatin1());
}

void VidiconProtocol::setSnapshotPlanParameter(QString SessionID, const SnapshotPlanParameter &param)
{
    QString urlSuffix = QString("/ISAPI/Snapshot?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                                "<SnapshotChannel>"
                                "<Enabled>%1</Enabled>"
                                "<SnapIntervalTime>%2</SnapIntervalTime>")
                       .arg(param.Enabled)
                       .arg(param.SnapIntervalTime));
    for(int i=0; i<7; i++){
        if(param.weeksStateMap[i] != Qt::Checked)
            continue;
        requestBody.append(QString("<WeekDay_%1>").arg(i));
        for(int j=0; j<4; j++){
            requestBody.append(QString("<Section_%1>"
                                            "<PlanTimeEnabled>%2</PlanTimeEnabled>"
                                            "<BeginTime>%3</BeginTime>"
                                            "<EndTime>%4</EndTime>"
                                        "</Section_%1>")
                               .arg(j)
                               .arg(param.Plans[i][j].PlanTimeEnabled)
                               .arg(param.Plans[i][j].BeginTime.toString("HH:mm:ss"))
                               .arg(param.Plans[i][j].EndTime.toString("HH:mm:ss")));
        }

        requestBody.append(QString("</WeekDay_%1>").arg(i));
    }
    requestBody.append(QString("</SnapshotChannel>"));

    handlePrePare(request, requestBody);
    m_currentType = RESPONSESTATUS;
    m_reply = m_manager->put(request, requestBody.toLatin1());
}

void VidiconProtocol::getMotionDetectionParameter(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/Event/MotionDetectionParam?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;

    handlePrePare(request, requestBody);
    m_currentType = MOTION;
    m_reply = m_manager->post(request, requestBody.toLatin1());
}

void VidiconProtocol::setMotionDetectionParameter(QString SessionID, const MotionDetectionParameter &param)
{
    QString urlSuffix = QString("/ISAPI/Event/MotionDetectionParam?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                                "<MotionDetectionParam>"
                                    "<Enabled>%1</Enabled>"
                                    "<AlarmOutput>%2</AlarmOutput>"
                                    "<VideoOutput>%3</VideoOutput>"
                                    "<AlarmDuration>%4</AlarmDuration>"
                                    "<AlarmThreshold>%5</AlarmThreshold>"
                                    "<PreRecTime>%6</PreRecTime>"
                                    "<DelayRecTime>%7</DelayRecTime>"
                                    "<Sensitivity>%8</Sensitivity>")
                               .arg(param.Enabled)
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
            requestBody.append(QString("<Section_%1>"
                                            "<PlanTimeEnabled>%2</PlanTimeEnabled>"
                                            "<BeginTime>%3</BeginTime>"
                                            "<EndTime>%4</EndTime>"
                                        "</Section_%1>")
                                       .arg(j)
                                       .arg(param.Plans[i][j].PlanTimeEnabled)
                                       .arg(param.Plans[i][j].BeginTime.toString("HH:mm:ss"))
                                       .arg(param.Plans[i][j].EndTime.toString("HH:mm:ss")));
        }
        requestBody.append(QString("</WeekDay_%1>").arg(i));
    }
    requestBody.append(QString("<ZoneSetting><AreaMask>%1</AreaMask></ZoneSetting>\
                                </MotionDetectionParam>").arg(param.AreaMask));

    handlePrePare(request, requestBody);
    m_currentType = RESPONSESTATUS;
    m_reply = m_manager->put(request, requestBody.toLatin1());
}

void VidiconProtocol::getSensorAlarmParameter(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/Event/SensorAlarmParam?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;

    handlePrePare(request, requestBody);
    m_currentType = SENSOR;
    m_reply = m_manager->post(request, requestBody.toLatin1());
}

void VidiconProtocol::setSensorAlarmParameter(QString SessionID, const SensorAlarmParameter &param)
{
    QString urlSuffix = QString("/ISAPI/Event/SensorAlarmParam?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                                "<SensorList>"
                                    "<Enabled>%1</Enabled>"
                                    "<AlarmOutput>%2</AlarmOutput>"
                                    "<VideoOutput>%3</VideoOutput>"
                                    "<AlarmDuration>%4</AlarmDuration>"
                                    "<SensorType>%5</SensorType>")
                       .arg(param.Enabled)
                       .arg(param.AlarmOutput)
                       .arg(param.VideoOutput)
                       .arg(param.AlarmDuration)
                       .arg(param.SensorType));
    for(int i=0; i<7; i++){
        if(param.weeksStateMap[i] != Qt::Checked)
            continue;
        requestBody.append(QString("<WeekDay_%1>").arg(i));
        for(int j=0; j<4; j++){
            requestBody.append(QString("<Section_%1>"
                                            "<PlanTimeEnabled>%2</PlanTimeEnabled>"
                                            "<BeginTime>%3</BeginTime>"
                                            "<EndTime>%4</EndTime>"
                                        "</Section_%1>")
                               .arg(j)
                               .arg(param.Plans[i][j].PlanTimeEnabled)
                               .arg(param.Plans[i][j].BeginTime.toString("HH:mm:ss"))
                               .arg(param.Plans[i][j].EndTime.toString("HH:mm:ss")));
        }

        requestBody.append(QString("</WeekDay_%1>").arg(i));
    }
    requestBody.append(QString("</SensorList>"));

    handlePrePare(request, requestBody);
    m_currentType = RESPONSESTATUS;
    m_reply = m_manager->put(request, requestBody.toLatin1());
}

void VidiconProtocol::getVideoBlindAlarmParameter(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/Event/VideoBlindAlarmParam?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;

    handlePrePare(request, requestBody);
    m_currentType = BLIND;
    m_reply = m_manager->post(request, requestBody.toLatin1());
}

void VidiconProtocol::setVideoBlindAlarmParameter(QString SessionID, const VideoBlindAlarmParameter &param)
{
    QString urlSuffix = QString("/ISAPI/Event/VideoBlindAlarmParam?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                                "<VideoBlindAlarmParam>"
                                    "<Enabled>%1</Enabled>"
                                    "<AlarmOutput>%2</AlarmOutput>"
                                    "<VideoOutput>%3</VideoOutput>"
                                    "<AlarmDuration>%4</AlarmDuration>"
                                    "<Sensitivity>%5</Sensitivity>")
                       .arg(param.Enabled)
                       .arg(param.AlarmOutput)
                       .arg(param.VideoOutput)
                       .arg(param.AlarmDuration)
                       .arg(param.Sensitivity));
    for(int i=0; i<7; i++){
        if(param.weeksStateMap[i] != Qt::Checked)
            continue;
        requestBody.append(QString("<WeekDay_%1>").arg(i));
        for(int j=0; j<4; j++){
            requestBody.append(QString("<Section_%1>"
                                            "<PlanTimeEnabled>%2</PlanTimeEnabled>"
                                            "<BeginTime>%3</BeginTime>"
                                            "<EndTime>%4</EndTime>"
                                        "</Section_%1>")
                               .arg(j)
                               .arg(param.Plans[i][j].PlanTimeEnabled)
                               .arg(param.Plans[i][j].BeginTime.toString("HH:mm:ss"))
                               .arg(param.Plans[i][j].EndTime.toString("HH:mm:ss")));
        }

        requestBody.append(QString("</WeekDay_%1>").arg(i));
    }
    requestBody.append(QString("</VideoBlindAlarmParam>"));

    handlePrePare(request, requestBody);
    m_currentType = RESPONSESTATUS;
    m_reply = m_manager->put(request, requestBody.toLatin1());
}

void VidiconProtocol::getPullAlarmRequest(QString SessionID, int channel)
{
    QString urlSuffix = QString("/ISAPI/Alarm/PullMsg?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                                "<PullMsg>"
                                    "<Channel>%1</Channel>"
                                "</PullMsg>")
                       .arg(channel));

    handlePrePare(request, requestBody);
    m_currentType = PULLMESSAGE;
    m_reply = m_manager->post(request, requestBody.toLatin1());
}

void VidiconProtocol::getMotionDetectionChanged(QString SessionID, int MotionArea)
{
    QString urlSuffix = QString("/ISAPI/Event/MotionChange?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                                "<MotionChange>"
                                    "<MotionArea>%1</MotionArea>"
                                "</MotionChange>")
                       .arg(MotionArea));

    handlePrePare(request, requestBody);
    m_reply = m_manager->post(request, requestBody.toLatin1());
}

void VidiconProtocol::getImageParameter(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/Preview/ImageConfig?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;

    handlePrePare(request, requestBody);
    m_currentType = IMAGE;
    m_reply = m_manager->post(request, requestBody.toLatin1());
}

void VidiconProtocol::setImageParameter(QString SessionID, const ImageParameter &param)
{
    QString urlSuffix = QString("/ISAPI/Preview/ImageConfig?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                                "<ImageChannel>"
                                    "<ImageFlip>"
                                        "<Mirror>%1</Mirror>"
                                        "<Turn>%2</Turn>"
                                    "</ImageFlip>"
                                    "<Vision>"
                                        "<VisionMode>%3</VisionMode>"
                                    "</Vision>"
                                    "<Exposure>"
                                        "<ExposureMode>%4</ExposureMode>"
                                        "<Shutter>%5</Shutter>"
                                    "</Exposure>"
                                    "<IrcutFilter>"
                                        "<IrcutFilterMode>%6</IrcutFilterMode>"
                                        "<HighLowLevel>%7</HighLowLevel>"
                                        "<BeginTime>%8</BeginTime>"
                                        "<EndTime>%9</EndTime>"
                                    "</IrcutFilter>"
                                    "<AntiFlash>"
                                        "<AntiFlashMode>%10</AntiFlashMode>"
                                    "</AntiFlash>"
                                    "<BLC>"
                                        "<BLCMode>%11</BLCMode>"
                                        "<BLCIntensity>%12</BLCIntensity>"
                                        "<WDRIntensity>%13</WDRIntensity>"
                                        "<HLCIntensity>%14</HLCIntensity>"
                                        "<DWDRIntensity>%15</DWDRIntensity>"
                                    "</BLC>"
                                    "<NoiseReduce>"
                                        "<NoiseReduceMode>%16</NoiseReduceMode>"
                                    "</NoiseReduce>"
                                    "<LDC>"
                                        "<LDCEnabled>%17</LDCEnabled>"
                                    "</LDC>"
                                    "<Color>"
                                        "<BrightnessLevel>%18</BrightnessLevel>"
                                        "<ContrastLevel>%19</ContrastLevel>"
                                        "<SaturationLevel>%20</SaturationLevel>"
                                        "<HueLevel>%21</HueLevel>"
                                        "<Sharpness>%22</Sharpness>"
                                    "</Color>"
                                "</ImageChannel>")
                       .arg(param.Mirror)
                       .arg(param.Turn)
                       .arg(param.VisionMode)
                       .arg(param.ExposureMode)
                       .arg(param.Shutter)
                       .arg(param.IrcutFilterMode)
                       .arg(param.HighLowLevel)
                       .arg(param.BeginTime.toString("hh:mm"))
                       .arg(param.EndTime.toString("hh:mm"))
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

    handlePrePare(request, requestBody);
    m_currentType = RESPONSESTATUS;
    m_reply = m_manager->put(request, requestBody.toLatin1());
}

void VidiconProtocol::getNTP(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/System/NTP?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;

    handlePrePare(request, requestBody);
    m_currentType = NTP;
    m_reply = m_manager->post(request, requestBody.toLatin1());
}

void VidiconProtocol::setNTP(QString SessionID, const NTPParameter &param)
{
    QString urlSuffix = QString("/ISAPI/System/NTP?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                               " <NTPStatus>"
                                    "<TimeZone>"
                                        "<TZ>%1</TZ>"
                                    "</TimeZone>"
                                    "<IsUpdateTime>%2</IsUpdateTime>"
                                    "<Enabled>%3</Enabled>"
                                    "<UTCDateTime>%4</UTCDateTime>"
                                    "<NTPServer>%5</NTPServer>"
                                "</NTPStatus>")
                       .arg(param.TZ)
                       .arg(param.IsUpdateTime)
                       .arg(param.Enabled)
                       .arg(param.UTCDateTime)
                       .arg(param.NTPServer));

    handlePrePare(request, requestBody);
    m_currentType = RESPONSESTATUS;
    m_reply = m_manager->put(request, requestBody.toLatin1());
}

void VidiconProtocol::setRestoreDefaultParameter(QString SessionID, int Enabled)
{
    QString urlSuffix = QString("/ISAPI/System/RestoreDefault?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                        "<RestoreDefault>"
                            "<Enabled>%1</Enabled>"
                        "</RestoreDefault>")
                       .arg(Enabled));

    handlePrePare(request, requestBody);
    m_reply = m_manager->put(request, requestBody.toLatin1());
}

void VidiconProtocol::setDeviceReboot(QString SessionID, int Enabled)
{
    QString urlSuffix = QString("/ISAPI/System/Reboot?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                        "<Reboot>"
                            "<Enabled>%1</Enabled>"
                        "</Reboot>")
                       .arg(Enabled));

    handlePrePare(request, requestBody);
    m_currentType = RESPONSESTATUS;
    m_reply = m_manager->put(request, requestBody.toLatin1());
}

void VidiconProtocol::setFormat(QString SessionID, int param)
{
    QString urlSuffix = QString("/ISAPI/System/Format?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                        "<Format>"
                            "<Param>%1</Param>"
                        "</Format>")
                       .arg(param));

    handlePrePare(request, requestBody);
    m_currentType = RESPONSESTATUS;
    m_reply = m_manager->put(request, requestBody.toLatin1());
}

void VidiconProtocol::PTZControl(QString SessionID, int Zoom, int Focus)
{
    QString urlSuffix = QString("/ISAPI/PTZ/PtzMotorsCtr?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                        "<PTZMotorsCtr>"
                            "<Zoom>%1</Zoom>"
                            "<Focus>%2</Focus>"
                        "</PTZMotorsCtr>")
                       .arg(Zoom)
                       .arg(Focus));

    handlePrePare(request, requestBody);
    m_reply = m_manager->put(request, requestBody.toLatin1());
}

void VidiconProtocol::getLPRSystemVersion(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/LPR/LPRVersion?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;

    handlePrePare(request, requestBody);
    m_reply = m_manager->post(request, requestBody.toLatin1());
}

void VidiconProtocol::getLPRParameter(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/LPR/LPRParam?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;

    handlePrePare(request, requestBody);
    m_reply = m_manager->post(request, requestBody.toLatin1());
}

void VidiconProtocol::setLPRParameter(QString SessionID, const LPRParameter &param1, const AreaPoint &param2)
{
    QString urlSuffix = QString("/ISAPI/LPR/LPRParam?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                                "<LPRParam>"
                                    "<Enabled>%1</Enabled>"
                                    "<MinPlateWidth>%2</MinPlateWidth>"
                                    "<MaxPlateWidth>%3</MaxPlateWidth>"
                                    "<MinSymbolCount>%4</MinSymbolCount>"
                                    "<MaxSymbolCount>%5</MaxSymbolCount>"
                                    "<TTLMilliSeconds>%6</TTLMilliSeconds>"
                                    "<ResultDelayMilliSeconds>%7</ResultDelayMilliSeconds>")
                       .arg(param1.Enabled)
                       .arg(param1.MinPlateWidth)
                       .arg(param1.MaxPlateWidth)
                       .arg(param1.MinSymbolCount)
                       .arg(param1.MaxSymbolCount)
                       .arg(param1.TTLMilliSeconds)
                       .arg(param1.ResultDelayMilliSeconds));
    for(int i=0; i<1; i++){
        requestBody.append(QString("<ZoneSetting_%1>").arg(i));

        requestBody.append(QString("<AreaID><!-- req, xs: int --></AreaID>"
                                    "<TopLeft>"
                                        "<X>%1</X>"
                                        "<Y>%2</Y>"
                                    "</TopLeft>"
                                    "<TopRight>"
                                        "<X>%3</X>"
                                        "<Y>%4</Y>"
                                    "</TopRight>"
                                    "<BottomRight>"
                                        "<X>%5</X>"
                                        "<Y>%6</Y>"
                                    "</BottomRight>"
                                    "<BottomLeft>"
                                        "<X>%7</X>"
                                        "<Y>%8</Y>"
                                    "</BottomLeft>")
                           .arg(param2.TopLeft.x())
                           .arg(param2.TopLeft.y())
                           .arg(param2.TopRight.x())
                           .arg(param2.TopRight.y())
                           .arg(param2.BottomRight.x())
                           .arg(param2.BottomRight.y())
                           .arg(param2.BottomLeft.x())
                           .arg(param2.BottomLeft.y()));

        requestBody.append(QString("</ZoneSetting_%1>").arg(i));
    }
    requestBody.append(QString("</LPRParam>"));

    handlePrePare(request, requestBody);
    m_reply = m_manager->put(request, requestBody.toLatin1());
}

void VidiconProtocol::resetLPRParameter(QString SessionID, int Enabled)
{
    QString urlSuffix = QString("/ISAPI/LPR/LPRResetParam?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                        "<LPRResetParam>"
                            "<Enabled>%1</Enabled>"
                        "</LPRResetParam>")
                       .arg(Enabled));

    handlePrePare(request, requestBody);
    m_reply = m_manager->put(request, requestBody.toLatin1());
}

void VidiconProtocol::getInformation(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/LPR/LPROutput?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                        "<LPROutput>"
                            "<LastCarID>%1</LastCarID>"
                        "</LPROutput>")
                       .arg(""));

    handlePrePare(request, requestBody);
    m_reply = m_manager->post(request, requestBody.toLatin1());
}

void VidiconProtocol::backUpQuery(QString SessionID, const BackUpQueryParameter &param)
{
    QString urlSuffix = QString("/ISAPI/Record/BackUp?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                        "<QueryBackUp>"
                            "<Type>%1</Type>"
                            "<Time>%2</Time>"
                        "</QueryBackUp>")
                       .arg(param.Type)
                       .arg(param.Date.toString("yyyy-MM-dd")));

    handlePrePare(request, requestBody);
    switch(param.Type) {
    case 0: {
        m_currentType = QUERYVIDEOTIMEDAY;
        break;
    }
    case 1: {
        m_currentType = QUERYVIDEONAMEDAY;
        break;
    }
    case 2: {
        m_currentType = QUERYPICTURENAMEDAY;
        break;
    }
    case 6: {
        m_currentType = QUERYFILEMONTH;
        break;
    }
    }
    m_reply = m_manager->put(request, requestBody.toLatin1());
}

void VidiconProtocol::setRecordStartPlayingTime(QString SessionID, const StartPlayingParameter &param)
{
    QString urlSuffix = QString("/ISAPI/Record/TimePos?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                                "<TimePos>"
                                    "<htmlid>%1</htmlid>"
                                    "<playing>%2</playing>"
                                    "<Time>%3</Time>"
                                "</TimePos>")
                       .arg(param.htmlid)
                       .arg(param.playing)
                       .arg(param.Time.toString("yyyy-MM-ddTHH:mm:ss")));

    handlePrePare(request, requestBody);
    m_currentType = STARTPLAYING;
    m_reply = m_manager->put(request, requestBody.toLatin1());
}

void VidiconProtocol::setFastOrSlowPlayState(QString SessionID, const PlayStateParameter &param)
{
    QString urlSuffix = QString("/ISAPI/Record/RecordRunState?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                                "<RecordRunState>"
                                    "<htmlid>%1</htmlid>"
                                    "<StateValue>%2</StateValue>"
                                "</RecordRunState>")
                       .arg(param.htmlid)
                       .arg(param.StateValue));

    handlePrePare(request, requestBody);
    m_currentType = PLAYSTATE;
    m_reply = m_manager->post(request, requestBody.toLatin1());
}

void VidiconProtocol::getCurrentPlayingTime(int htmlid, QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/Record/GetPosTime?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                        "<GetPosTime>"
                            "<htmlid>%1</htmlid>"
                        "</GetPosTime>")
                       .arg(htmlid));

    handlePrePare(request, requestBody);
    m_currentType = CURRENTPLAYINGTIME;
    m_reply = m_manager->post(request, requestBody.toLatin1());
}

void VidiconProtocol::getUserConfig(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/UserManagement/UsrConfig?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;

    handlePrePare(request, requestBody);
    m_currentType = USERCONFIG;
    m_reply = m_manager->post(request, requestBody.toLatin1());
}

void VidiconProtocol::setUserConfig(QString SessionID, const UserConfigInfo &info)
{
    QString urlSuffix = QString("/ISAPI/UserManagement/UsrConfig?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                               "<UserManagment>"
                                   "<UserName>%1</UserName>"
                                   "<PassWord>%2</PassWord>"
                                   "<Privilege>%3</Privilege>"
                               "</UserManagment>")
                       .arg(info.UserName)
                       .arg(info.PassWord)
                       .arg(info.Privilege));

    handlePrePare(request, requestBody);
    m_currentType = RESPONSESTATUS;
    m_reply = m_manager->put(request, requestBody.toLatin1());
}

void VidiconProtocol::addUser(QString SessionID, const UserConfigInfo &info)
{
    QString urlSuffix = QString("/ISAPI/UserManagement/UsrConfig?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                               "<UserManagment>"
                                   "<UserName>%1</UserName>"
                                   "<PassWord>%2</PassWord>"
                                   "<Privilege>%3</Privilege>"
                               "</UserManagment>")
                       .arg(info.UserName)
                       .arg(info.PassWord)
                       .arg(info.Privilege));

    handlePrePare(request, requestBody);
    m_currentType = RESPONSESTATUS;
    m_reply = m_manager->put(request, requestBody.toLatin1());
}

void VidiconProtocol::delUser(QString SessionID, const UserConfigInfo &info)
{
    QString urlSuffix = QString("/ISAPI/UserManagement/DelUsr?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                               "<DelUsr>"
                                   "<UserName>%1</UserName>"
                               "</DelUsr>")
                       .arg(info.UserName));

    handlePrePare(request, requestBody);
    m_currentType = RESPONSESTATUS;
    m_reply = m_manager->put(request, requestBody.toLatin1());
}

void VidiconProtocol::getWifiConfig(QString SessionID)
{
    QString urlSuffix = QString("/ISAPI/Network/Wifi?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;

    handlePrePare(request, requestBody);
    m_currentType = WIFI;
    m_reply = m_manager->post(request, requestBody.toLatin1());
}

void VidiconProtocol::setWifiConfig(QString SessionID, const WifiConfigParameter &param)
{
    QString urlSuffix = QString("/ISAPI/Network/Wifi?ID=%1").arg(SessionID);
    QNetworkRequest request;
    request.setUrl(QUrl(urlPrefix() + urlSuffix));

    QString requestBody;
    requestBody.append(QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                               "<WifiConfig>"
                                   "<wifiapssid>%1</wifiapssid>"
                                   "<wifiappassword>%2</wifiappassword>"
                               "</WifiConfig>")
                       .arg(param.ssid)
                       .arg(param.passwd));

    handlePrePare(request, requestBody);
    m_currentType = RESPONSESTATUS;
    m_reply = m_manager->put(request, requestBody.toLatin1());
}

void VidiconProtocol::handlePrePare(QNetworkRequest &request, QString RequestBody)
{
    //定制请求头
    request.setRawHeader(QByteArray("Accept"),           QByteArray("text/plain, */*; q=0.01"));
    request.setRawHeader(QByteArray("X-Requested-With"), QByteArray("XMLHttpRequest"));
    request.setRawHeader(QByteArray("Referer"),          QByteArray(QString("http://%1/BaseInforPage.html").arg(m_host).toLatin1()));
    request.setRawHeader(QByteArray("Accept-Language"),  QByteArray("zh-CN"));
    request.setRawHeader(QByteArray("Accept-Encoding"),  QByteArray("gzip, deflate"));
    request.setRawHeader(QByteArray("User-Agent"),       QByteArray("Mozilla/5.0 (Windows NT 10.0; WOW64; Trident/7.0; rv:11.0) like Gecko"));
    request.setRawHeader(QByteArray("Host"),             QByteArray(m_host.toLatin1()));
    request.setRawHeader(QByteArray("Content-Length"),   QByteArray(QString::number(RequestBody.length()).toLatin1()));
    request.setRawHeader(QByteArray("Connection"),       QByteArray("Keep-Alive"));
    request.setRawHeader(QByteArray("Cache-Control"),    QByteArray("no-cache"));
    request.setRawHeader(QByteArray("Content-Type"),     QByteArray("application/xml"));
}

void VidiconProtocol::handleReply(QNetworkReply *reply)
{
    QByteArray buffer;
    if (reply->error() == QNetworkReply::NoError)
        buffer = reply->readAll();
    else
        emit error(reply->error());

    qDebug() << "#VidiconProtocol# hanndlerReply, send signal ParameterType:" << m_replyMap.value(reply);
    emit signalReceiveData(m_replyMap.value(reply), buffer);

#if 0
        qDebug("#VidiconProtocol# hanndlerReply, response content start............");
        qDebug() << bytes.toStdString().data();
        qDebug("#VidiconProtocol# hanndlerReply, response content end  ............");
#endif
    m_replyMap.remove(reply);
    m_currentType = NONE;
    reply->deleteLater();
}


void VidiconProtocol::handleSetParameter(Type type, QVariant param, QString SessionID)
{
    qDebug() << "#VidiconProtocol# handleSetParameter type:" << type;
    switch (type) {
        case VIDEOENCODING: {
            VideoEncodingParameter temp = param.value<VideoEncodingParameter>();
            setVideoEncodingParameter(SessionID, temp);
            break;
        }
        case AUDIOENCODING: {
            AudioEncodingParameter temp = param.value<AudioEncodingParameter>();
            setAudioEncodingParameter(SessionID, temp);
            break;
        }
        case OSD: {
            QList<OSDParameter> temp = param.value<QList<OSDParameter>>();
            setOSDParameter(SessionID, temp);
            break;
        }
        case NTP: {
            NTPParameter temp = param.value<NTPParameter>();
            setNTP(SessionID, temp);
            break;
        }
        case PRIVACY: {
            QList<PrivacyMaskParameter> temp = param.value<QList<PrivacyMaskParameter>>();
            setPrivacyMaskParameter(SessionID, temp);
            break;
        }
        case IMAGE: {
            ImageParameter temp = param.value<ImageParameter>();
            setImageParameter(SessionID, temp);
            break;
        }
        case TCPIP: {
            BasicParameter temp = param.value<BasicParameter>();
            setBasicParameter(SessionID, temp);
            break;
        }
        case OTHER: {
            OtherParameter temp = param.value<OtherParameter>();
            setOtherParameter(SessionID, temp);
            break;
        }
        case MOTION: {
            MotionDetectionParameter temp = param.value<MotionDetectionParameter>();
            setMotionDetectionParameter(SessionID, temp);
            break;
        }
        case BLIND: {
            VideoBlindAlarmParameter temp = param.value<VideoBlindAlarmParameter>();
            setVideoBlindAlarmParameter(SessionID, temp);
            break;
        }
        case SENSOR: {
            SensorAlarmParameter temp = param.value<SensorAlarmParameter>();
            setSensorAlarmParameter(SessionID, temp);
            break;
        }
        case SCHEDULE: {
            RemoteRecordingPlan temp = param.value<RemoteRecordingPlan>();
            setRemoteRecordingPlan(SessionID, temp);
            break;
        }
        case SDSTORAGE: {
            SDStorageParameter temp = param.value<SDStorageParameter>();
            setSDStorageParameter(SessionID, temp);
            break;
        }
        case SNAPSHOT: {
            SnapshotPlanParameter temp = param.value<SnapshotPlanParameter>();
            setSnapshotPlanParameter(SessionID, temp);
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
            BackUpQueryParameter temp = param.value<BackUpQueryParameter>();
            backUpQuery(SessionID, temp);
            break;
        }
        case STARTPLAYING: {
            StartPlayingParameter temp = param.value<StartPlayingParameter>();
            setRecordStartPlayingTime(SessionID, temp);
            break;
        }
        case PLAYSTATE: {
            PlayStateParameter temp = param.value<PlayStateParameter>();
            setFastOrSlowPlayState(SessionID, temp);
            break;
        }
        case USERCONFIG: {
            UserConfigInfo temp = param.value<UserConfigInfo>();
            setUserConfig(SessionID, temp);
            break;
        }
        case ADDUSER: {
            UserConfigInfo temp = param.value<UserConfigInfo>();
            addUser(SessionID, temp);
            break;
        }
        case DELETEUSER: {
            UserConfigInfo temp = param.value<UserConfigInfo>();
            delUser(SessionID, temp);
            break;
        }
        case WIFI: {
            WifiConfigParameter temp = param.value<WifiConfigParameter>();
            setWifiConfig(SessionID, temp);
            break;
        }
        default:
            qDebug() << "#VidiconProtocol# handleSetParameter ignore signal, type:" << type;
            break;
    }
    if (m_reply->isOpen())
        m_replyMap.insert(m_reply, m_currentType);
}

void VidiconProtocol::handleGetParameter(Type type, const QVariant param, QString SessionID)
{
    qDebug() << "#VidiconProtocol# handleGetParameter type:" << type;
    switch (type) {
        case VIDEOENCODING: {
            VideoBasic temp = param.value<VideoBasic>();
            getVideoEncodingParameter(SessionID, temp);
            break;
        }
        case AUDIOENCODING: {
            getAudioEncodingParameter(SessionID);
            break;
        }
        case IMAGE: {
            getImageParameter(SessionID);
            break;
        }
        case OSD: {
            getOSDParameter(SessionID);
            break;
        }
        case TCPIP: {
            getBasicParameter(SessionID);
            break;
        }
        case OTHER: {
            getOtherParameter(SessionID);
            break;
        }
        case PPPOE: {
            getPPPOEParameter(SessionID);
            break;
        }
        case DDNS: {
            getDDNSParameter(SessionID);
            break;
        }
        case EMAIL: {
            getEmailParameter(SessionID);
            break;
        }
        case FTP: {
            getFTPParameter(SessionID);
            break;
        }
        case BONJOUR: {
            getBonjourParameter(SessionID);
            break;
        }
        case SNMP: {
            getSNMPParameter(SessionID);
            break;
        }
        case UPNP: {
            getUPNPParameter(SessionID);
            break;
        }
        case HTTPS: {
            getHTTPsParameter(SessionID);
            break;
        }
        case P2P: {
            getP2PParameter(SessionID);
            break;
        }
        case RTSP: {
            getOtherParameter(SessionID);
            break;
        }
        case MOTION: {
            getMotionDetectionParameter(SessionID);
            break;
        }
        case SENSOR: {
            getSensorAlarmParameter(SessionID);
            break;
        }
        case BLIND: {
            getVideoBlindAlarmParameter(SessionID);
            break;
        }
        case SCHEDULE: {
            getRemoteRecordingPlan(SessionID);
            break;
        }
        case SDSTORAGE: {
            getSDStorageParameter(SessionID);
            break;
        }
        case SDCARD: {
            getSDCardStatusQuery(SessionID);
            break;
        }
        case SNAPSHOT: {
            getSnapshotPlanParameter(SessionID);
            break;
        }
        case NTP: {
            getNTP(SessionID);
            break;
        }
        case GETDEVICEINFO: {
            getDeviceInfomation(SessionID);
            break;
        }
        case PULLMESSAGE: {
            getPullAlarmRequest(SessionID, 0);
            break;
        }
        case USERCONFIG: {
            getUserConfig(SessionID);
            break;
        }
        case WIFI: {
            getWifiConfig(SessionID);
            break;
        }
        default:
            qDebug() << "#VidiconProtocol# handleGetParameter ignore signal, type:" << type;
            break;
    }

    if (m_reply->isOpen())
        m_replyMap.insert(m_reply, m_currentType);
}


