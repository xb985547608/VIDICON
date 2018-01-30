#include "parsexml.h"
#include <QDomDocument>
#include <QDomElement>

ParseXML *ParseXML::_instance = NULL;
ParseXML::ParseXML(QObject *parent) : QObject(parent)
{

}

bool ParseXML::parseVideoEncodingParameter(VidiconProtocol::VideoEncodingParameter *param, QByteArray data)
{
    QDomDocument *doc = new QDomDocument();
    QString errorMsg;
    int errorLine, errorColumn;

    if(!doc->setContent(data, &errorMsg, &errorLine, &errorColumn)) {
        qDebug() << "#ParseXML# parseVideoEncodingParameter Error occurred: "
                 << "errorMsg" << errorMsg
                 << "errorLine" << errorLine
                 << "errorColumn" << errorColumn;
        delete doc;
        return false;
    }

    QDomElement root = doc->documentElement();
    if(root.isNull()) {
        delete doc;
        return false;
    }

    if (root.tagName().compare("videoparam", Qt::CaseInsensitive) != 0) {
        qDebug("#ParseXML# parseVideoEncodingParameter XML data unmatched");
        delete doc;
        return false;
    }

    QDomElement child = root.firstChildElement();
    while(!child.isNull()) {
        if(child.tagName().compare("channel", Qt::CaseInsensitive) == 0) {
            param->Channel = child.text().toInt();
        }else if(child.tagName().compare("StreamType", Qt::CaseInsensitive) == 0) {
            param->StreamType = child.text().toInt();
        }else if(child.tagName().compare("VideoCodecType", Qt::CaseInsensitive) == 0) {
            param->VideoCodecType = child.text();
        }else if(child.tagName().compare("VideoResolutionWidth", Qt::CaseInsensitive) == 0) {
            param->VideoResolutionWidth = child.text().toInt();
        }else if(child.tagName().compare("VideoResolutionHeight", Qt::CaseInsensitive) == 0) {
            param->VideoResolutionHeight = child.text().toInt();
        }else if(child.tagName().compare("VideoQualityControlType", Qt::CaseInsensitive) == 0) {
            param->VideoQualityControlType = child.text();
        }else if(child.tagName().compare("ConstantBitRate", Qt::CaseInsensitive) == 0) {
            param->ConstantBitRate = child.text().toInt();
        }else if(child.tagName().compare("FixedQuality", Qt::CaseInsensitive) == 0) {
            param->FixedQuality = child.text().toInt();
        }else if(child.tagName().compare("FrameRate", Qt::CaseInsensitive) == 0) {
            param->FrameRate = child.text().toInt();
        }else if(child.tagName().compare("SnapShotImageType", Qt::CaseInsensitive) == 0) {
            param->SnapShotImageType = child.text();
        }else if(child.tagName().compare("GovLength", Qt::CaseInsensitive) == 0) {
            param->GovLength = child.text().toInt();
        }
        child = child.nextSiblingElement();
    }

    delete doc;
    return true;
}

bool ParseXML::parseAudioEncodingParameter(VidiconProtocol::AudioEncodingParameter *param, QByteArray data)
{
    QDomDocument *doc = new QDomDocument();
    QString errorMsg;
    int errorLine, errorColumn;

    if(!doc->setContent(data, &errorMsg, &errorLine, &errorColumn)) {
        qDebug() << "#ParseXML# parseAudioEncodingParameter Error occurred: "
                 << "errorMsg" << errorMsg
                 << "errorLine" << errorLine
                 << "errorColumn" << errorColumn;
        delete doc;
        return false;
    }

    QDomElement root = doc->documentElement();
    if(root.isNull()) {
        delete doc;
        return false;
    }

    if (root.tagName().compare("audioconfig", Qt::CaseInsensitive) != 0) {
        qDebug("#ParseXML# parseAudioEncodingParameter XML data unmatched");
        delete doc;
        return false;
    }

    QDomElement child = root.firstChildElement();
    while(!child.isNull()) {
        if(child.tagName().compare("Enabled", Qt::CaseInsensitive) == 0) {
            param->Enabled = child.text().toInt();
        }else if(child.tagName().compare("Encoding", Qt::CaseInsensitive) == 0) {
            param->Encoding = child.text();
        }else if(child.tagName().compare("Bitrate", Qt::CaseInsensitive) == 0) {
            param->Bitrate = child.text().toInt();
        }else if(child.tagName().compare("VideoResolutionWidth", Qt::CaseInsensitive) == 0) {
            param->SampleRate = child.text().toInt();
        }
        child = child.nextSiblingElement();
    }

    delete doc;
    return true;
}

bool ParseXML::parseOSDParameter(VidiconProtocol::OSDParameter *param, QByteArray data)
{
    QDomDocument *doc = new QDomDocument();
    QString errorMsg;
    int errorLine, errorColumn;

    if(!doc->setContent(data, &errorMsg, &errorLine, &errorColumn)) {
        qDebug() << "#ParseXML# parseAudioEncodingParameter Error occurred: "
                 << "errorMsg" << errorMsg
                 << "errorLine" << errorLine
                 << "errorColumn" << errorColumn;
        delete doc;
        return false;
    }

    QDomElement root = doc->documentElement();
    if(root.isNull()) {
        delete doc;
        return false;
    }

    if (root.tagName().compare("OSDInfo", Qt::CaseInsensitive) != 0) {
        qDebug("#ParseXML# parseOSDParameter XML data unmatched");
        delete doc;
        return false;
    }
    QDomElement child1 = root.firstChildElement();
    while(!child1.isNull()) {
        for(int i=0; i<4; i++){
            if(child1.tagName().compare(QString("osdparam_%1").arg(i), Qt::CaseInsensitive) == 0) {
                QDomElement child2 = child1.firstChildElement();
                while(!child2.isNull()){
                    if(child2.tagName().compare("osdtype", Qt::CaseInsensitive) == 0) {
                        param[i].OSDType = child2.text().toInt();
                    }else if(child2.tagName().compare("position", Qt::CaseInsensitive) == 0) {
                        QDomElement child3 = child2.firstChildElement();
                        while(!child3.isNull()){
                            if(child3.tagName().compare("osdtext", Qt::CaseInsensitive) == 0) {
                                param[i].OSDText = child3.text();
                            }else if(child3.tagName().compare("enabled", Qt::CaseInsensitive) == 0) {
                                param[i].Enabled = child3.text().toInt();
                            }else if(child3.tagName().compare("x", Qt::CaseInsensitive) == 0) {
                                param[i].x = child3.text().toInt();
                            }else if(child3.tagName().compare("y", Qt::CaseInsensitive) == 0) {
                                param[i].y = child3.text().toInt();
                            }
                            child3 = child3.nextSiblingElement();
                        }
                    }else if(child2.tagName().compare("DateFormat", Qt::CaseInsensitive) == 0) {
                        param[i].DateFormat = child2.text().toInt();
                    }else if(child2.tagName().compare("FontSize", Qt::CaseInsensitive) == 0) {
                        param[i].FontSize = child2.text().toInt();
                    }else if(child2.tagName().compare("FontColor", Qt::CaseInsensitive) == 0) {
                        param[i].FontColor = child2.text();
                    }
                    child2 = child2.nextSiblingElement();
                }
                break;
            }
        }
        child1 = child1.nextSiblingElement();
    }
    delete doc;
    return true;
}

bool ParseXML::parseNTPParameter(VidiconProtocol::NTPParameter *param, QByteArray data)
{
    QDomDocument *doc = new QDomDocument();
    QString errorMsg;
    int errorLine, errorColumn;

    if(!doc->setContent(data, &errorMsg, &errorLine, &errorColumn)) {
        qDebug() << "#ParseXML# parseAudioEncodingParameter Error occurred: "
                 << "errorMsg" << errorMsg
                 << "errorLine" << errorLine
                 << "errorColumn" << errorColumn;
        delete doc;
        return false;
    }

    QDomElement root = doc->documentElement();
    if(root.isNull()) {
        delete doc;
        return false;
    }

    if (root.tagName().compare("NTPStatus", Qt::CaseInsensitive) != 0) {
        qDebug("#ParseXML# parseNTPParameter XML data unmatched");
        delete doc;
        return false;
    }

    QDomElement child = root.firstChildElement();
    while(!child.isNull()) {
        if(child.tagName().compare("timezone", Qt::CaseInsensitive) == 0) {
            if(child.firstChildElement().tagName().compare("tz", Qt::CaseInsensitive) == 0) {
                param->TZ = child.firstChildElement().text();
            }
        }else if(child.tagName().compare("isupdatetime", Qt::CaseInsensitive) == 0) {
            param->IsUpdateTime = child.text().toInt();
        }else if(child.tagName().compare("enabled", Qt::CaseInsensitive) == 0) {
            param->Enabled = child.text().toInt();
        }else if(child.tagName().compare("utcdatetime", Qt::CaseInsensitive) == 0) {
            param->UTCDateTime = child.text();
        }else if(child.tagName().compare("ntpserver", Qt::CaseInsensitive) == 0) {
            param->NTPServer = child.text();
        }
        child = child.nextSiblingElement();
    }

    delete doc;
    return true;
}

