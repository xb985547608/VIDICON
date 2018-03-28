#ifndef CMDONGLE_H
#define CMDONGLE_H

#include <QStateMachine>
#include <QThread>
#include <QTimer>

#include "operationinterface_global.h"

extern "C" {
#include "CodeMeter.h"
}

//公司码or产品码
#define FIRM_CODE    10
#define PRODUCT_CODE 13

class OPERATIONINTERFACESHARED_EXPORT CMDongle : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Type LEDType WRITE setLEDType)
public:
    //灯控制
    enum Type {
        LedNone = 0,
        LedGreen,
        LedRed,
        LedYellow
    };
    Q_ENUM(Type)

    explicit CMDongle(QObject *parent = nullptr);

    void setBlinkInterval(int interval);
    inline bool isAuthorization()
    { return m_isAuthorization; }

    void errorHandler();

protected:

signals:
    void signalAuthorizationChange(bool);

public slots:
    void setLEDType(Type type);

    void refresh();

private:
    bool m_isAuthorization;

    QStateMachine *m_stateMachine;
    QState        *m_brightState;
    QState        *m_darkState;

    Type    m_ledType;
    QTimer *m_timer;
    QTimer *m_ledTimer;

    HCMSysEntry             m_cmEntry;    //加密狗入口
    CMACCESS                m_cmAccess;
    CMBOXCONTROL            m_cmBoxCtrl;
    CMPROGRAM_BOXCONTROL    m_cmPgmBoxCtrl;
};

#endif // CMDONGLE_H
