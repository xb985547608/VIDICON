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
    int errorCount = 0;

    while(isRun) {
        while(!udpSocket->isValid() ||
              (udpSocket->state() != QUdpSocket::BoundState)) {
            udpSocket->abort();
            udpSocket->close();
            delete udpSocket;
            udpSocket = new QUdpSocket();

            //将套接字绑定在指定接口上(这里通常与之绑定的是无线网卡的地址
            if (!udpSocket->bind(specifiedIP, UDPRECV_PORT, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint)) {
                qDebug() << "#SearchDeviceThread# udp socket bind error!" << specifiedIP << UDPRECV_PORT;
                sleep(1);
                errorCount++;
                if (errorCount >= 3)
                    break;
                continue;
            }

            qDebug() << "#SearchDeviceThread# prepare work finished";
            udpSocket->writeDatagram("<Discovery/>\r\n", QHostAddress(BROADCAST_ADDR), UDPSEND_PORT);
        }

        //等待设备的回应，超时则认为设备搜索完成
        if(udpSocket->isValid() &&
                udpSocket->waitForReadyRead(1000)) {
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