bool ParseXML::parseDeviceInfo(VidiconProtocol::DeviceInfo *param, QByteArray data)
{
    QDomDocument *doc = new QDomDocument();
    QString errorMsg;
    int errorLine, errorColumn;

    if(!doc->setContent(data, &errorMsg, &errorLine, &errorColumn)) {
        qDebug() << "#ParseXML# parseAudioEncodingParameter Error occurred: "
                 << "errorMsg" << errorMsg
                 << "errorLine" << errorLine
                 << "errorColumn" << errorColumn;
        delete doc;
        return false;
    }

    QDomElement root = doc->documentElement();
    if(root.isNull()) {
        delete doc;
        return false;
    }

    if (root.tagName().compare("DeviceInfo", Qt::CaseInsensitive) != 0) {
        qDebug("#ParseXML# parseDeviceInfo XML data unmatched");
        delete doc;
        return false;
    }

    QDomElement child = root.firstChildElement();
    while(!child.isNull()) {
        if(child.tagName().compare("DeviceName", Qt::CaseInsensitive) == 0) {
            param->DeviceName = child.text();
        }else if(child.tagName().compare("DeviceID", Qt::CaseInsensitive) == 0) {
            param->DeviceID = child.text().toInt();
        }else if(child.tagName().compare("DeviceModel", Qt::CaseInsensitive) == 0) {
            param->DeviceModel = child.text();
        }else if(child.tagName().compare("SoftwareVer", Qt::CaseInsensitive) == 0) {
            param->SoftwareVer = child.text();
        }else if(child.tagName().compare("FactoryInfo", Qt::CaseInsensitive) == 0) {
            param->FactoryInfo = child.text();
        }
        child = child.nextSiblingElement();
    }

    delete doc;
    return true;
}

bool ParseXML::parseImageParameter(VidiconProtocol::ImageParameter *param, QByteArray data)
{
    QDomDocument *doc = new QDomDocument();
    QString errorMsg;
    int errorLine, errorColumn;

    if(!doc->setContent(data, &errorMsg, &errorLine, &errorColumn)) {
        qDebug() << "#ParseXML# parseAudioEncodingParameter Error occurred: "
                 << "errorMsg" << errorMsg
                 << "errorLine" << errorLine
                 << "errorColumn" << errorColumn;
        delete doc;
        return false;
    }

    QDomElement root = doc->documentElement();
    if(root.isNull()) {
        delete doc;
        return false;
    }

    if (root.tagName().compare("ImageChannel", Qt::CaseInsensitive) != 0) {
        qDebug("#ParseXML# parseImageParameter XML data unmatched");
        delete doc;
        return false;
    }

    QDomElement child1 = root.firstChildElement();
    while(!child1.isNull()) {
        if(child1.tagName().compare("ImageFlip", Qt::CaseInsensitive) == 0) {
            QDomElement child2 = child1.firstChildElement();
            while(!child2.isNull()) {
                if(child2.tagName().compare("Mirror", Qt::CaseInsensitive) == 0) {
                    param->Mirror = child2.text().toInt();
                }else if(child2.tagName().compare("Turn", Qt::CaseInsensitive) == 0) {
                    param->Turn = child2.text().toInt();
                }
                child2 = child2.nextSiblingElement();
            }
        }else if(child1.tagName().compare("Vision", Qt::CaseInsensitive) == 0) {
            if(child1.firstChildElement().tagName().compare("VisionMode", Qt::CaseInsensitive) == 0) {
                param->VisionMode = child1.firstChildElement().text().toInt();
            }
        }else if(child1.tagName().compare("Exposure", Qt::CaseInsensitive) == 0) {
            QDomElement child2 = child1.firstChildElement();
            while(!child2.isNull()) {
                if(child2.tagName().compare("ExposureMode", Qt::CaseInsensitive) == 0) {
                    param->ExposureMode = child2.text().toInt();
                }else if(child2.tagName().compare("Shutter", Qt::CaseInsensitive) == 0) {
                    param->Shutter = child2.text().toInt();
                }
                child2 = child2.nextSiblingElement();
            }
        }else if(child1.tagName().compare("IrcutFilter", Qt::CaseInsensitive) == 0) {
            if(child1.firstChildElement().tagName().compare("IrcutFilterMode", Qt::CaseInsensitive) == 0) {
                param->IrcutFilterMode = child1.firstChildElement().text().toInt();
            }
        }else if(child1.tagName().compare("AntiFlash", Qt::CaseInsensitive) == 0) {
            if(child1.firstChildElement().tagName().compare("AntiFlashMode", Qt::CaseInsensitive) == 0) {
                param->AntiFlashMode = child1.firstChildElement().text().toInt();
            }
        }else if(child1.tagName().compare("BLC", Qt::CaseInsensitive) == 0) {
            QDomElement child2 = child1.firstChildElement();
            while(!child2.isNull()) {
                if(child2.tagName().compare("BLCMode", Qt::CaseInsensitive) == 0) {
                    param->BLCMode = child2.text().toInt();
                }else if(child2.tagName().compare("BLCIntensity", Qt::CaseInsensitive) == 0) {
                    param->BLCIntensity = child2.text().toInt();
                }else if(child2.tagName().compare("WDRIntensity", Qt::CaseInsensitive) == 0) {
                    param->WDRIntensity = child2.text().toInt();
                }else if(child2.tagName().compare("HLCIntensity", Qt::CaseInsensitive) == 0) {
                    param->HLCIntensity = child2.text().toInt();
                }else if(child2.tagName().compare("DWDRIntensity", Qt::CaseInsensitive) == 0) {
                    param->DWDRIntensity = child2.text().toInt();
                }
                child2 = child2.nextSiblingElement();
            }
        }else if(child1.tagName().compare("LED", Qt::CaseInsensitive) == 0) {
            QDomElement child2 = child1.firstChildElement();
            while(!child2.isNull()) {
                if(child2.tagName().compare("LedStrength", Qt::CaseInsensitive) == 0) {
//                    param-> = child2.text().toInt();
                }else if(child2.tagName().compare("Lux", Qt::CaseInsensitive) == 0) {
//                    param->BLCIntensity = child2.text().toInt();
                }else if(child2.tagName().compare("Sensitivity", Qt::CaseInsensitive) == 0) {
//                    param->WDRIntensity = child2.text().toInt();
                }
                child2 = child2.nextSiblingElement();
            }
        }else if(child1.tagName().compare("NoiseReduce", Qt::CaseInsensitive) == 0) {
            if(child1.firstChildElement().tagName().compare("NoiseReduceMode", Qt::CaseInsensitive) == 0) {
                param->NoiseReduceMode = child1.firstChildElement().text().toInt();
            }
        }else if(child1.tagName().compare("LDC", Qt::CaseInsensitive) == 0) {
            if(child1.firstChildElement().tagName().compare("LDCEnabled", Qt::CaseInsensitive) == 0) {
                param->LDCEnabled = child1.firstChildElement().text().toInt();
            }
        }else if(child1.tagName().compare("Color", Qt::CaseInsensitive) == 0) {
            QDomElement child2 = child1.firstChildElement();
            while(!child2.isNull()) {
                if(child2.tagName().compare("BrightnessLevel", Qt::CaseInsensitive) == 0) {
                    param->BrightnessLevel = child2.text().toInt();
                }else if(child2.tagName().compare("ContrastLevel", Qt::CaseInsensitive) == 0) {
                    param->ContrastLevel = child2.text().toInt();
                }else if(child2.tagName().compare("SaturationLevel", Qt::CaseInsensitive) == 0) {
                    param->SaturationLevel = child2.text().toInt();
                }else if(child2.tagName().compare("HueLevel", Qt::CaseInsensitive) == 0) {
                    param->HueLevel = child2.text().toInt();
                }else if(child2.tagName().compare("Sharpness", Qt::CaseInsensitive) == 0) {
                    param->Sharpness = child2.text().toInt();
                }
                child2 = child2.nextSiblingElement();
            }
        }else if(child1.tagName().compare("WhiteBalance", Qt::CaseInsensitive) == 0) {
            QDomElement child2 = child1.firstChildElement();
            while(!child2.isNull()) {
                if(child2.tagName().compare("WhiteBalanceMode", Qt::CaseInsensitive) == 0) {
//                    param->BrightnessLevel = child2.text().toInt();
                }else if(child2.tagName().compare("BlueGain", Qt::CaseInsensitive) == 0) {
//                    param->ContrastLevel = child2.text().toInt();
                }else if(child2.tagName().compare("RedGain", Qt::CaseInsensitive) == 0) {
//                    param->SaturationLevel = child2.text().toInt();
                }
                child2 = child2.nextSiblingElement();
            }
        }else if(child1.tagName().compare("AeRange", Qt::CaseInsensitive) == 0) {
            QDomElement child2 = child1.firstChildElement();
            while(!child2.isNull()) {
                if(child2.tagName().compare("AGain", Qt::CaseInsensitive) == 0) {
//                    param->BrightnessLevel = child2.text().toInt();
                }else if(child2.tagName().compare("AGain", Qt::CaseInsensitive) == 0) {
//                    param->ContrastLevel = child2.text().toInt();
                }else if(child2.tagName().compare("ISPDGain", Qt::CaseInsensitive) == 0) {
//                    param->SaturationLevel = child2.text().toInt();
                }else if(child2.tagName().compare("SysGain", Qt::CaseInsensitive) == 0) {
//                    param->SaturationLevel = child2.text().toInt();
                }
                child2 = child2.nextSiblingElement();
            }
        }
        child1 = child1.nextSiblingElement();
    }

    delete doc;
    return true;
}

