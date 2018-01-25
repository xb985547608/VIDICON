#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include "operationinterface_global.h"

#define SYSTEM_CONF_FILE        "vidicon.ini"
#define USER_GROUP         "USER"
#define FACTORY_USER       "factory"
#define NORMAL_USER        "normal"
#define DEFAULT_PASSWD     "123456"
#define DEFAULT_CODEC      "UTF-8"

class OPERATIONINTERFACESHARED_EXPORT SettingsObject : public QObject
{
    Q_OBJECT
public:
    static SettingsObject *getInstance()
    {
        if(NULL == _instance){
            _instance = new SettingsObject();
        }
        return _instance;
    }
    ~SettingsObject();

    QString getUserMapPasswd(QString user);

private:
    static SettingsObject *_instance;
    explicit SettingsObject(QObject *parent = nullptr);

signals:

public slots:

private:
    QSettings *settings;

};

#endif // SETTINGS_H
