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
    qDebug() << "#SearchDeviceThread# thread start!!";

    udpSocket = new QUdpSocket();

    while(isRun) {
        qDebug("tag1");
        while(!(udpSocket->isValid()) || udpSocket->state() != QUdpSocket::BoundState) {
            udpSocket->abort();
            udpSocket->close();
            delete udpSocket;
            udpSocket = new QUdpSocket();

            //将套接字绑定在指定接口上(这里通常与之绑定的是无线网卡的地址)
            if (!udpSocket->bind(specifiedIP, UDPMULTICASTRECEIVEPORT, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint)) {
                qDebug() << "#SearchDeviceThread# udp socket bind error!";
                continue;
            }
            udpSocket->setSocketOption(QAbstractSocket::MulticastLoopbackOption, 1);
            if (!udpSocket->joinMulticastGroup(QHostAddress(UDPMULTICASTADDR))) {
//                qDebug() << "#SearchDeviceThread# udp socket join group " << UDPMULTICASTADDR << " fail!";
            }
            msleep(100);
            qDebug() << "#SearchDeviceThread# prepare work finished";
        }

        udpSocket->writeDatagram("<Discovery/>\r\n", QHostAddress(UDPMULTICASTADDR), UDPMULTICASTSENDPORT);
        if(udpSocket->waitForReadyRead(1000)) {
            readyRead();
        }else {
            qDebug() << udpSocket->error();
        }
        msleep(100);
    }
    udpSocket->abort();
    udpSocket->close();
    qDebug() << "#SearchDeviceThread# thread end!!";
}

void SearchDeviceThread::readyRead()
{
    QHostAddress ip;
    quint16 port;
    int length = udpSocket->pendingDatagramSize();
    if (length <= 0)
        return;

    char buff[length];
    int ret;
    ret = udpSocket->readDatagram(buff, length, &ip, &port);

    buff[ret] = '\0';
    qDebug() << "Sender IP:" << ip.toString() << "Port:" << port;
    qDebug() << "Data Length:" << ret << "Data Content:" << buff;

    DeviceInfo *deviceInfo = new DeviceInfo;
    if(ParseXML::getInstance()->parseDiscoveryDevice(deviceInfo, buff)) {
        isRun = false;
        emit signalDeviceInfo(deviceInfo);
    }
}