bool ParseXML::parseBasicParameter(VidiconProtocol::BasicParameter *param, QByteArray data)
{
    QDomDocument *doc = new QDomDocument();
    QString errorMsg;
    int errorLine, errorColumn;

    if(!doc->setContent(data, &errorMsg, &errorLine, &errorColumn)) {
        qDebug() << "#ParseXML# parseAudioEncodingParameter Error occurred: "
                 << "errorMsg" << errorMsg
                 << "errorLine" << errorLine
                 << "errorColumn" << errorColumn;
        delete doc;
        return false;
    }

    QDomElement root = doc->documentElement();
    if(root.isNull()) {
        delete doc;
        return false;
    }

    if (root.tagName().compare("NetworkConfig", Qt::CaseInsensitive) != 0) {
        qDebug("#ParseXML# parseBasicParameter XML data unmatched");
        delete doc;
        return false;
    }

    QDomElement child1 = root.firstChildElement();
    while(!child1.isNull()) {
        if(child1.tagName().compare("NetLink", Qt::CaseInsensitive) == 0) {
            QDomElement child2 = child1.firstChildElement();
            while(!child2.isNull()) {
                if(child2.tagName().compare("MaxLink", Qt::CaseInsensitive) == 0) {
                    param->MaxLink = child2.text().toInt();
                }else if(child2.tagName().compare("DHCP", Qt::CaseInsensitive) == 0) {
                    param->DHCP = child2.text().toInt();
                }else if(child2.tagName().compare("Name", Qt::CaseInsensitive) == 0) {
                    param->Name = child2.text();
                }else if(child2.tagName().compare("Ipv4", Qt::CaseInsensitive) == 0) {
                    QDomElement child3 = child2.firstChildElement();
                    while(!child3.isNull()) {
                        if(child3.tagName().compare("IpAddr", Qt::CaseInsensitive) == 0) {
                            param->ipv4.IpAddr = child3.text();
                        }else if(child3.tagName().compare("SubnetMask", Qt::CaseInsensitive) == 0) {
                            param->ipv4.SubnetMask = child3.text();
                        }else if(child3.tagName().compare("Gateway", Qt::CaseInsensitive) == 0) {
                            param->ipv4.Gateway = child3.text();
                        }else if(child3.tagName().compare("DNS1", Qt::CaseInsensitive) == 0) {
                            param->ipv4.DNS1 = child3.text();
                        }else if(child3.tagName().compare("DNS2", Qt::CaseInsensitive) == 0) {
                            param->ipv4.DNS2 = child3.text();
                        }
                        child3 = child3.nextSiblingElement();
                    }
                }else if(child2.tagName().compare("Ipv6", Qt::CaseInsensitive) == 0) {
                    QDomElement child3 = child2.firstChildElement();
                    while(!child3.isNull()) {
                        if(child3.tagName().compare("IpAddr", Qt::CaseInsensitive) == 0) {
                            param->ipv6.IpAddr = child3.text();
                        }else if(child3.tagName().compare("Ipv6PrefixLength", Qt::CaseInsensitive) == 0) {
                            param->ipv6.Ipv6PrefixLength = child3.text().toInt();
                        }else if(child3.tagName().compare("Gateway", Qt::CaseInsensitive) == 0) {
                            param->ipv6.Gateway = child3.text();
                        }else if(child3.tagName().compare("DNS1", Qt::CaseInsensitive) == 0) {
                            param->ipv6.DNS1 = child3.text();
                        }else if(child3.tagName().compare("DNS2", Qt::CaseInsensitive) == 0) {
                            param->ipv6.DNS2 = child3.text();
                        }
                        child3 = child3.nextSiblingElement();
                    }
                }else if(child2.tagName().compare("MACAddress", Qt::CaseInsensitive) == 0) {
                    param->MACAddress = child2.text();
                }
                child2 = child2.nextSiblingElement();
            }
        }
        child1 = child1.nextSiblingElement();
    }

    delete doc;
    return true;
}

bool ParseXML::parseOtherParameter(VidiconProtocol::OtherParameter *param, QByteArray data)
{
    QDomDocument *doc = new QDomDocument();
    QString errorMsg;
    int errorLine, errorColumn;

    if(!doc->setContent(data, &errorMsg, &errorLine, &errorColumn)) {
        qDebug() << "#ParseXML# parseAudioEncodingParameter Error occurred: "
                 << "errorMsg" << errorMsg
                 << "errorLine" << errorLine
                 << "errorColumn" << errorColumn;
        delete doc;
        return false;
    }

    QDomElement root = doc->documentElement();
    if(root.isNull()) {
        delete doc;
        return false;
    }

    if (root.tagName().compare("ExtServer", Qt::CaseInsensitive) != 0) {
        qDebug("#ParseXML# parseOtherParameter XML data unmatched");
        delete doc;
        return false;
    }

    QDomElement child1 = root.firstChildElement();
    VidiconProtocol::OtherBasicParameter *param1 = (VidiconProtocol::OtherBasicParameter *)(param);
    while(!child1.isNull()) {
        for(int i=1; i<=3; i++) {
            if(child1.tagName().compare(QString("Option_%1").arg(i), Qt::CaseInsensitive) == 0) {
                QDomElement child2 = child1.firstChildElement();
                while(!child2.isNull()) {
                    if(child2.tagName().compare("ServerType", Qt::CaseInsensitive) == 0) {
                        param1->ServerType = child2.text().toInt();
                    }else if(child2.tagName().compare("Enabled", Qt::CaseInsensitive) == 0) {
                        param1->Enabled = child2.text().toInt();
                    }else if(child2.tagName().compare("Port", Qt::CaseInsensitive) == 0) {
                        param1->Port = child2.text().toInt();
                    }
                    child2 = child2.nextSiblingElement();
                }
                param1++;
            }
        }
        child1 = child1.nextSiblingElement();
    }

    delete doc;
    return true;
}

bool ParseXML::parsePPPOEParameter(VidiconProtocol::PPPOEParameter *param, QByteArray data)
{
    QDomDocument *doc = new QDomDocument();
    QString errorMsg;
    int errorLine, errorColumn;

    if(!doc->setContent(data, &errorMsg, &errorLine, &errorColumn)) {
        qDebug() << "#ParseXML# parseAudioEncodingParameter Error occurred: "
                 << "errorMsg" << errorMsg
                 << "errorLine" << errorLine
                 << "errorColumn" << errorColumn;
        delete doc;
        return false;
    }

    QDomElement root = doc->documentElement();
    if(root.isNull()) {
        delete doc;
        return false;
    }

    if (root.tagName().compare("PPPOEStatus", Qt::CaseInsensitive) != 0) {
        qDebug("#ParseXML# parsePPPOEParameter XML data unmatched");
        delete doc;
        return false;
    }

    QDomElement child = root.firstChildElement();
    while(!child.isNull()) {
        if(child.tagName().compare("Enabled", Qt::CaseInsensitive) == 0) {
            param->Enabled = child.text().toInt();
        }else if(child.tagName().compare("PPPOEName", Qt::CaseInsensitive) == 0) {
            param->PPPOEName = child.text();
        }else if(child.tagName().compare("PPPOEPassword", Qt::CaseInsensitive) == 0) {
            param->PPPOEPassword = child.text();
        }
        child = child.nextSiblingElement();
    }

    delete doc;
    return true;
}

