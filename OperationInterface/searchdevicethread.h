#ifndef SEARCHDEVICETHREAD_H
#define SEARCHDEVICETHREAD_H

/**
 * @brief         搜索设备线程,通过发送广播消息
 * @author        xiaobin <xiaobin@sunniwell.com>
 * @date          20180504
 */

#include <QThread>
#include <QUdpSocket>

#include <QMutex>
#include "operationinterface_global.h"

#define BROADCAST_ADDR "255.255.255.255"
#define UDPRECV_PORT 3704
#define UDPSEND_PORT 3704

struct WholeDeviceInfo{
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
};
Q_DECLARE_METATYPE(WholeDeviceInfo)

class OPERATIONINTERFACESHARED_EXPORT SearchDeviceThread : public QThread
{
    Q_OBJECT
public:
    explicit SearchDeviceThread(QObject *parent = nullptr);

    void setSpecifiedIP(const QHostAddress &ip);

signals:
    void signalDeviceInfo(QVariant value);

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
