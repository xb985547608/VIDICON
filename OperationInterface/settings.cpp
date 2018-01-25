#include "settings.h"
#include <QTextCodec>
#include <QCryptographicHash>

SettingsObject *SettingsObject::_instance = NULL;
SettingsObject::SettingsObject(QObject *parent) : QObject(parent)
{
    settings = new QSettings(SYSTEM_CONF_FILE, QSettings::IniFormat);
    settings->setIniCodec(QTextCodec::codecForName(DEFAULT_CODEC));

    settings->beginGroup(USER_GROUP);
    if(settings->value(NORMAL_USER, QVariant("")).toString().isEmpty()){
        QCryptographicHash c(QCryptographicHash::Md5);
        c.addData(DEFAULT_PASSWD);
        settings->setValue(NORMAL_USER, QVariant(c.result().toHex().toStdString().data()));
    }
    if(settings->value(FACTORY_USER, QVariant("")).toString().isEmpty()){
        QCryptographicHash c(QCryptographicHash::Md5);
        c.addData(DEFAULT_PASSWD);
        settings->setValue(FACTORY_USER, QVariant(c.result().toHex().toStdString().data()));
    }
    settings->endGroup();
}

SettingsObject::~SettingsObject()
{

}

QString SettingsObject::getUserMapPasswd(QString user)
{
    settings->beginGroup(USER_GROUP);
    QString passwd = settings->value(user, QVariant("")).toString();
    settings->endGroup();
    return passwd;
}