bool ParseXML::parseDDNSParameter(VidiconProtocol::DDNSParameter *param, QByteArray data)
{
    QDomDocument *doc = new QDomDocument();
    QString errorMsg;
    int errorLine, errorColumn;

    if(!doc->setContent(data, &errorMsg, &errorLine, &errorColumn)) {
        qDebug() << "#ParseXML# parseAudioEncodingParameter Error occurred: "
                 << "errorMsg" << errorMsg
                 << "errorLine" << errorLine
                 << "errorColumn" << errorColumn;
        delete doc;
        return false;
    }

    QDomElement root = doc->documentElement();
    if(root.isNull()) {
        delete doc;
        return false;
    }

    if (root.tagName().compare("DDNSStatus", Qt::CaseInsensitive) != 0) {
        qDebug("#ParseXML# parseDDNSParameter XML data unmatched");
        delete doc;
        return false;
    }

    QDomElement child = root.firstChildElement();
    while(!child.isNull()) {
        if(child.tagName().compare("Enabled", Qt::CaseInsensitive) == 0) {
            param->Enabled = child.text().toInt();
        }else if(child.tagName().compare("DDNSType", Qt::CaseInsensitive) == 0) {
            param->DDNSType = child.text();
        }else if(child.tagName().compare("DDNSServerName", Qt::CaseInsensitive) == 0) {
            param->DDNSServerName = child.text();
        }else if(child.tagName().compare("DDNSName", Qt::CaseInsensitive) == 0) {
            param->DDNSName = child.text();
        }else if(child.tagName().compare("DDNSUser", Qt::CaseInsensitive) == 0) {
            param->DDNSUser = child.text();
        }else if(child.tagName().compare("DDNSPassword", Qt::CaseInsensitive) == 0) {
            param->DDNSPassword = child.text();
        }
        child = child.nextSiblingElement();
    }

    delete doc;
    return true;
}

bool ParseXML::parseEmailParameter(VidiconProtocol::EmailParameter *param, QByteArray data)
{
    QDomDocument *doc = new QDomDocument();
    QString errorMsg;
    int errorLine, errorColumn;

    if(!doc->setContent(data, &errorMsg, &errorLine, &errorColumn)) {
        qDebug() << "#ParseXML# parseAudioEncodingParameter Error occurred: "
                 << "errorMsg" << errorMsg
                 << "errorLine" << errorLine
                 << "errorColumn" << errorColumn;
        delete doc;
        return false;
    }

    QDomElement root = doc->documentElement();
    if(root.isNull()) {
        delete doc;
        return false;
    }

    if (root.tagName().compare("EmailStatus", Qt::CaseInsensitive) != 0) {
        qDebug("#ParseXML# parseEmailParameter XML data unmatched");
        delete doc;
        return false;
    }

    QDomElement child = root.firstChildElement();
    while(!child.isNull()) {
        if(child.tagName().compare("Enabled", Qt::CaseInsensitive) == 0) {
            param->Enabled = child.text().toInt();
        }else if(child.tagName().compare("MotionAlarmTitle", Qt::CaseInsensitive) == 0) {
            param->MotionAlarmTitle = child.text();
        }else if(child.tagName().compare("SensorAlarmTitle", Qt::CaseInsensitive) == 0) {
            param->SensorAlarmTitle = child.text();
        }else if(child.tagName().compare("SmtpServer", Qt::CaseInsensitive) == 0) {
            param->SmtpServer = child.text();
        }else if(child.tagName().compare("SmtpUser", Qt::CaseInsensitive) == 0) {
            param->SmtpUser = child.text();
        }else if(child.tagName().compare("SmtpPassword", Qt::CaseInsensitive) == 0) {
            param->SmtpPassword = child.text();
        }else if(child.tagName().compare("Sender", Qt::CaseInsensitive) == 0) {
            param->Sender = child.text();
        }else if(child.tagName().compare("Receiver_1", Qt::CaseInsensitive) == 0) {
            param->Receiver_1 = child.text();
        }else if(child.tagName().compare("Receiver_2", Qt::CaseInsensitive) == 0) {
            param->Receiver_2 = child.text();
        }else if(child.tagName().compare("Receiver_3", Qt::CaseInsensitive) == 0) {
            param->Receiver_3 = child.text();
        }else if(child.tagName().compare("Receiver_4", Qt::CaseInsensitive) == 0) {
            param->Receiver_4 = child.text();
        }else if(child.tagName().compare("SmtpPort", Qt::CaseInsensitive) == 0) {
            param->SmtpPort = child.text().toInt();
        }
        child = child.nextSiblingElement();
    }

    delete doc;
    return true;
}

bool ParseXML::parseFTPParameter(VidiconProtocol::FTPParameter *param, QByteArray data)
{
    QDomDocument *doc = new QDomDocument();
    QString errorMsg;
    int errorLine, errorColumn;

    if(!doc->setContent(data, &errorMsg, &errorLine, &errorColumn)) {
        qDebug() << "#ParseXML# parseAudioEncodingParameter Error occurred: "
                 << "errorMsg" << errorMsg
                 << "errorLine" << errorLine
                 << "errorColumn" << errorColumn;
        delete doc;
        return false;
    }

    QDomElement root = doc->documentElement();
    if(root.isNull()) {
        delete doc;
        return false;
    }

    if (root.tagName().compare("FTPStatus", Qt::CaseInsensitive) != 0) {
        qDebug("#ParseXML# parseFTPParameter XML data unmatched");
        delete doc;
        return false;
    }

    QDomElement child = root.firstChildElement();
    while(!child.isNull()) {
        if(child.tagName().compare("Enabled", Qt::CaseInsensitive) == 0) {
            param->Enabled = child.text().toInt();
        }else if(child.tagName().compare("FTPMode", Qt::CaseInsensitive) == 0) {
            param->FTPMode = child.text().toInt();
        }else if(child.tagName().compare("FTPServer", Qt::CaseInsensitive) == 0) {
            param->FTPServer = child.text();
        }else if(child.tagName().compare("FTPUser", Qt::CaseInsensitive) == 0) {
            param->FTPUser = child.text();
        }else if(child.tagName().compare("FTPPassword", Qt::CaseInsensitive) == 0) {
            param->FTPPassword = child.text();
        }else if(child.tagName().compare("UploadDirectory", Qt::CaseInsensitive) == 0) {
            param->UploadDirectory = child.text();
        }else if(child.tagName().compare("FTPPort", Qt::CaseInsensitive) == 0) {
            param->FTPPort = child.text().toInt();
        }
        child = child.nextSiblingElement();
    }

    delete doc;
    return true;
}

bool ParseXML::parseBonjourParameter(VidiconProtocol::BonjourParameter *param, QByteArray data)
{
    QDomDocument *doc = new QDomDocument();
    QString errorMsg;
    int errorLine, errorColumn;

    if(!doc->setContent(data, &errorMsg, &errorLine, &errorColumn)) {
        qDebug() << "#ParseXML# parseAudioEncodingParameter Error occurred: "
                 << "errorMsg" << errorMsg
                 << "errorLine" << errorLine
                 << "errorColumn" << errorColumn;
        delete doc;
        return false;
    }

    QDomElement root = doc->documentElement();
    if(root.isNull()) {
        delete doc;
        return false;
    }

    if (root.tagName().compare("BonjourStatus", Qt::CaseInsensitive) != 0) {
        qDebug("#ParseXML# parseBonjourParameter XML data unmatched");
        delete doc;
        return false;
    }

    QDomElement child = root.firstChildElement();
    while(!child.isNull()) {
        if(child.tagName().compare("Enabled", Qt::CaseInsensitive) == 0) {
            param->Enabled = child.text().toInt();
        }else if(child.tagName().compare("Name", Qt::CaseInsensitive) == 0) {
            param->Name = child.text();
        }
        child = child.nextSiblingElement();
    }

    delete doc;
    return true;
}

