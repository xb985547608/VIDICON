#include "cmdongle.h"
#include <QDebug>

CMDongle::CMDongle(QObject *parent) :
    QObject(parent),
    m_isAuthorization(false),
    m_cmEntry(NULL)
{
    m_timer = new QTimer(this);
    m_timer->start(1000);
    connect(m_timer, &QTimer::timeout, this, &CMDongle::refresh);

    m_ledTimer = new QTimer(this);
    m_ledTimer->start(500);

    m_stateMachine = new QStateMachine(this);

    m_brightState = new QState(m_stateMachine);
    m_darkState = new QState(m_stateMachine);

    m_brightState->assignProperty(this, "LEDType", LedNone);
    m_darkState->assignProperty(this, "LEDType", LedNone);

    m_brightState->addTransition(m_ledTimer, SIGNAL(timeout()), m_darkState);
    m_darkState->addTransition(m_ledTimer, SIGNAL(timeout()), m_brightState);

    m_stateMachine->setInitialState(m_darkState);
    m_stateMachine->start();

    memset(&m_cmAccess, 0, sizeof(m_cmAccess));
    m_cmAccess.mflCtrl = CM_ACCESS_NOUSERLIMIT;
    m_cmAccess.mulFirmCode = FIRM_CODE;
    m_cmAccess.mulProductCode = PRODUCT_CODE;
}


void CMDongle::setBlinkInterval(int interval)
{
    if (m_ledTimer->isActive() &&
            (m_ledTimer->interval() != interval)) {
        m_ledTimer->stop();
        m_ledTimer->start(interval / 2);
    }
}

QString CMDongle::name()
{
    return "U盾";
}

void CMDongle::errorHandler()
{
    switch (CmGetLastErrorCode()) {
        case CMERROR_NO_ERROR:
            return;
        case CMERROR_BOX_NOT_FOUND:
            qDebug() << "#CMDongle# not found matching CmContainer";
            break;
        case CMERROR_ENTRY_NOT_FOUND:
            qDebug() << "#CMDongle# not found entry";
            break;
        case CMERROR_INVALID_HANDLE:
            qDebug() << "#CMDongle# CmDongle pulled out";
            break;
        default:
            char acErrText[256];
            CmGetLastErrorText(CM_GLET_ERRORTEXT, acErrText, sizeof(acErrText));
            qDebug() << "#CMDongle# unKnown error: " << acErrText;
            break;
    }

    // 针对该错误，请尝试关闭句柄。
    if (NULL != m_cmEntry) {
        CmRelease(m_cmEntry);
        m_cmEntry = NULL;
    }

    m_brightState->assignProperty(this, "LEDType", LedRed);
    m_isAuthorization = false;
}

void CMDongle::setLEDType(CMDongle::Type type)
{
    if (m_cmEntry != NULL) {
        // 现在将从锁内检索旧数据。
        // 只变更LED设定.
        int ret = CmGetInfo(m_cmEntry, CM_GEI_BOXCONTROL, &m_cmBoxCtrl, sizeof(m_cmBoxCtrl));
        if (ret == 0) {
            errorHandler();
            return;
        }
        // 设置需要的LED信息。
        m_cmPgmBoxCtrl.mflCtrl = CM_BC_ABSOLUTE;
        m_cmPgmBoxCtrl.musIndicatorFlags = ((m_cmBoxCtrl.musIndicatorFlags & 0x0fffffffc) | type);
        m_cmPgmBoxCtrl.musReserve = 0;
        ret = CmProgram(m_cmEntry, CM_GF_SET_BOXCONTROL, &m_cmPgmBoxCtrl, sizeof(m_cmPgmBoxCtrl), NULL, 0);
        if (ret == 0) {
            errorHandler();
            return;
        }
    }
}

void CMDongle::refresh()
{
    if (m_cmEntry != NULL)
        CmRelease(m_cmEntry);

    //以本地方式访问CmStick
    m_cmEntry = CmAccess(CM_ACCESS_LOCAL, &m_cmAccess);
    if (m_cmEntry == NULL) {
        errorHandler();
        return;
    }

    m_brightState->assignProperty(this, "LEDType", LedGreen);
    m_isAuthorization = true;
}
