#ifndef TABNETWORK_H
#define TABNETWORK_H

#include <QStackedWidget>
#include <QMap>

class NetworkWidget : public QStackedWidget
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

signals:
    void signalSetParameter(int type, void *param, QString SessionID = "R00001");
    void signalGetParameter(int type, int StreamType = 0, int Channel = 0, QString SessionID = "R00001");

public slots:
    void handlerSwitchTab(const QModelIndex &index);
    void handlerPrepareData();
    void handlerReceiveData(int type, QByteArray data);

private:
    QWidget *tcpIpWidget;
    QMap<QString, QWidget *> tcpIpMap;

    QWidget *PPPOEWidget;
    QMap<QString, QWidget *> PPPOEMap;

    QWidget *DDNSClientWidget;
    QMap<QString, QWidget *> DDNSClientMap;

    QWidget *EmailWidget;
    QMap<QString, QWidget *> EmailMap;

    QWidget *FTPWidget;
    QMap<QString, QWidget *> FTPMap;

    QWidget *BonjourWidget;
    QMap<QString, QWidget *> BonjourMap;

    QWidget *SNMPWidget;
    QMap<QString, QWidget *> SNMPMap;

    QWidget *UPNPWidget;
    QMap<QString, QWidget *> UPNPMap;

    QWidget *HTTPsWidget;
    QMap<QString, QWidget *> HTTPsMap;

    QWidget *P2PWidget;
    QMap<QString, QWidget *> P2PMap;

    QWidget *RTSPWidget;
    QMap<QString, QWidget *> RTSPMap;
};

#endif // TABNETWORK_H