bool ParseXML::parseSNMPParameter(VidiconProtocol::SNMPParameter *param, QByteArray data)
{
    QDomDocument *doc = new QDomDocument();
    QString errorMsg;
    int errorLine, errorColumn;

    if(!doc->setContent(data, &errorMsg, &errorLine, &errorColumn)) {
        qDebug() << "#ParseXML# parseAudioEncodingParameter Error occurred: "
                 << "errorMsg" << errorMsg
                 << "errorLine" << errorLine
                 << "errorColumn" << errorColumn;
        delete doc;
        return false;
    }

    QDomElement root = doc->documentElement();
    if(root.isNull()) {
        delete doc;
        return false;
    }

    if (root.tagName().compare("SNMPStatus", Qt::CaseInsensitive) != 0) {
        qDebug("#ParseXML# parseSNMPParameter XML data unmatched");
        delete doc;
        return false;
    }

    QDomElement child = root.firstChildElement();
    while(!child.isNull()) {
        if(child.tagName().compare("Enabled", Qt::CaseInsensitive) == 0) {
            QDomElement child2 = child.firstChildElement();
            while(!child2.isNull()) {
                if(child2.tagName().compare("Ver1", Qt::CaseInsensitive) == 0) {
                    param->EnabledVer1 = child2.text().toInt();
                }else if(child2.tagName().compare("Ver2", Qt::CaseInsensitive) == 0) {
                    param->EnabledVer2 = child2.text().toInt();
                }
                child2 = child2.nextSiblingElement();
            }
        }else if(child.tagName().compare("ReadPublic", Qt::CaseInsensitive) == 0) {
            param->ReadPublic = child.text();
        }else if(child.tagName().compare("WritePublic", Qt::CaseInsensitive) == 0) {
            param->WritePublic = child.text();
        }else if(child.tagName().compare("TrapAddress", Qt::CaseInsensitive) == 0) {
            param->TrapAddress = child.text();
        }else if(child.tagName().compare("SnmpPort", Qt::CaseInsensitive) == 0) {
            param->SnmpPort = child.text().toInt();
        }else if(child.tagName().compare("TrapPort", Qt::CaseInsensitive) == 0) {
            param->TrapPort = child.text().toInt();
        }
        child = child.nextSiblingElement();
    }

    delete doc;
    return true;
}

bool ParseXML::parseUPNPParameter(VidiconProtocol::UPNPParameter *param, QByteArray data)
{
    QDomDocument *doc = new QDomDocument();
    QString errorMsg;
    int errorLine, errorColumn;

    if(!doc->setContent(data, &errorMsg, &errorLine, &errorColumn)) {
        qDebug() << "#ParseXML# parseAudioEncodingParameter Error occurred: "
                 << "errorMsg" << errorMsg
                 << "errorLine" << errorLine
                 << "errorColumn" << errorColumn;
        delete doc;
        return false;
    }

    QDomElement root = doc->documentElement();
    if(root.isNull()) {
        delete doc;
        return false;
    }

    if (root.tagName().compare("UPNPStatus", Qt::CaseInsensitive) != 0) {
        qDebug("#ParseXML# parseUPNPParameter XML data unmatched");
        delete doc;
        return false;
    }

    QDomElement child = root.firstChildElement();
    while(!child.isNull()) {
        if(child.tagName().compare("Enabled", Qt::CaseInsensitive) == 0) {
            param->Enabled = child.text().toInt();
        }
        child = child.nextSiblingElement();
    }

    delete doc;
    return true;
}

bool ParseXML::parseHTTPsParameter(VidiconProtocol::HTTPsParameter *param, QByteArray data)
{
    QDomDocument *doc = new QDomDocument();
    QString errorMsg;
    int errorLine, errorColumn;

    if(!doc->setContent(data, &errorMsg, &errorLine, &errorColumn)) {
        qDebug() << "#ParseXML# parseAudioEncodingParameter Error occurred: "
                 << "errorMsg" << errorMsg
                 << "errorLine" << errorLine
                 << "errorColumn" << errorColumn;
        delete doc;
        return false;
    }

    QDomElement root = doc->documentElement();
    if(root.isNull()) {
        delete doc;
        return false;
    }

    if (root.tagName().compare("HTTPsStatus", Qt::CaseInsensitive) != 0) {
        qDebug("#ParseXML# parseHTTPsParameter XML data unmatched");
        delete doc;
        return false;
    }

    QDomElement child = root.firstChildElement();
    while(!child.isNull()) {
        if(child.tagName().compare("Enabled", Qt::CaseInsensitive) == 0) {
            param->Enabled = child.text().toInt();
        }else if(child.tagName().compare("HTTPsPort", Qt::CaseInsensitive) == 0) {
            param->HTTPsPort = child.text().toInt();
        }
        child = child.nextSiblingElement();
    }

    delete doc;
    return true;
}

bool ParseXML::parseP2PParameter(VidiconProtocol::P2PParameter *param, QByteArray data)
{
    QDomDocument *doc = new QDomDocument();
    QString errorMsg;
    int errorLine, errorColumn;

    if(!doc->setContent(data, &errorMsg, &errorLine, &errorColumn)) {
        qDebug() << "#ParseXML# parseAudioEncodingParameter Error occurred: "
                 << "errorMsg" << errorMsg
                 << "errorLine" << errorLine
                 << "errorColumn" << errorColumn;
        delete doc;
        return false;
    }

    QDomElement root = doc->documentElement();
    if(root.isNull()) {
        delete doc;
        return false;
    }

    if (root.tagName().compare("P2PStatus", Qt::CaseInsensitive) != 0) {
        qDebug("#ParseXML# parseP2PParameter XML data unmatched");
        delete doc;
        return false;
    }

    QDomElement child = root.firstChildElement();
    while(!child.isNull()) {
        if(child.tagName().compare("Enabled", Qt::CaseInsensitive) == 0) {
            param->Enabled = child.text().toInt();
        }else if(child.tagName().compare("P2PUUID", Qt::CaseInsensitive) == 0) {
            param->P2PUUID = child.text();
        }
        child = child.nextSiblingElement();
    }

    delete doc;
    return true;
}

bool ParseXML::parseMotionParameter(VidiconProtocol::MotionDetectionParameter *param, QByteArray data)
{
    QDomDocument *doc = new QDomDocument();
    QString errorMsg;
    int errorLine, errorColumn;

    if(!doc->setContent(data, &errorMsg, &errorLine, &errorColumn)) {
        qDebug() << "#ParseXML# parseAudioEncodingParameter Error occurred: "
                 << "errorMsg" << errorMsg
                 << "errorLine" << errorLine
                 << "errorColumn" << errorColumn;
        delete doc;
        return false;
    }

    QDomElement root = doc->documentElement();
    if(root.isNull()) {
        delete doc;
        return false;
    }

    if (root.tagName().compare("MotionDetectionParam", Qt::CaseInsensitive) != 0) {
        qDebug("#ParseXML# parseMotionParameter XML data unmatched");
        delete doc;
        return false;
    }
    QDomElement child = root.firstChildElement();
    while(!child.isNull()) {
        if(child.tagName().compare("Enabled", Qt::CaseInsensitive) == 0) {
            param->Enabled = child.text().toInt();
        }else if(child.tagName().compare("AlarmOutput", Qt::CaseInsensitive) == 0) {
            param->AlarmOutput = child.text().toInt();
        }else if(child.tagName().compare("VideoOutput", Qt::CaseInsensitive) == 0) {
            param->VideoOutput = child.text().toInt();
        }else if(child.tagName().compare("AlarmDuration", Qt::CaseInsensitive) == 0) {
            param->AlarmDuration = child.text().toInt();
        }else if(child.tagName().compare("AlarmThreshold", Qt::CaseInsensitive) == 0) {
            param->AlarmThreshold = child.text().toInt();
        }else if(child.tagName().compare("Sensitivity", Qt::CaseInsensitive) == 0) {
            param->Sensitivity = child.text().toInt();
        }else if(child.tagName().compare("PreRecTime", Qt::CaseInsensitive) == 0) {
            param->PreRecTime = child.text().toInt();
        }else if(child.tagName().compare("DelayRecTime", Qt::CaseInsensitive) == 0) {
            param->DelayRecTime = child.text().toInt();
        }else if(child.tagName().compare("AreaMask", Qt::CaseInsensitive) == 0) {
            param->AreaMask = child.text();
        }

        for(int i=0; i<7; i++) {
            if(child.tagName().compare(QString("WeekDay_%1").arg(i), Qt::CaseInsensitive) == 0) {
                QDomElement child2 = child.firstChildElement();
                while(!child2.isNull()) {
                    for(int j=0; j<4; j++) {
                        if(child2.tagName().compare(QString("Section_%1").arg(j), Qt::CaseInsensitive) == 0) {
                            QDomElement child3 = child2.firstChildElement();
                            while(!child3.isNull()) {
                                if(child3.tagName().compare("PlanTimeEnabled", Qt::CaseInsensitive) == 0) {
                                    param->Plans[i][j].PlanTimeEnabled = child3.text().toInt();
                                }else if(child3.tagName().compare("BeginTime", Qt::CaseInsensitive) == 0) {
                                    QStringList list = child3.text().split(':');
                                    param->Plans[i][j].BeginTime = QTime(list.at(0).toInt(), list.at(1).toInt(), list.at(2).toInt());
                                }else if(child3.tagName().compare("EndTime", Qt::CaseInsensitive) == 0) {
                                    QStringList list = child3.text().split(':');
                                    param->Plans[i][j].EndTime = QTime(list.at(0).toInt(), list.at(1).toInt(), list.at(2).toInt());
                                }
                                child3 = child3.nextSiblingElement();
                            }
                            continue;
                        }
                    }
                    child2 = child2.nextSiblingElement();
                }
                continue;
            }
        }
        child = child.nextSiblingElement();
    }
    delete doc;
    return true;
}

