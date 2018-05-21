#include "selectcurrentipdialog.h"
#include "ui_selectcurrentipdialog.h"
#include <QNetworkInterface>
#include <QList>
#include <QDebug>

SelectCurrentIpDialog::SelectCurrentIpDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::SelectCurrentIpDialog)
{
    m_ui->setupUi(this);

    foreach (QNetworkInterface netInterface, QNetworkInterface::allInterfaces())
    {
        //设备名
        qDebug() << "设备名称:" << netInterface.humanReadableName().toUtf8();

        //MAC地址
        qDebug() << "HardwareAddress:" << netInterface.hardwareAddress();

        if (netInterface.hardwareAddress().isEmpty() || netInterface.addressEntries().isEmpty())
            continue;

        foreach (QNetworkAddressEntry entry, netInterface.addressEntries())
        {
            QString item = netInterface.humanReadableName();
            if (entry.ip().protocol() == QAbstractSocket::IPv6Protocol)
                item.append(" IPV6");
            else if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol)
                item.append(" IPV4");
            else
                continue;

            item.append("\t").append(entry.ip().toString());
            m_ui->listWidget->addItem(item);
            m_allInterface.append(entry.ip());
        }
    }
}

SelectCurrentIpDialog::~SelectCurrentIpDialog()
{
    delete m_ui;
}

QHostAddress SelectCurrentIpDialog::getCurrentSelectIp()
{
    return m_allInterface.at(m_ui->listWidget->currentRow());
}
