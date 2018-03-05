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
    udpSocket = new QUdpSocket();

    while(isRun) {
        while(!(udpSocket->isValid()) || udpSocket->state() != QUdpSocket::BoundState) {
            udpSocket->abort();
            udpSocket->close();
            delete udpSocket;
            udpSocket = new QUdpSocket();

            if (!udpSocket->bind(specifiedIP, UDPMULTICASTRECEIVEPORT, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint)) {
                qDebug() << "#SearchDeviceThread# udp socket bind error!";
            }
            udpSocket->setSocketOption(QAbstractSocket::MulticastLoopbackOption, 1);
            if (!udpSocket->joinMulticastGroup(QHostAddress(UDPMULTICASTADDR))) {
                qDebug() << "#SearchDeviceThread# udp socket join group " << " fail!";
            }
            msleep(1000);
        }

        udpSocket->writeDatagram("<Discovery/>\r\n", QHostAddress(UDPMULTICASTADDR), UDPMULTICASTSENDPORT);
        if(udpSocket->waitForReadyRead(1000)) {
            readyRead();
        }else {
            qDebug() << udpSocket->error();
        }
        msleep(1000);
    }
    udpSocket->abort();
    udpSocket->close();
    deleteLater();
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
