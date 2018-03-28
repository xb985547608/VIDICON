#ifndef TABNETWORK_H
#define TABNETWORK_H

#include <QMap>
#include "stackedwidget.h"

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
};

#endif // TABNETWORK_H
