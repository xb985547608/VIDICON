#ifndef SETTINGS_H
#define SETTINGS_H

/**
 * @brief         永久性信息存储
 * @author        xiaobin <xiaobin@sunniwell.com>
 * @date          20180504
 */

#include <QSettings>
#include "operationinterface_global.h"

#define SYSTEM_CONF_FILE        "vidicon.ini"
#define USER_GROUP         "USER"
#define FACTORY_USER       "factory"
#define NORMAL_USER        "normal"
#define DEFAULT_PASSWD     "admin"
#define DEFAULT_CODEC      "UTF-8"

class OPERATIONINTERFACESHARED_EXPORT SettingsObject : public QObject
{
    Q_OBJECT
public:
    static SettingsObject *getInstance()
    {
        if(NULL == s_instance){
            s_instance = new SettingsObject();
        }
        return s_instance;
    }
    ~SettingsObject();

    QString getUserMapPasswd(QString user);
    static QByteArray mixMD5(const QByteArray &value);

private:
    static SettingsObject *s_instance;
    explicit SettingsObject(QObject *parent = nullptr);

signals:

public slots:

private:
    QSettings *m_settings;

};

#endif // SETTINGS_H
