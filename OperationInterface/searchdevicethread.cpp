#include "searchdevicethread.h"
#include <QHostAddress>
#include "parsexml.h"

SearchDeviceThread::SearchDeviceThread(QObject *parent) :
    QThread(parent),
    udpSocket(NULL),
    isRun(true)
{
    specifiedIP.setAddress(QHostAddress::AnyIPv4);
}

void SearchDeviceThread::setSpecifiedIP(const QHostAddress &ip)
{
    specifiedIP = ip;
}

void SearchDeviceThread::handleSearchDevice()
{

}

void SearchDeviceThread::run()
{
    qDebug() << "#SearchDeviceThread# start serach device";

    isRun = true;
    udpSocket = new QUdpSocket();

    while(isRun) {
        while(!(udpSocket->isValid()) || udpSocket->state() != QUdpSocket::BoundState) {
            udpSocket->abort();
            udpSocket->close();
            delete udpSocket;
            udpSocket = new QUdpSocket();

            //将套接字绑定在指定接口上(这里通常与之绑定的是无线网卡的地址
            if (!udpSocket->bind(specifiedIP, UDPMULTICASTRECEIVEPORT, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint)) {
                qDebug() << "#SearchDeviceThread# udp socket bind error!";
                sleep(1);
                continue;
            }

            //开启本地回环
            udpSocket->setSocketOption(QAbstractSocket::MulticastLoopbackOption, 1);

            //加入组播组   未知BUG  已成功加入组播组还是返回false
            if (!udpSocket->joinMulticastGroup(QHostAddress(UDPMULTICASTADDR))) {
//                qDebug() << "#SearchDeviceThread# udp socket join group " << UDPMULTICASTADDR << " fail!";
//                sleep(1);
//                continue;
            }
            qDebug() << "#SearchDeviceThread# prepare work finished";
            udpSocket->writeDatagram("<Discovery/>\r\n", QHostAddress(UDPMULTICASTADDR), UDPMULTICASTSENDPORT);
        }

        //等待设备的回应，超时则认为设备搜索完成
        if(udpSocket->waitForReadyRead(1000)) {
            readyRead();
        }else {
            isRun = false;
        }
        msleep(100);
    }
    udpSocket->abort();
    udpSocket->close();
    delete udpSocket;
    qDebug() << "#SearchDeviceThread# serach device finished";
}

void SearchDeviceThread::readyRead()
{
    QHostAddress ip;
    quint16 port;
    int length = udpSocket->pendingDatagramSize();
    if (length <= 0)
        return;

    char *buff = (char *)malloc(length);
    int ret;
    ret = udpSocket->readDatagram(buff, length, &ip, &port);

    buff[ret] = '\0';
    qDebug() << "Sender IP:" << ip.toString() << "Port:" << port;
    qDebug() << "Data Length:" << ret << "Data Content:" << buff;

    WholeDeviceInfo deviceInfo;
    if(ParseXML::getInstance()->parseDiscoveryDevice(&deviceInfo, buff)) {
        emit signalDeviceInfo(QVariant::fromValue(deviceInfo));
    }
}
