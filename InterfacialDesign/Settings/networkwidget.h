#ifndef TABNETWORK_H
#define TABNETWORK_H

/**
 * @brief         主要对网络参数的可视化和设置
 * @author        xiaobin <xiaobin@sunniwell.com>
 * @date          20180504
 */

#include <QMap>
#include "stackedwidget.h"
#include <QRegExpValidator>

class NetworkWidget : public StackedWidget
{
    Q_OBJECT
public:
    explicit NetworkWidget(QWidget *parent = nullptr);
    ~NetworkWidget();

    void initTCPIPWidget();
    void initPPPOEWidget();
    void initDDNSClientWidget();
    void initEmailWidget();
    void initFTPWidget();
    void initBonjourWidget();
    void initSNMPWidget();
    void initUPNPWidget();
    void initHTTPsWidget();
    void initP2PWidget();
    void initRTSPWidget();

public slots:
    void setCurrentIndex(const QModelIndex &index) override;
    void handlePrepareData() override;
    void handleReceiveData(VidiconProtocol::Type type, QByteArray data) override;

    /**
     * 检查当前界面的一些输入是否合法，从而控制当前"save"按键的使能or失能
     */
    void checkInput(QString text);

private:
    QWidget *m_tcpIpWidget;
    QMap<QString, QWidget *> m_tcpIpMap;

    QWidget *m_PPPOEWidget;
    QMap<QString, QWidget *> m_PPPOEMap;

    QWidget *m_DDNSClientWidget;
    QMap<QString, QWidget *> m_DDNSClientMap;

    QWidget *m_EmailWidget;
    QMap<QString, QWidget *> m_EmailMap;

    QWidget *m_FTPWidget;
    QMap<QString, QWidget *> m_FTPMap;

    QWidget *m_BonjourWidget;
    QMap<QString, QWidget *> m_BonjourMap;

    QWidget *m_SNMPWidget;
    QMap<QString, QWidget *> m_SNMPMap;

    QWidget *m_UPNPWidget;
    QMap<QString, QWidget *> m_UPNPMap;

    QWidget *m_HTTPsWidget;
    QMap<QString, QWidget *> m_HTTPsMap;

    QWidget *m_P2PWidget;
    QMap<QString, QWidget *> m_P2PMap;

    QWidget *m_RTSPWidget;
    QMap<QString, QWidget *> m_RTSPMap;

    QList<QLineEdit *> m_notPass;
};

#endif // TABNETWORK_H