bool ParseXML::parseBlindParameter(VidiconProtocol::VideoBlindAlarmParameter *param, QByteArray data)
{
    QDomDocument *doc = new QDomDocument();
    QString errorMsg;
    int errorLine, errorColumn;

    if(!doc->setContent(data, &errorMsg, &errorLine, &errorColumn)) {
        qDebug() << "#ParseXML# parseAudioEncodingParameter Error occurred: "
                 << "errorMsg" << errorMsg
                 << "errorLine" << errorLine
                 << "errorColumn" << errorColumn;
        delete doc;
        return false;
    }

    QDomElement root = doc->documentElement();
    if(root.isNull()) {
        delete doc;
        return false;
    }

    if (root.tagName().compare("VideoBlindAlarmParam", Qt::CaseInsensitive) != 0) {
        qDebug("#ParseXML# parseBlindParameter XML data unmatched");
        delete doc;
        return false;
    }
    QDomElement child = root.firstChildElement();
    while(!child.isNull()) {
        if(child.tagName().compare("Enabled", Qt::CaseInsensitive) == 0) {
            param->Enabled = child.text().toInt();
        }else if(child.tagName().compare("AlarmOutput", Qt::CaseInsensitive) == 0) {
            param->AlarmOutput = child.text().toInt();
        }else if(child.tagName().compare("VideoOutput", Qt::CaseInsensitive) == 0) {
            param->VideoOutput = child.text().toInt();
        }else if(child.tagName().compare("AlarmDuration", Qt::CaseInsensitive) == 0) {
            param->AlarmDuration = child.text().toInt();
        }else if(child.tagName().compare("Sensitivity", Qt::CaseInsensitive) == 0) {
            param->Sensitivity = child.text().toInt();
        }

        for(int i=0; i<7; i++) {
            if(child.tagName().compare(QString("WeekDay_%1").arg(i), Qt::CaseInsensitive) == 0) {
                QDomElement child2 = child.firstChildElement();
                while(!child2.isNull()) {
                    for(int j=0; j<4; j++) {
                        if(child2.tagName().compare(QString("Section_%1").arg(j), Qt::CaseInsensitive) == 0) {
                            QDomElement child3 = child2.firstChildElement();
                            while(!child3.isNull()) {
                                if(child3.tagName().compare("PlanTimeEnabled", Qt::CaseInsensitive) == 0) {
                                    param->Plans[i][j].PlanTimeEnabled = child3.text().toInt();
                                }else if(child3.tagName().compare("BeginTime", Qt::CaseInsensitive) == 0) {
                                    QStringList list = child3.text().split(':');
                                    param->Plans[i][j].BeginTime = QTime(list.at(0).toInt(), list.at(1).toInt(), list.at(2).toInt());
                                }else if(child3.tagName().compare("EndTime", Qt::CaseInsensitive) == 0) {
                                    QStringList list = child3.text().split(':');
                                    param->Plans[i][j].EndTime = QTime(list.at(0).toInt(), list.at(1).toInt(), list.at(2).toInt());
                                }
                                child3 = child3.nextSiblingElement();
                            }
                            continue;
                        }
                    }
                    child2 = child2.nextSiblingElement();
                }
                continue;
            }
        }
        child = child.nextSiblingElement();
    }
    delete doc;
    return true;
}

bool ParseXML::parseSensorParameter(VidiconProtocol::SensorAlarmParameter *param, QByteArray data)
{
    QDomDocument *doc = new QDomDocument();
    QString errorMsg;
    int errorLine, errorColumn;

    if(!doc->setContent(data, &errorMsg, &errorLine, &errorColumn)) {
        qDebug() << "#ParseXML# parseAudioEncodingParameter Error occurred: "
                 << "errorMsg" << errorMsg
                 << "errorLine" << errorLine
                 << "errorColumn" << errorColumn;
        delete doc;
        return false;
    }

    QDomElement root = doc->documentElement();
    if(root.isNull()) {
        delete doc;
        return false;
    }

    if (root.tagName().compare("SensorList", Qt::CaseInsensitive) != 0) {
        qDebug("#ParseXML# parseSensorParameter XML data unmatched");
        delete doc;
        return false;
    }
    QDomElement child = root.firstChildElement();
    while(!child.isNull()) {
        if(child.tagName().compare("Enabled", Qt::CaseInsensitive) == 0) {
            param->Enabled = child.text().toInt();
        }else if(child.tagName().compare("AlarmOutput", Qt::CaseInsensitive) == 0) {
            param->AlarmOutput = child.text().toInt();
        }else if(child.tagName().compare("VideoOutput", Qt::CaseInsensitive) == 0) {
            param->VideoOutput = child.text().toInt();
        }else if(child.tagName().compare("AlarmDuration", Qt::CaseInsensitive) == 0) {
            param->AlarmDuration = child.text().toInt();
        }else if(child.tagName().compare("SensorType", Qt::CaseInsensitive) == 0) {
            param->SensorType = child.text().toInt();
        }

        for(int i=0; i<7; i++) {
            if(child.tagName().compare(QString("WeekDay_%1").arg(i), Qt::CaseInsensitive) == 0) {
                QDomElement child2 = child.firstChildElement();
                while(!child2.isNull()) {
                    for(int j=0; j<4; j++) {
                        if(child2.tagName().compare(QString("Section_%1").arg(j), Qt::CaseInsensitive) == 0) {
                            QDomElement child3 = child2.firstChildElement();
                            while(!child3.isNull()) {
                                if(child3.tagName().compare("PlanTimeEnabled", Qt::CaseInsensitive) == 0) {
                                    param->Plans[i][j].PlanTimeEnabled = child3.text().toInt();
                                }else if(child3.tagName().compare("BeginTime", Qt::CaseInsensitive) == 0) {
                                    QStringList list = child3.text().split(':');
                                    param->Plans[i][j].BeginTime = QTime(list.at(0).toInt(), list.at(1).toInt(), list.at(2).toInt());
                                }else if(child3.tagName().compare("EndTime", Qt::CaseInsensitive) == 0) {
                                    QStringList list = child3.text().split(':');
                                    param->Plans[i][j].EndTime = QTime(list.at(0).toInt(), list.at(1).toInt(), list.at(2).toInt());
                                }
                                child3 = child3.nextSiblingElement();
                            }
                            continue;
                        }
                    }
                    child2 = child2.nextSiblingElement();
                }
                continue;
            }
        }
        child = child.nextSiblingElement();
    }
    delete doc;
    return true;
}

