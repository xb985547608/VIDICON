#include "settings.h"
#include <QTextCodec>
#include <QCryptographicHash>

SettingsObject *SettingsObject::s_instance = NULL;
SettingsObject::SettingsObject(QObject *parent) : QObject(parent)
{
    m_settings = new QSettings(SYSTEM_CONF_FILE, QSettings::IniFormat);
    m_settings->setIniCodec(QTextCodec::codecForName(DEFAULT_CODEC));

    m_settings->beginGroup(USER_GROUP);
    if(m_settings->value(NORMAL_USER, QVariant("")).toString().isEmpty()){
        QCryptographicHash c(QCryptographicHash::Md5);
        c.addData(DEFAULT_PASSWD);
        m_settings->setValue(NORMAL_USER, QVariant(mixMD5(c.result()).toHex().toStdString().data()));
    }
    if(m_settings->value(FACTORY_USER, QVariant("")).toString().isEmpty()){
        QCryptographicHash c(QCryptographicHash::Md5);
        c.addData(DEFAULT_PASSWD);
        m_settings->setValue(FACTORY_USER, QVariant(mixMD5(c.result()).toHex().toStdString().data()));
    }
    m_settings->endGroup();
}

SettingsObject::~SettingsObject()
{

}

QString SettingsObject::getUserMapPasswd(QString user)
{
    m_settings->beginGroup(USER_GROUP);
    QString passwd = m_settings->value(user, QVariant("")).toString();
    m_settings->endGroup();
    return passwd;
}


//混淆MD5值 对MD5值做除2操作
QByteArray SettingsObject::mixMD5(const QByteArray &value)
{
    QByteArray temp;
    for (int i=0; i<value.length(); i++)
        temp.append(value.at(i) / 2);
    return temp;
}
