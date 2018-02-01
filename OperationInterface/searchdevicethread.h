#ifndef SEARCHDEVICETHREAD_H
#define SEARCHDEVICETHREAD_H

#include <QThread>
#include <QUdpSocket>

#include <QMutex>
#include "operationinterface_global.h"

#define UDPMULTICASTADDR "255.255.255.255"
#define UDPMULTICASTRECEIVEPORT 3704
#define UDPMULTICASTSENDPORT 3704

class OPERATIONINTERFACESHARED_EXPORT SearchDeviceThread : public QThread
{
    Q_OBJECT
public:
    typedef struct DeviceInfo{
        QString DeviceName;
        QString DeviceModel;
        QString SerialNumber;
        QString SoftwareVersion;
        QString P2P_UID;
        QString IPAddr;
        QString MacAddr;
        QString SubnetMask;
        QString Gateway;
        QString DNSIpAddr1;
        QString DNSIpAddr2;
        int     HTTPPort;
        int     ONVIFPort;
        int     RTSPPort;
        int     DHCPEnabled;
        QString Format;
    }DeviceInfo;

    explicit SearchDeviceThread(QObject *parent = nullptr);

    void setSpecifiedIP(const QHostAddress &ip);

signals:
    void signalDeviceInfo(DeviceInfo *info);
public slots:
    void handleSearchDevice();

protected:
    void run() override;
    void readyRead();

private:
    QUdpSocket *udpSocket;
    QHostAddress specifiedIP;
    bool isRun;
};

#endif // SEARCHDEVICETHREAD_H