bool ParseXML::parseScheduleParameter(VidiconProtocol::RemoteRecordingPlan *param, QByteArray data)
{
    QDomDocument *doc = new QDomDocument();
    QString errorMsg;
    int errorLine, errorColumn;

    if(!doc->setContent(data, &errorMsg, &errorLine, &errorColumn)) {
        qDebug() << "#ParseXML# parseAudioEncodingParameter Error occurred: "
                 << "errorMsg" << errorMsg
                 << "errorLine" << errorLine
                 << "errorColumn" << errorColumn;
        delete doc;
        return false;
    }

    QDomElement root = doc->documentElement();
    if(root.isNull()) {
        delete doc;
        return false;
    }

    if (root.tagName().compare("RemoteVideoPlan", Qt::CaseInsensitive) != 0) {
        qDebug("#ParseXML# parseScheduleParameter XML data unmatched");
        delete doc;
        return false;
    }
    QDomElement child = root.firstChildElement();
    while(!child.isNull()) {
        if(child.tagName().compare("Enabled", Qt::CaseInsensitive) == 0) {
            param->Enabled = child.text().toInt();
        }

        for(int i=0; i<7; i++) {
            if(child.tagName().compare(QString("WeekDay_%1").arg(i), Qt::CaseInsensitive) == 0) {
                QDomElement child2 = child.firstChildElement();
                while(!child2.isNull()) {
                    for(int j=0; j<4; j++) {
                        if(child2.tagName().compare(QString("Section_%1").arg(j), Qt::CaseInsensitive) == 0) {
                            QDomElement child3 = child2.firstChildElement();
                            while(!child3.isNull()) {
                                if(child3.tagName().compare("PlanTimeEnabled", Qt::CaseInsensitive) == 0) {
                                    param->Plans[i][j].PlanTimeEnabled = child3.text().toInt();
                                }else if(child3.tagName().compare("BeginTime", Qt::CaseInsensitive) == 0) {
                                    QStringList list = child3.text().split(':');
                                    param->Plans[i][j].BeginTime = QTime(list.at(0).toInt(), list.at(1).toInt(), list.at(2).toInt());
                                }else if(child3.tagName().compare("EndTime", Qt::CaseInsensitive) == 0) {
                                    QStringList list = child3.text().split(':');
                                    param->Plans[i][j].EndTime = QTime(list.at(0).toInt(), list.at(1).toInt(), list.at(2).toInt());
                                }
                                child3 = child3.nextSiblingElement();
                            }
                            continue;
                        }
                    }
                    child2 = child2.nextSiblingElement();
                }
                continue;
            }
        }
        child = child.nextSiblingElement();
    }
    delete doc;
    return true;
}

bool ParseXML::parseSnapshotParameter(VidiconProtocol::SnapshotPlanParameter *param, QByteArray data)
{
    QDomDocument *doc = new QDomDocument();
    QString errorMsg;
    int errorLine, errorColumn;

    if(!doc->setContent(data, &errorMsg, &errorLine, &errorColumn)) {
        qDebug() << "#ParseXML# parseAudioEncodingParameter Error occurred: "
                 << "errorMsg" << errorMsg
                 << "errorLine" << errorLine
                 << "errorColumn" << errorColumn;
        delete doc;
        return false;
    }

    QDomElement root = doc->documentElement();
    if(root.isNull()) {
        delete doc;
        return false;
    }

    if (root.tagName().compare("SnapshotChannel", Qt::CaseInsensitive) != 0) {
        qDebug("#ParseXML# parseSnapshotParameter XML data unmatched");
        delete doc;
        return false;
    }
    QDomElement child = root.firstChildElement();
    while(!child.isNull()) {
        if(child.tagName().compare("Enabled", Qt::CaseInsensitive) == 0) {
            param->Enabled = child.text().toInt();
        }else if(child.tagName().compare("SnapIntervalTime", Qt::CaseInsensitive) == 0) {
            param->SnapIntervalTime = child.text().toInt();
        }

        for(int i=0; i<7; i++) {
            if(child.tagName().compare(QString("WeekDay_%1").arg(i), Qt::CaseInsensitive) == 0) {
                QDomElement child2 = child.firstChildElement();
                while(!child2.isNull()) {
                    for(int j=0; j<4; j++) {
                        if(child2.tagName().compare(QString("Section_%1").arg(j), Qt::CaseInsensitive) == 0) {
                            QDomElement child3 = child2.firstChildElement();
                            while(!child3.isNull()) {
                                if(child3.tagName().compare("PlanTimeEnabled", Qt::CaseInsensitive) == 0) {
                                    param->Plans[i][j].PlanTimeEnabled = child3.text().toInt();
                                }else if(child3.tagName().compare("BeginTime", Qt::CaseInsensitive) == 0) {
                                    QStringList list = child3.text().split(':');
                                    param->Plans[i][j].BeginTime = QTime(list.at(0).toInt(), list.at(1).toInt(), list.at(2).toInt());
                                }else if(child3.tagName().compare("EndTime", Qt::CaseInsensitive) == 0) {
                                    QStringList list = child3.text().split(':');
                                    param->Plans[i][j].EndTime = QTime(list.at(0).toInt(), list.at(1).toInt(), list.at(2).toInt());
                                }
                                child3 = child3.nextSiblingElement();
                            }
                            continue;
                        }
                    }
                    child2 = child2.nextSiblingElement();
                }
                continue;
            }
        }
        child = child.nextSiblingElement();
    }
    delete doc;
    return true;
}

bool ParseXML::parseSDCardStatusParameter(VidiconProtocol::SDCardStatus *param, QByteArray data)
{
    QDomDocument *doc = new QDomDocument();
    QString errorMsg;
    int errorLine, errorColumn;

    if(!doc->setContent(data, &errorMsg, &errorLine, &errorColumn)) {
        qDebug() << "#ParseXML# parseAudioEncodingParameter Error occurred: "
                 << "errorMsg" << errorMsg
                 << "errorLine" << errorLine
                 << "errorColumn" << errorColumn;
        delete doc;
        return false;
    }

    QDomElement root = doc->documentElement();
    if(root.isNull()) {
        delete doc;
        return false;
    }

    if (root.tagName().compare("QuerySDCardStat", Qt::CaseInsensitive) != 0) {
        qDebug("#ParseXML# parseSDCardStatusParameter XML data unmatched");
        delete doc;
        return false;
    }
    QDomElement child = root.firstChildElement();
    while(!child.isNull()) {
        if(child.tagName().compare("TotoalSizeKByte", Qt::CaseInsensitive) == 0) {
            param->TotoalSizeKByte = child.text().toInt();
        }else if(child.tagName().compare("UsedKByte", Qt::CaseInsensitive) == 0) {
            param->UsedKByte = child.text().toInt();
        }else if(child.tagName().compare("AvailableKByte", Qt::CaseInsensitive) == 0) {
            param->AvailableKByte = child.text().toInt();
        }

        child = child.nextSiblingElement();
    }
    delete doc;
    return true;
}

bool ParseXML::parseSDStorageParameter(VidiconProtocol::SDStorageParameter *param, QByteArray data)
{
    QDomDocument *doc = new QDomDocument();
    QString errorMsg;
    int errorLine, errorColumn;

    if(!doc->setContent(data, &errorMsg, &errorLine, &errorColumn)) {
        qDebug() << "#ParseXML# parseAudioEncodingParameter Error occurred: "
                 << "errorMsg" << errorMsg
                 << "errorLine" << errorLine
                 << "errorColumn" << errorColumn;
        delete doc;
        return false;
    }

    QDomElement root = doc->documentElement();
    if(root.isNull()) {
        delete doc;
        return false;
    }

    if (root.tagName().compare("SDStorage", Qt::CaseInsensitive) != 0) {
        qDebug("#ParseXML# parseSDStorageParameter XML data unmatched");
        delete doc;
        return false;
    }
    QDomElement child = root.firstChildElement();
    while(!child.isNull()) {
        if(child.tagName().compare("OperType", Qt::CaseInsensitive) == 0) {
            param->OperType = child.text().toInt();
        }else if(child.tagName().compare("RecordSelect", Qt::CaseInsensitive) == 0) {
            param->RecordSelect = child.text().toInt();
        }else if(child.tagName().compare("RecordMode", Qt::CaseInsensitive) == 0) {
            param->RecordMode = child.text().toInt();
        }else if(child.tagName().compare("RecordTime", Qt::CaseInsensitive) == 0) {
            param->RecordTime = child.text().toInt();
        }

        child = child.nextSiblingElement();
    }
    delete doc;
    return true;
}

bool ParseXML::parseBackUpQueryParameter(VidiconProtocol::BackUpQueryParameter *param, QByteArray data)
{
    QDomDocument *doc = new QDomDocument();
    QString errorMsg;
    int errorLine, errorColumn;

    if(!doc->setContent(data, &errorMsg, &errorLine, &errorColumn)) {
        qDebug() << "#ParseXML# parseAudioEncodingParameter Error occurred: "
                 << "errorMsg" << errorMsg
                 << "errorLine" << errorLine
                 << "errorColumn" << errorColumn;
        delete doc;
        return false;
    }

    QDomElement root = doc->documentElement();
    if(root.isNull()) {
        delete doc;
        return false;
    }

    if (root.tagName().compare("QueryBackUp", Qt::CaseInsensitive) != 0) {
        qDebug("#ParseXML# parseBackUpQueryParameter XML data unmatched");
        delete doc;
        return false;
    }
    QDomElement child = root.firstChildElement();
    switch(param->Type) {
    case 0: {
        while(!child.isNull()) {
            if(child.tagName().left(8).compare("TimePara", Qt::CaseInsensitive) == 0) {
                VidiconProtocol::TimeParameter timeParam;

                QDomElement child2 = child.firstChildElement();
                while(!child2.isNull()) {
                    if(child2.tagName().compare("StarTime", Qt::CaseInsensitive) == 0) {
                        QStringList list = child2.text().split(':');
                        timeParam.StarTime = QTime(list.at(0).toInt(), list.at(1).toInt(), list.at(2).toInt());
                    }else if(child2.tagName().compare("EndTime", Qt::CaseInsensitive) == 0) {
                        QStringList list = child2.text().split(':');
                        timeParam.EndTime = QTime(list.at(0).toInt(), list.at(1).toInt(), list.at(2).toInt());
                    }else if(child2.tagName().compare("RecordType", Qt::CaseInsensitive) == 0) {
                        timeParam.RecordType = child2.text().toInt();
                    }
                    child2 = child2.nextSiblingElement();
                }

                param->TimeParamMap->insert(child.tagName().right(1).toInt(), timeParam);
            }
            child = child.nextSiblingElement();
        }
        break;
    }
    case 1: {
        while(!child.isNull()) {
            if(child.tagName().left(8).compare("FilePara", Qt::CaseInsensitive) == 0) {
                param->fileList.append(child.firstChildElement().text());
            }
            child = child.nextSiblingElement();
        }
        break;
    }
    case 6: {
        while(!child.isNull()) {
            if(child.tagName().left(8).compare("FilePara", Qt::CaseInsensitive) == 0) {
                int day = child.tagName().mid(8).toInt();
                int exist = child.firstChildElement().text().toInt();
                param->MonthMap->insert(day, exist);
            }
            child = child.nextSiblingElement();
        }
    }
    default:
        break;
    }

    delete doc;
    return true;
}

bool ParseXML::parsePullMsg(VidiconProtocol::PullMessage *param, QByteArray data)
{
    QDomDocument *doc = new QDomDocument();
    QString errorMsg;
    int errorLine, errorColumn;

    if(!doc->setContent(data, &errorMsg, &errorLine, &errorColumn)) {
        qDebug() << "#ParseXML# parseAudioEncodingParameter Error occurred: "
                 << "errorMsg" << errorMsg
                 << "errorLine" << errorLine
                 << "errorColumn" << errorColumn;
        delete doc;
        return false;
    }

    QDomElement root = doc->documentElement();
    if(root.isNull()) {
        delete doc;
        return false;
    }

    if (root.tagName().compare("PullMsg", Qt::CaseInsensitive) != 0) {
        qDebug("#ParseXML# parsePullMsg XML data unmatched");
        delete doc;
        return false;
    }
    QDomElement child = root.firstChildElement();

    param->AlarmCount = 0;
    while(!child.isNull()) {
        if(child.tagName().compare("Channel", Qt::CaseInsensitive) == 0) {
            param->Channel = child.text().toInt();
        }else if(child.tagName().compare("MotionAlarmEnabled", Qt::CaseInsensitive) == 0) {
            param->MotionAlarmEnabled = child.text().toInt();
        }else if(child.tagName().compare("SensorAlarmEnabled", Qt::CaseInsensitive) == 0) {
            param->SensorAlarmEnabled = child.text().toInt();
        }else if(child.tagName().compare("VideoBlindAlarmEnabled", Qt::CaseInsensitive) == 0) {
            param->VideoBlindAlarmEnabled = child.text().toInt();
        }else if(child.tagName().right(5).compare("alarm", Qt::CaseInsensitive) == 0) {
            QDomElement child2 = child.firstChildElement();
            while(!child2.isNull()) {
                if(child2.tagName().compare("AlarmType", Qt::CaseInsensitive) == 0) {
                    param->msgs[param->AlarmCount].AlarmType = child2.text();
                    param->AlarmCount++;
                }
                child2 = child2.nextSiblingElement();
            }
        }
        child = child.nextSiblingElement();
    }
    delete doc;
    return true;
}

bool ParseXML::parseResponseStatus(VidiconProtocol::ResponseStatus *param, QByteArray data)
{
    QDomDocument *doc = new QDomDocument();
    QString errorMsg;
    int errorLine, errorColumn;

    if(!doc->setContent(data, &errorMsg, &errorLine, &errorColumn)) {
        qDebug() << "#ParseXML# parseAudioEncodingParameter Error occurred: "
                 << "errorMsg" << errorMsg
                 << "errorLine" << errorLine
                 << "errorColumn" << errorColumn;
        delete doc;
        return false;
    }

    QDomElement root = doc->documentElement();
    if(root.isNull()) {
        delete doc;
        return false;
    }

    if (root.tagName().compare("ResponseStatus", Qt::CaseInsensitive) != 0) {
        qDebug("#ParseXML# parseResponseStatus XML data unmatched");
        delete doc;
        return false;
    }
    QDomElement child = root.firstChildElement();
    while(!child.isNull()) {
        if(child.tagName().compare("RequestURL", Qt::CaseInsensitive) == 0) {
            param->RequestURL = child.text();
        }else if(child.tagName().compare("StatusCode", Qt::CaseInsensitive) == 0) {
            param->StatusCode = child.text().toInt();
        }else if(child.tagName().compare("StatusString", Qt::CaseInsensitive) == 0) {
            param->StatusString = child.text();
        }
        child = child.nextSiblingElement();
    }
    delete doc;
    return true;
}

bool ParseXML::parseDiscoveryDevice(SearchDeviceThread::DeviceInfo *param, QByteArray data)
{
    QDomDocument *doc = new QDomDocument();
    QString errorMsg;
    int errorLine, errorColumn;

    if(!doc->setContent(data, &errorMsg, &errorLine, &errorColumn)) {
        qDebug() << "#ParseXML# parseAudioEncodingParameter Error occurred: "
                 << "errorMsg" << errorMsg
                 << "errorLine" << errorLine
                 << "errorColumn" << errorColumn;
        delete doc;
        return false;
    }

    QDomElement root = doc->documentElement();
    if(root.isNull()) {
        delete doc;
        return false;
    }

    if (root.tagName().compare("Discovery", Qt::CaseInsensitive) != 0) {
        qDebug("#ParseXML# parseDiscoveryDevice XML data unmatched");
        delete doc;
        return false;
    }
    QDomElement child = root.firstChildElement();
    if(child.isNull()) {
        return false;
    }
    while(!child.isNull()) {
        if(child.tagName().compare("DeviceName", Qt::CaseInsensitive) == 0) {
            param->DeviceName = child.text();
        }else if(child.tagName().compare("DeviceModel", Qt::CaseInsensitive) == 0) {
            param->DeviceModel = child.text();
        }else if(child.tagName().compare("SerialNumber", Qt::CaseInsensitive) == 0) {
            param->SerialNumber = child.text();
        }else if(child.tagName().compare("SoftwareVersion", Qt::CaseInsensitive) == 0) {
            param->SoftwareVersion = child.text();
        }else if(child.tagName().compare("P2P_UID", Qt::CaseInsensitive) == 0) {
            param->P2P_UID = child.text();
        }else if(child.tagName().compare("IPAddr", Qt::CaseInsensitive) == 0) {
            param->IPAddr = child.text();
        }else if(child.tagName().compare("MacAddr", Qt::CaseInsensitive) == 0) {
            param->MacAddr = child.text();
        }else if(child.tagName().compare("SubnetMask", Qt::CaseInsensitive) == 0) {
            param->SubnetMask = child.text();
        }else if(child.tagName().compare("Gateway", Qt::CaseInsensitive) == 0) {
            param->Gateway = child.text();
        }else if(child.tagName().compare("DNSIpAddr1", Qt::CaseInsensitive) == 0) {
            param->DNSIpAddr1 = child.text();
        }else if(child.tagName().compare("DNSIpAddr2", Qt::CaseInsensitive) == 0) {
            param->DNSIpAddr2 = child.text();
        }else if(child.tagName().compare("HTTPPort", Qt::CaseInsensitive) == 0) {
            param->HTTPPort = child.text().toInt();
        }else if(child.tagName().compare("ONVIFPort", Qt::CaseInsensitive) == 0) {
            param->ONVIFPort = child.text().toInt();
        }else if(child.tagName().compare("RTSPPort", Qt::CaseInsensitive) == 0) {
            param->RTSPPort = child.text().toInt();
        }else if(child.tagName().compare("DHCPEnabled", Qt::CaseInsensitive) == 0) {
            param->DHCPEnabled = child.text().toInt();
        }else if(child.tagName().compare("Format", Qt::CaseInsensitive) == 0) {
            param->Format = child.text();
        }
        child = child.nextSiblingElement();
    }
    delete doc;
    return true;
}
