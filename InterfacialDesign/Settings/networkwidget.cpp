#include "networkwidget.h"
#include <QGridLayout>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QSlider>
#include <QDebug>
#include <QModelIndex>
#include "Protocol/vidiconprotocol.h"
#include "parsexml.h"
#include <QCheckBox>

NetworkWidget::NetworkWidget(QWidget *parent) : QStackedWidget(parent)
{
    initTCPIPWidget();
    initPPPOEWidget();
    initDDNSClientWidget();
    initEmailWidget();
    initFTPWidget();
    initBonjourWidget();
    initSNMPWidget();
    initUPNPWidget();
    initHTTPsWidget();
    initP2PWidget();
    initRTSPWidget();

    connect(VidiconProtocol::getInstance(), &VidiconProtocol::signalReceiveData, this, &NetworkWidget::handleReceiveData);
    connect(this, &NetworkWidget::signalSetParameter, VidiconProtocol::getInstance(), &VidiconProtocol::handleSetParameter);
    connect(this, &NetworkWidget::signalGetParameter, VidiconProtocol::getInstance(), &VidiconProtocol::handleGetParameter);
}

NetworkWidget::~NetworkWidget()
{
    qDebug("delete TabNetwork");
}

void NetworkWidget::initTCPIPWidget()
{
    QStringList list;
    tcpIpWidget = new QWidget(this);

    QLabel *lbl1 = new QLabel("最大连接数：", tcpIpWidget);
    QLineEdit *lineEdit1 = new QLineEdit(tcpIpWidget);
    tcpIpMap.insert("Max connection", lineEdit1);

    QLabel *lbl2 = new QLabel("DHCP启用：", tcpIpWidget);
    QComboBox *comboBox1 = new QComboBox(tcpIpWidget);
    list << "Disable" << "Enable";
    comboBox1->addItems(list);
    list.clear();
    tcpIpMap.insert("DHCP", comboBox1);

    QLabel *lbl3 = new QLabel("IPv4地址：", tcpIpWidget);
    QLineEdit *lineEdit2 = new QLineEdit(tcpIpWidget);
    lineEdit2->setInputMask("000.000.000.000;");
    tcpIpMap.insert("IPv4 Address", lineEdit2);

    QLabel *lbl4 = new QLabel("IPv4掩码地址：", tcpIpWidget);
    QLineEdit *lineEdit3 = new QLineEdit(tcpIpWidget);
    lineEdit3->setInputMask("000.000.000.000;");
    tcpIpMap.insert("IPv4 Subnet", lineEdit3);

    QLabel *lbl5 = new QLabel("IPv4网关地址：", tcpIpWidget);
    QLineEdit *lineEdit4 = new QLineEdit(tcpIpWidget);
    lineEdit4->setInputMask("000.000.000.000;");
    tcpIpMap.insert("IPv4 Gateway", lineEdit4);

    QLabel *lbl6 = new QLabel("IPv4主DNS：", tcpIpWidget);
    QLineEdit *lineEdit5 = new QLineEdit(tcpIpWidget);
    lineEdit5->setInputMask("000.000.000.000;");
    tcpIpMap.insert("IPv4 DNS 1", lineEdit5);

    QLabel *lbl7 = new QLabel("IPv4备用DNS：", tcpIpWidget);
    QLineEdit *lineEdit6 = new QLineEdit(tcpIpWidget);
    lineEdit6->setInputMask("000.000.000.000;");
    tcpIpMap.insert("IPv4 DNS 2", lineEdit6);

    QLabel *lbl8 = new QLabel("MAC地址：", tcpIpWidget);
    QLineEdit *lineEdit7 = new QLineEdit(tcpIpWidget);
    lineEdit7->setInputMask("HH:HH:HH:HH:HH:HH;");
    tcpIpMap.insert("IPv4 MacAddr", lineEdit7);

    QLabel *lbl9 = new QLabel("IPv6地址：", tcpIpWidget);
    QLineEdit *lineEdit8 = new QLineEdit(tcpIpWidget);
    tcpIpMap.insert("IPv6 Address", lineEdit8);

    QLabel *lbl10 = new QLabel("IPv6掩码地址：", tcpIpWidget);
    QLineEdit *lineEdit9 = new QLineEdit(tcpIpWidget);
    tcpIpMap.insert("IPv6 Gateway", lineEdit9);

    QLabel *lbl11 = new QLabel("IPv6主DNS：", tcpIpWidget);
    QLineEdit *lineEdit10 = new QLineEdit(tcpIpWidget);
    tcpIpMap.insert("IPv6 DNS 1", lineEdit10);

    QLabel *lbl12 = new QLabel("IPv6备用DNS：", tcpIpWidget);
    QLineEdit *lineEdit11 = new QLineEdit(tcpIpWidget);
    tcpIpMap.insert("IPv6 DNS 2", lineEdit11);

    //目前ipv6还未完全推广，暂不使用
    lbl9->setVisible(false);
    lbl10->setVisible(false);
    lbl11->setVisible(false);
    lbl12->setVisible(false);

    lineEdit8->setVisible(false);
    lineEdit9->setVisible(false);
    lineEdit10->setVisible(false);
    lineEdit11->setVisible(false);

    QLabel *lbl13 = new QLabel("HTTP端口(1-65535)：", tcpIpWidget);
    QLineEdit *lineEdit12 = new QLineEdit(tcpIpWidget);
    lineEdit12->setValidator(new QIntValidator(1, 65535, tcpIpWidget));
    tcpIpMap.insert("HTTP Port", lineEdit12);

    QLabel *lbl14 = new QLabel("Onvif端口(1-65535)：", tcpIpWidget);
    QLineEdit *lineEdit13 = new QLineEdit(tcpIpWidget);
    lineEdit13->setValidator(new QIntValidator(1, 65535, tcpIpWidget));
    tcpIpMap.insert("Onvif Port", lineEdit13);

    QLabel *lbl15 = new QLabel("RTSP端口(1-65535)：", tcpIpWidget);
    QLineEdit *lineEdit14 = new QLineEdit(tcpIpWidget);
    lineEdit14->setValidator(new QIntValidator(1, 65535, tcpIpWidget));
    tcpIpMap.insert("RTSP Port", lineEdit14);

    QPushButton *btn = new QPushButton("保存", tcpIpWidget);
    connect(btn, &QPushButton::clicked, this, &NetworkWidget::handlePrepareData);

    QGridLayout *layout1 = new QGridLayout;

    layout1->addWidget(lbl1,       0, 0, 1, 1);
    layout1->addWidget(lineEdit1,  0, 1, 1, 2);

    layout1->addWidget(lbl2,       1, 0, 1, 1);
    layout1->addWidget(comboBox1,  1, 1, 1, 2);

    layout1->addWidget(lbl3,       2, 0, 1, 1);
    layout1->addWidget(lineEdit2,  2, 1, 1, 2);

    layout1->addWidget(lbl4,       3, 0, 1, 1);
    layout1->addWidget(lineEdit3,  3, 1, 1, 2);

    layout1->addWidget(lbl5,       4, 0, 1, 1);
    layout1->addWidget(lineEdit4,  4, 1, 1, 2);

    layout1->addWidget(lbl6,       5, 0, 1, 1);
    layout1->addWidget(lineEdit5,  5, 1, 1, 2);

    layout1->addWidget(lbl7,       6, 0, 1, 1);
    layout1->addWidget(lineEdit6,  6, 1, 1, 2);

    layout1->addWidget(lbl8,       7, 0, 1, 1);
    layout1->addWidget(lineEdit7,  7, 1, 1, 2);

    layout1->addWidget(lbl9,       8, 0, 1, 1);
    layout1->addWidget(lineEdit8,  8, 1, 1, 2);

    layout1->addWidget(lbl10,      9, 0, 1, 1);
    layout1->addWidget(lineEdit9,  9, 1, 1, 2);

    layout1->addWidget(lbl11,      10, 0, 1, 1);
    layout1->addWidget(lineEdit10, 10, 1, 1, 2);

    layout1->addWidget(lbl12,      11, 0, 1, 1);
    layout1->addWidget(lineEdit11, 11, 1, 1, 2);

    layout1->addWidget(lbl13,      12, 0, 1, 1);
    layout1->addWidget(lineEdit12, 12, 1, 1, 2);

    layout1->addWidget(lbl14,      13, 0, 1, 1);
    layout1->addWidget(lineEdit13, 13, 1, 1, 2);

    layout1->addWidget(lbl15,      14, 0, 1, 1);
    layout1->addWidget(lineEdit14, 14, 1, 1, 2);

    layout1->addWidget(btn, 15, 0, 1, 3, Qt::AlignCenter);

    QVBoxLayout *layout2 = new QVBoxLayout;
    layout2->addLayout(layout1);
    layout2->addStretch();

    QHBoxLayout *layout3 = new QHBoxLayout(tcpIpWidget);
    layout3->addStretch(1);
    layout3->addLayout(layout2, 10);
    layout3->addStretch(1);

    addWidget(tcpIpWidget);
}

void NetworkWidget::initPPPOEWidget()
{
    PPPOEWidget = new QWidget(this);

    QRadioButton *rBtn = new QRadioButton("启用PPPOE", PPPOEWidget);
    PPPOEMap.insert("Enable", rBtn);

    QLabel *lbl1 = new QLabel("用户名：", PPPOEWidget);
    QLineEdit *lineEdit1 = new QLineEdit(PPPOEWidget);
    PPPOEMap.insert("Username", lineEdit1);

    QLabel *lbl2 = new QLabel("密码：", PPPOEWidget);
    QLineEdit *lineEdit2 = new QLineEdit(PPPOEWidget);
    PPPOEMap.insert("Password", lineEdit1);

    QPushButton *btn = new QPushButton("保存", PPPOEWidget);
    btn->setFixedWidth(50);

    QGridLayout *layout1 = new QGridLayout;
    layout1->addWidget(rBtn,      0, 0, 1, 1);

    layout1->addWidget(lbl1,      1, 0, 1, 1);
    layout1->addWidget(lineEdit1, 1, 1, 1, 2);

    layout1->addWidget(lbl2,      2, 0, 1, 1);
    layout1->addWidget(lineEdit2, 2, 1, 1, 2);

    layout1->addWidget(btn,       3, 0, 1, 3, Qt::AlignCenter);

    QVBoxLayout *layout2 = new QVBoxLayout;
    layout2->addLayout(layout1);
    layout2->addStretch();

    QHBoxLayout *layout3 = new QHBoxLayout(PPPOEWidget);
    layout3->addStretch(1);
    layout3->addLayout(layout2, 10);
    layout3->addStretch(1);

    addWidget(PPPOEWidget);
}

void NetworkWidget::initDDNSClientWidget()
{
    QStringList list;
    DDNSClientWidget = new QWidget(this);

    QRadioButton *rBtn = new QRadioButton("启用DDNS", DDNSClientWidget);
    DDNSClientMap.insert("Enable", rBtn);

    QLabel *lbl1 = new QLabel("协议类型：", DDNSClientWidget);
    QComboBox *comboBox = new QComboBox();
    list << "3322.org" << "DynDDNS" << "Noip";
    comboBox->addItems(list);
    list.clear();
    DDNSClientMap.insert("Provider", comboBox);

    QLabel *lbl2 = new QLabel("DDNS服务器：", DDNSClientWidget);
    QLineEdit *lineEdit1 = new QLineEdit(DDNSClientWidget);
    DDNSClientMap.insert("Server", lineEdit1);

    QLabel *lbl3 = new QLabel("DDNS域名：", DDNSClientWidget);
    QLineEdit *lineEdit2 = new QLineEdit(DDNSClientWidget);
    DDNSClientMap.insert("Hostname", lineEdit2);

    QLabel *lbl4 = new QLabel("用户名：", DDNSClientWidget);
    QLineEdit *lineEdit3 = new QLineEdit(DDNSClientWidget);
    DDNSClientMap.insert("Username", lineEdit3);

    QLabel *lbl5 = new QLabel("密码：", DDNSClientWidget);
    QLineEdit *lineEdit4 = new QLineEdit(DDNSClientWidget);
    DDNSClientMap.insert("Password", lineEdit4);

    QPushButton *btn = new QPushButton("保存", PPPOEWidget);
    btn->setFixedWidth(50);

    QGridLayout *layout1 = new QGridLayout;
    layout1->addWidget(rBtn,      0, 0, 1, 1);

    layout1->addWidget(lbl1,      1, 0, 1, 1);
    layout1->addWidget(comboBox,  1, 1, 1, 2);

    layout1->addWidget(lbl2,      2, 0, 1, 1);
    layout1->addWidget(lineEdit1, 2, 1, 1, 2);

    layout1->addWidget(lbl3,      3, 0, 1, 1);
    layout1->addWidget(lineEdit2, 3, 1, 1, 2);

    layout1->addWidget(lbl4,      4, 0, 1, 1);
    layout1->addWidget(lineEdit3, 4, 1, 1, 2);

    layout1->addWidget(lbl5,      5, 0, 1, 1);
    layout1->addWidget(lineEdit4, 5, 1, 1, 2);

    layout1->addWidget(btn,       6, 0, 1, 3, Qt::AlignCenter);

    QVBoxLayout *layout2 = new QVBoxLayout;
    layout2->addLayout(layout1);
    layout2->addStretch();

    QHBoxLayout *layout3 = new QHBoxLayout(DDNSClientWidget);
    layout3->addStretch(1);
    layout3->addLayout(layout2, 10);
    layout3->addStretch(1);

    addWidget(DDNSClientWidget);
}

void NetworkWidget::initEmailWidget()
{
    EmailWidget = new QWidget(this);

    QRadioButton *rBtn = new QRadioButton("启用邮箱", EmailWidget);
    EmailMap.insert("Enable", rBtn);

    QLabel *lbl1 = new QLabel("移动侦测标题：", EmailWidget);
    QLineEdit *lineEdit1 = new QLineEdit(EmailWidget);
    EmailMap.insert("Motion Subject", lineEdit1);

    QLabel *lbl2 = new QLabel("探头报警标题：", EmailWidget);
    QLineEdit *lineEdit2 = new QLineEdit(EmailWidget);
    EmailMap.insert("Alarm Subject", lineEdit2);

    QLabel *lbl3 = new QLabel("SMTP服务器：", EmailWidget);
    QLineEdit *lineEdit3 = new QLineEdit(EmailWidget);
    EmailMap.insert("SMTP Server", lineEdit3);

    QLabel *lbl4 = new QLabel("SMTP端口：", EmailWidget);
    QLineEdit *lineEdit4 = new QLineEdit(EmailWidget);
    EmailMap.insert("SMTP Port", lineEdit4);

    QLabel *lbl5 = new QLabel("发送人E-mail：", EmailWidget);
    QLineEdit *lineEdit5 = new QLineEdit(EmailWidget);
    EmailMap.insert("Sender Address", lineEdit5);

    QLabel *lbl6 = new QLabel("密码：", EmailWidget);
    QLineEdit *lineEdit6 = new QLineEdit(EmailWidget);
    EmailMap.insert("Sender Password", lineEdit6);

    QLabel *lbl7 = new QLabel("收件人E-mail：", EmailWidget);
    QLineEdit *lineEdit7 = new QLineEdit(EmailWidget);
    EmailMap.insert("Recipient Address-1", lineEdit7);

    QLabel *lbl8 = new QLabel("收件人E-mail：", EmailWidget);
    QLineEdit *lineEdit8 = new QLineEdit(EmailWidget);
    EmailMap.insert("Recipient Address-2", lineEdit8);

    QLabel *lbl9 = new QLabel("收件人E-mail：", EmailWidget);
    QLineEdit *lineEdit9 = new QLineEdit(EmailWidget);
    EmailMap.insert("Recipient Address-3", lineEdit9);

    QLabel *lbl10 = new QLabel("收件人E-mail：", EmailWidget);
    QLineEdit *lineEdit10 = new QLineEdit(EmailWidget);
    EmailMap.insert("Recipient Address-4", lineEdit10);

    QPushButton *btn1 = new QPushButton("保存", EmailWidget);
    btn1->setFixedWidth(50);

    QPushButton *btn2 = new QPushButton("测试", EmailWidget);
    btn2->setFixedWidth(50);

    QGridLayout *layout1 = new QGridLayout;
    layout1->addWidget(rBtn,       0, 0, 1, 1);

    layout1->addWidget(lbl1,       1, 0, 1, 1);
    layout1->addWidget(lineEdit1,  1, 1, 1, 2);

    layout1->addWidget(lbl2,       2, 0, 1, 1);
    layout1->addWidget(lineEdit2,  2, 1, 1, 2);

    layout1->addWidget(lbl3,       3, 0, 1, 1);
    layout1->addWidget(lineEdit3,  3, 1, 1, 2);

    layout1->addWidget(lbl4,       4, 0, 1, 1);
    layout1->addWidget(lineEdit4,  4, 1, 1, 2);

    layout1->addWidget(lbl5,       5, 0, 1, 1);
    layout1->addWidget(lineEdit5,  5, 1, 1, 2);

    layout1->addWidget(lbl6,       6, 0, 1, 1);
    layout1->addWidget(lineEdit6,  6, 1, 1, 2);

    layout1->addWidget(lbl7,       7, 0, 1, 1);
    layout1->addWidget(lineEdit7,  7, 1, 1, 2);

    layout1->addWidget(lbl8,       8, 0, 1, 1);
    layout1->addWidget(lineEdit8,  8, 1, 1, 2);

    layout1->addWidget(lbl9,       9, 0, 1, 1);
    layout1->addWidget(lineEdit9,  9, 1, 1, 2);

    layout1->addWidget(lbl10,      10, 0, 1, 1);
    layout1->addWidget(lineEdit10, 10, 1, 1, 2);

    layout1->addWidget(btn1,       11, 0, 1, 1, Qt::AlignRight);
    layout1->addWidget(btn2,       11, 2, 1, 1, Qt::AlignLeft);

    QVBoxLayout *layout2 = new QVBoxLayout;
    layout2->addLayout(layout1);
    layout2->addStretch();

    QHBoxLayout *layout3 = new QHBoxLayout(EmailWidget);
    layout3->addStretch(1);
    layout3->addLayout(layout2, 10);
    layout3->addStretch(1);

    addWidget(EmailWidget);
}

void NetworkWidget::initFTPWidget()
{
    QStringList list;
    FTPWidget = new QWidget(this);

    QRadioButton *rBtn = new QRadioButton("启用FTP服务器", FTPWidget);
    FTPMap.insert("Enable", rBtn);

    QLabel *lbl1 = new QLabel("FTP服务器", FTPWidget);
    QLineEdit *lineEdit1 = new QLineEdit(FTPWidget);
    FTPMap.insert("Server", lineEdit1);

    QLabel *lbl2 = new QLabel("FTP端口：", FTPWidget);
    QLineEdit *lineEdit2 = new QLineEdit(FTPWidget);
    FTPMap.insert("Port", lineEdit2);

    QLabel *lbl3 = new QLabel("模式：", FTPWidget);
    QComboBox *comboBox = new QComboBox();
    list << "被动" << "主动";
    comboBox->addItems(list);
    list.clear();
    FTPMap.insert("Mode", comboBox);

    QLabel *lbl4 = new QLabel("用户名：", FTPWidget);
    QLineEdit *lineEdit3 = new QLineEdit(FTPWidget);
    FTPMap.insert("Username", lineEdit3);

    QLabel *lbl5 = new QLabel("密码：", FTPWidget);
    QLineEdit *lineEdit4 = new QLineEdit(FTPWidget);
    FTPMap.insert("Password", lineEdit4);

    QLabel *lbl6 = new QLabel("上传路径：", FTPWidget);
    QLineEdit *lineEdit5 = new QLineEdit(FTPWidget);
    FTPMap.insert("Upload Directory", lineEdit5);

    QPushButton *btn1 = new QPushButton("保存", EmailWidget);
    btn1->setFixedWidth(50);

    QPushButton *btn2 = new QPushButton("测试", EmailWidget);
    btn2->setFixedWidth(50);

    QGridLayout *layout1 = new QGridLayout;
    layout1->addWidget(rBtn,      0, 0, 1, 1);

    layout1->addWidget(lbl1,      1, 0, 1, 1);
    layout1->addWidget(lineEdit1, 1, 1, 1, 2);

    layout1->addWidget(lbl2,      2, 0, 1, 1);
    layout1->addWidget(lineEdit2, 2, 1, 1, 2);

    layout1->addWidget(lbl3,      3, 0, 1, 1);
    layout1->addWidget(comboBox,  3, 1, 1, 2);

    layout1->addWidget(lbl4,      4, 0, 1, 1);
    layout1->addWidget(lineEdit3, 4, 1, 1, 2);

    layout1->addWidget(lbl5,      5, 0, 1, 1);
    layout1->addWidget(lineEdit4, 5, 1, 1, 2);

    layout1->addWidget(lbl6,      6, 0, 1, 1);
    layout1->addWidget(lineEdit5, 6, 1, 1, 2);

    layout1->addWidget(btn1,      7, 0, 1, 1, Qt::AlignRight);
    layout1->addWidget(btn2,      7, 2, 1, 1, Qt::AlignLeft);

    QVBoxLayout *layout2 = new QVBoxLayout;
    layout2->addLayout(layout1);
    layout2->addStretch();

    QHBoxLayout *layout3 = new QHBoxLayout(FTPWidget);
    layout3->addStretch(1);
    layout3->addLayout(layout2, 10);
    layout3->addStretch(1);

    addWidget(FTPWidget);
}

void NetworkWidget::initBonjourWidget()
{
    BonjourWidget = new QWidget(this);

    QRadioButton *rBtn = new QRadioButton("启用Bonjour", BonjourWidget);
    BonjourMap.insert("Enable", rBtn);

    QLabel *lbl1 = new QLabel("Bonjour：", BonjourWidget);
    QLineEdit *lineEdit1 = new QLineEdit(BonjourWidget);
    BonjourMap.insert("Bonjour", lineEdit1);

    QPushButton *btn1 = new QPushButton("恢复默认值", EmailWidget);
    btn1->setFixedWidth(100);

    QPushButton *btn2 = new QPushButton("更新", EmailWidget);
    btn2->setFixedWidth(50);

    QPushButton *btn3 = new QPushButton("保存", EmailWidget);
    btn3->setFixedWidth(50);

    QGridLayout *layout1 = new QGridLayout;
    layout1->addWidget(rBtn,      0, 0, 1, 1);

    layout1->addWidget(lbl1,      1, 0, 1, 1);
    layout1->addWidget(lineEdit1, 1, 1, 1, 2);

    layout1->addWidget(btn1,      2, 1, 1, 1, Qt::AlignLeft);
    layout1->addWidget(btn2,      2, 2, 1, 1, Qt::AlignRight);

    layout1->addWidget(btn3,      3, 1, 1, 1, Qt::AlignCenter);

    QVBoxLayout *layout2 = new QVBoxLayout;
    layout2->addLayout(layout1);
    layout2->addStretch();

    QHBoxLayout *layout3 = new QHBoxLayout(BonjourWidget);
    layout3->addStretch(1);
    layout3->addLayout(layout2, 10);
    layout3->addStretch(1);

    addWidget(BonjourWidget);
}

void NetworkWidget::initSNMPWidget()
{
    SNMPWidget = new QWidget(this);

    QCheckBox *cb1 = new QCheckBox("SNMP v1", SNMPWidget);
    SNMPMap.insert("SNMP v1", cb1);

    QCheckBox *cb2 = new QCheckBox("SNMP v2", SNMPWidget);
    SNMPMap.insert("SNMP v2", cb2);

    QLabel *lbl1 = new QLabel("SNMP端口(1-65535)：", SNMPWidget);
    QLineEdit *lineEdit1 = new QLineEdit(SNMPWidget);
    SNMPMap.insert("SNMP Port", lineEdit1);

    QLabel *lbl2 = new QLabel("Read Community：", SNMPWidget);
    QLineEdit *lineEdit2 = new QLineEdit(SNMPWidget);
    SNMPMap.insert("Read Community", lineEdit2);

    QLabel *lbl3 = new QLabel("Write Community：", SNMPWidget);
    QLineEdit *lineEdit3 = new QLineEdit(SNMPWidget);
    SNMPMap.insert("Write Community", lineEdit3);

    QLabel *lbl4 = new QLabel("Trap地址：", SNMPWidget);
    QLineEdit *lineEdit4 = new QLineEdit(SNMPWidget);
    SNMPMap.insert("Trap Address", lineEdit4);

    QLabel *lbl5 = new QLabel("Trap端口(1-65535)：", SNMPWidget);
    QLineEdit *lineEdit5 = new QLineEdit(SNMPWidget);
    SNMPMap.insert("Trap Port", lineEdit5);

    QPushButton *btn = new QPushButton("保存", EmailWidget);
    btn->setFixedWidth(50);

    QGridLayout *layout1 = new QGridLayout;
    layout1->addWidget(cb1,     0, 0, 1, 1);
    layout1->addWidget(cb2,     1, 0, 1, 1);

    layout1->addWidget(lbl1,      2, 0, 1, 1);
    layout1->addWidget(lineEdit1, 2, 1, 1, 2);

    layout1->addWidget(lbl2,      3, 0, 1, 1);
    layout1->addWidget(lineEdit2, 3, 1, 1, 2);

    layout1->addWidget(lbl3,      4, 0, 1, 1);
    layout1->addWidget(lineEdit3, 4, 1, 1, 2);

    layout1->addWidget(lbl4,      5, 0, 1, 1);
    layout1->addWidget(lineEdit4, 5, 1, 1, 2);

    layout1->addWidget(lbl5,      6, 0, 1, 1);
    layout1->addWidget(lineEdit5, 6, 1, 1, 2);

    layout1->addWidget(btn,       7, 1, 1, 1, Qt::AlignCenter);

    QVBoxLayout *layout2 = new QVBoxLayout;
    layout2->addLayout(layout1);
    layout2->addStretch();

    QHBoxLayout *layout3 = new QHBoxLayout(SNMPWidget);
    layout3->addStretch(1);
    layout3->addLayout(layout2, 10);
    layout3->addStretch(1);

    addWidget(SNMPWidget);
}

void NetworkWidget::initUPNPWidget()
{
    QStringList list;
    UPNPWidget = new QWidget(this);

    QLabel *lbl = new QLabel("是否启用UPNP：", UPNPWidget);
    QComboBox *comboBox = new QComboBox();
    list << "关闭" << "打开";
    comboBox->addItems(list);
    list.clear();
    UPNPMap.insert("UPNP", comboBox);

    QPushButton *btn = new QPushButton("保存", UPNPWidget);
    btn->setFixedWidth(50);

    QGridLayout *layout1 = new QGridLayout;
    layout1->addWidget(lbl,      0, 0, 1, 1);
    layout1->addWidget(comboBox, 0, 1, 1, 2);

    layout1->addWidget(btn,      1, 1, 1, 1, Qt::AlignCenter);

    QVBoxLayout *layout2 = new QVBoxLayout;
    layout2->addLayout(layout1);
    layout2->addStretch();

    QHBoxLayout *layout3 = new QHBoxLayout(UPNPWidget);
    layout3->addStretch(1);
    layout3->addLayout(layout2, 10);
    layout3->addStretch(1);

    addWidget(UPNPWidget);
}

void NetworkWidget::initHTTPsWidget()
{
    HTTPsWidget = new QWidget(this);

    QRadioButton *rBtn = new QRadioButton("启用HTTPs", HTTPsWidget);
    HTTPsMap.insert("Enable", rBtn);

    QLabel *lbl = new QLabel("HTTPs端口(1-65535)：", HTTPsWidget);
    QLineEdit *lineEdit = new QLineEdit(HTTPsWidget);
    HTTPsMap.insert("HTTPs Port", lineEdit);

    QPushButton *btn = new QPushButton("保存", HTTPsWidget);
    btn->setFixedWidth(50);

    QGridLayout *layout1 = new QGridLayout;
    layout1->addWidget(rBtn,     0, 0, 1, 1);
    layout1->addWidget(lbl,      1, 0, 1, 1);
    layout1->addWidget(lineEdit, 1, 1, 1, 2);

    layout1->addWidget(btn,      2, 1, 1, 1, Qt::AlignCenter);

    QVBoxLayout *layout2 = new QVBoxLayout;
    layout2->addLayout(layout1);
    layout2->addStretch();

    QHBoxLayout *layout3 = new QHBoxLayout(HTTPsWidget);
    layout3->addStretch(1);
    layout3->addLayout(layout2, 10);
    layout3->addStretch(1);

    addWidget(HTTPsWidget);
}

void NetworkWidget::initP2PWidget()
{
    P2PWidget = new QWidget(this);

    QRadioButton *rBtn = new QRadioButton("启用P2P", P2PWidget);
    P2PMap.insert("P2P", rBtn);

    QLabel *lbl1 = new QLabel("UUID：", P2PWidget);
    QLineEdit *lineEdit1 = new QLineEdit(P2PWidget);
    lineEdit1->setFixedWidth(400);
    P2PMap.insert("UUID", lineEdit1);

    QLabel *lbl2 = new QLabel("安卓应用：", P2PWidget);
    QLineEdit *lineEdit2 = new QLineEdit("http://www.zwcloud.wang/p2p_direct.html", P2PWidget);

    QLabel *lbl3 = new QLabel("IOS应用：", P2PWidget);
    QLineEdit *lineEdit3 = new QLineEdit("https://itunes.apple.com/us/app/p2pcmscam/id1173582301?mt=8", P2PWidget);

    QPushButton *btn = new QPushButton("Save", P2PWidget);
    btn->setFixedWidth(50);

    QGridLayout *layout1 = new QGridLayout;
    layout1->addWidget(rBtn,      0, 0, 1, 2);

    layout1->addWidget(lbl1,      1, 0, 1, 2);
    layout1->addWidget(lineEdit1, 1, 2, 1, 9);

    layout1->addWidget(lbl2,      2, 0, 1, 2);
    layout1->addWidget(lineEdit2, 2, 2, 1, 9);

    layout1->addWidget(lbl3,      3, 0, 1, 2);
    layout1->addWidget(lineEdit3, 3, 2, 1, 9);

    layout1->addWidget(btn,       4, 0, 1, 11, Qt::AlignCenter);

    QVBoxLayout *layout2 = new QVBoxLayout;
    layout2->addLayout(layout1);
    layout2->addStretch();

    QHBoxLayout *layout3 = new QHBoxLayout(P2PWidget);
    layout3->addStretch(1);
    layout3->addLayout(layout2, 10);
    layout3->addStretch(1);

    addWidget(P2PWidget);
}

void NetworkWidget::initRTSPWidget()
{
    RTSPWidget = new QWidget(this);

    QRadioButton *rBtn = new QRadioButton("启用RTSP", RTSPWidget);
    RTSPMap.insert("Enable", rBtn);

    QLabel *lbl1 = new QLabel("RTSP端口：", RTSPWidget);
    QLineEdit *lineEdit1 = new QLineEdit(RTSPWidget);
    RTSPMap.insert("RTSP Port", lineEdit1);

    QPushButton *btn = new QPushButton("保存", P2PWidget);
    btn->setFixedWidth(50);

    QLabel *lbl2 = new QLabel("URL例子：", RTSPWidget);
    QLabel *lbl3 = new QLabel("rtsp://IP:PORT/H264?channel=1&subtype=A&unicast=true&proto=Onvif/video\n"
                              "A:0(Main Steam) 1(Sub Stream)", RTSPWidget);
    lbl3->setFixedWidth(450);

    QGridLayout *layout1 = new QGridLayout;
    layout1->addWidget(rBtn,      0, 0, 1, 1);

    layout1->addWidget(lbl1,      1, 0, 1, 1);
    layout1->addWidget(lineEdit1, 1, 1, 1, 2);

    layout1->addWidget(btn,       2, 0, 1, 3, Qt::AlignCenter);

    layout1->addWidget(lbl2,      3, 0, 1, 1, Qt::AlignTop);
    layout1->addWidget(lbl3,      3, 1, 1, 2, Qt::AlignTop);

    QVBoxLayout *layout2 = new QVBoxLayout;
    layout2->addLayout(layout1);
    layout2->addStretch();

    QHBoxLayout *layout3 = new QHBoxLayout(RTSPWidget);
    layout3->addStretch(1);
    layout3->addLayout(layout2, 10);
    layout3->addStretch(1);

    addWidget(RTSPWidget);
}

void NetworkWidget::handleSwitchTab(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    switch(index.row()){
    case 0: {
        emit signalGetParameter(TCPIP);
        emit signalGetParameter(OTHER);
        break;
    }
    case 1: {
        emit signalGetParameter(PPPOE);
        break;
    }
    case 2: {
        emit signalGetParameter(DDNS);
        break;
    }
    case 3: {
        emit signalGetParameter(EMAIL);
        break;
    }
    case 4: {
        emit signalGetParameter(FTP);
        break;
    }
    case 5: {
        emit signalGetParameter(BONJOUR);
        break;
    }
    case 6: {
        emit signalGetParameter(SNMP);
        break;
    }
    case 7: {
        emit signalGetParameter(UPNP);
        break;
    }
    case 8: {
        emit signalGetParameter(HTTPS);
        break;
    }
    case 9: {
        emit signalGetParameter(P2P);
        break;
    }
    case 10: {
        emit signalGetParameter(RTSP);
        break;
    }
    default:
        break;
    }

    setCurrentIndex(index.row());
}

void NetworkWidget::handlePrepareData()
{
    switch(currentIndex()) {
    case 0: {
        VidiconProtocol::BasicParameter *param1 = new VidiconProtocol::BasicParameter;
        param1->MaxLink = static_cast<QLineEdit *>(tcpIpMap["Max connection"])->text().toInt();
        param1->DHCP = static_cast<QComboBox *>(tcpIpMap["DHCP"])->currentIndex();
        param1->ipv4.IpAddr = static_cast<QLineEdit *>(tcpIpMap["IPv4 Address"])->text();
        param1->ipv4.SubnetMask = static_cast<QLineEdit *>(tcpIpMap["IPv4 Subnet"])->text();
        param1->ipv4.Gateway = static_cast<QLineEdit *>(tcpIpMap["IPv4 Gateway"])->text();
        param1->ipv4.DNS1 = static_cast<QLineEdit *>(tcpIpMap["IPv4 DNS 1"])->text();
        param1->ipv4.DNS2 = static_cast<QLineEdit *>(tcpIpMap["IPv4 DNS 2"])->text();
        param1->ipv6.IpAddr = static_cast<QLineEdit *>(tcpIpMap["IPv6 Address"])->text();
        param1->ipv6.Gateway = static_cast<QLineEdit *>(tcpIpMap["IPv6 Gateway"])->text();
        param1->ipv6.DNS1 = static_cast<QLineEdit *>(tcpIpMap["IPv6 DNS 1"])->text();
        param1->ipv6.DNS2 = static_cast<QLineEdit *>(tcpIpMap["IPv6 DNS 2"])->text();
        param1->MACAddress = static_cast<QLineEdit *>(tcpIpMap["IPv4 MacAddr"])->text();
        emit signalSetParameter(TCPIP, param1);

        VidiconProtocol::OtherParameter *param2 = new VidiconProtocol::OtherParameter;
        param2->type1.Enabled = 1;
        param2->type1.ServerType = 1;
        param2->type1.Port = static_cast<QLineEdit *>(tcpIpMap["HTTP Port"])->text().toInt();
        param2->type2.Enabled = 1;
        param2->type2.ServerType = 2;
        param2->type2.Port = static_cast<QLineEdit *>(tcpIpMap["Onvif Port"])->text().toInt();
        param2->type3.Enabled = 1;
        param2->type3.ServerType = 3;
        param2->type3.Port = static_cast<QLineEdit *>(tcpIpMap["RTSP Port"])->text().toInt();
        emit signalSetParameter(OTHER, param2);

        qDebug() << "#TabMedia# handleSendData send signal, ParameterType:" << TCPIP << OTHER;
        break;
    }
    default:
        break;
    }
}

void NetworkWidget::handleReceiveData(int type, QByteArray data)
{
    bool isOK = false;

    switch (type) {
    case TCPIP: {
        VidiconProtocol::BasicParameter param;

        isOK = ParseXML::getInstance()->parseBasicParameter(&param, data);
        if (isOK) {
            static_cast<QLineEdit *>(tcpIpMap["Max connection"])->setText(QString::number(param.MaxLink));
            static_cast<QComboBox *>(tcpIpMap["DHCP"])->setCurrentIndex(param.DHCP);
            static_cast<QLineEdit *>(tcpIpMap["IPv4 Address"])->setText(param.ipv4.IpAddr);
            static_cast<QLineEdit *>(tcpIpMap["IPv4 Subnet"])->setText(param.ipv4.SubnetMask);
            static_cast<QLineEdit *>(tcpIpMap["IPv4 Gateway"])->setText(param.ipv4.Gateway);
            static_cast<QLineEdit *>(tcpIpMap["IPv4 DNS 1"])->setText(param.ipv4.DNS1);
            static_cast<QLineEdit *>(tcpIpMap["IPv4 DNS 2"])->setText(param.ipv4.DNS2);
            static_cast<QLineEdit *>(tcpIpMap["IPv6 Address"])->setText(param.ipv6.IpAddr);
            static_cast<QLineEdit *>(tcpIpMap["IPv6 Gateway"])->setText(param.ipv6.Gateway);
            static_cast<QLineEdit *>(tcpIpMap["IPv6 DNS 1"])->setText(param.ipv6.DNS1);
            static_cast<QLineEdit *>(tcpIpMap["IPv6 DNS 2"])->setText(param.ipv6.DNS2);
            static_cast<QLineEdit *>(tcpIpMap["IPv4 MacAddr"])->setText(param.MACAddress);
        }
        break;
    }
    case OTHER: {
        VidiconProtocol::OtherParameter param;
        VidiconProtocol::OtherBasicParameter *param1 = (VidiconProtocol::OtherBasicParameter *)(&param);

        isOK = ParseXML::getInstance()->parseOtherParameter(&param, data);
        if (isOK) {
            for(int i=1; i<=3; i++) {
                if(param1->ServerType == 1) {
                    static_cast<QLineEdit *>(tcpIpMap["HTTP Port"])->setText(QString::number(param1->Port));
                }else if(param1->ServerType == 2) {
                    static_cast<QLineEdit *>(tcpIpMap["Onvif Port"])->setText(QString::number(param1->Port));
                }else if(param1->ServerType == 3) {
                    static_cast<QLineEdit *>(tcpIpMap["RTSP Port"])->setText(QString::number(param1->Port));
                    static_cast<QRadioButton *>(RTSPMap["Enable"])->setChecked(param1->Enabled ? true : false);
                    static_cast<QLineEdit *>(RTSPMap["RTSP Port"])->setText(QString::number(param1->Port));
                }
                param1++;
            }
        }
        break;
    }
    case PPPOE: {
        VidiconProtocol::PPPOEParameter param;

        isOK = ParseXML::getInstance()->parsePPPOEParameter(&param, data);
        if (isOK) {
            static_cast<QRadioButton *>(PPPOEMap["Enable"])->setChecked(param.Enabled ? true : false);
            static_cast<QLineEdit *>(PPPOEMap["Username"])->setText(param.PPPOEName);
            static_cast<QLineEdit *>(PPPOEMap["Password"])->setText(param.PPPOEPassword);
        }
        break;
    }
    case DDNS: {
        VidiconProtocol::DDNSParameter param;

        isOK = ParseXML::getInstance()->parseDDNSParameter(&param, data);
        if (isOK) {
            static_cast<QRadioButton *>(DDNSClientMap["Enable"])->setChecked(param.Enabled ? true : false);
            static_cast<QComboBox *>(DDNSClientMap["Provider"])->setCurrentText(param.DDNSType);
            static_cast<QLineEdit *>(DDNSClientMap["Server"])->setText(param.DDNSServerName);
            static_cast<QLineEdit *>(DDNSClientMap["Hostname"])->setText(param.DDNSName);
            static_cast<QLineEdit *>(DDNSClientMap["Username"])->setText(param.DDNSUser);
            static_cast<QLineEdit *>(DDNSClientMap["Password"])->setText(param.DDNSPassword);
        }
        break;
    }
    case EMAIL: {
        VidiconProtocol::EmailParameter param;

        isOK = ParseXML::getInstance()->parseEmailParameter(&param, data);
        if (isOK) {
            static_cast<QRadioButton *>(EmailMap["Enable"])->setChecked(param.Enabled ? true : false);
            static_cast<QLineEdit *>(EmailMap["Motion Subject"])->setText(param.MotionAlarmTitle);
            static_cast<QLineEdit *>(EmailMap["Alarm Subject"])->setText(param.SensorAlarmTitle);
            static_cast<QLineEdit *>(EmailMap["SMTP Server"])->setText(param.SmtpServer);
            static_cast<QLineEdit *>(EmailMap["SMTP Port"])->setText(QString::number(param.SmtpPort));
            static_cast<QLineEdit *>(EmailMap["Sender Address"])->setText(param.SmtpServer);
            static_cast<QLineEdit *>(EmailMap["Sender Password"])->setText(param.SmtpPassword);
            static_cast<QLineEdit *>(EmailMap["Recipient Address-1"])->setText(param.Receiver_1);
            static_cast<QLineEdit *>(EmailMap["Recipient Address-2"])->setText(param.Receiver_2);
            static_cast<QLineEdit *>(EmailMap["Recipient Address-3"])->setText(param.Receiver_3);
            static_cast<QLineEdit *>(EmailMap["Recipient Address-4"])->setText(param.Receiver_4);
        }
        break;
    }
    case FTP: {
        VidiconProtocol::FTPParameter param;

        isOK = ParseXML::getInstance()->parseFTPParameter(&param, data);
        if (isOK) {
            static_cast<QRadioButton *>(FTPMap["Enable"])->setChecked(param.Enabled ? true : false);
            static_cast<QLineEdit *>(FTPMap["Server"])->setText(param.FTPServer);
            static_cast<QLineEdit *>(FTPMap["Port"])->setText(QString::number(param.FTPPort));
            static_cast<QComboBox *>(FTPMap["Mode"])->setCurrentIndex(param.FTPMode);
            static_cast<QLineEdit *>(FTPMap["Username"])->setText(param.FTPUser);
            static_cast<QLineEdit *>(FTPMap["Password"])->setText(param.FTPPassword);
            static_cast<QLineEdit *>(FTPMap["Upload Directory"])->setText(param.UploadDirectory);
        }
        break;
    }
    case BONJOUR: {
        VidiconProtocol::BonjourParameter param;

        isOK = ParseXML::getInstance()->parseBonjourParameter(&param, data);
        if (isOK) {
            static_cast<QRadioButton *>(BonjourMap["Enable"])->setChecked(param.Enabled ? true : false);
            static_cast<QLineEdit *>(BonjourMap["Bonjour"])->setText(param.Name);
        }
        break;
    }
    case SNMP: {
        VidiconProtocol::SNMPParameter param;

        isOK = ParseXML::getInstance()->parseSNMPParameter(&param, data);
        if (isOK) {
            static_cast<QCheckBox *>(SNMPMap["SNMP v1"])->setChecked(param.EnabledVer1 ? true : false);
            static_cast<QCheckBox *>(SNMPMap["SNMP v2"])->setChecked(param.EnabledVer2 ? true : false);
            static_cast<QLineEdit *>(SNMPMap["SNMP Port"])->setText(QString::number(param.SnmpPort));
            static_cast<QLineEdit *>(SNMPMap["Read Community"])->setText(param.ReadPublic);
            static_cast<QLineEdit *>(SNMPMap["Write Community"])->setText(param.WritePublic);
            static_cast<QLineEdit *>(SNMPMap["Trap Address"])->setText(param.TrapAddress);
            static_cast<QLineEdit *>(SNMPMap["Trap Port"])->setText(QString::number(param.TrapPort));
        }
        break;
    }
    case UPNP: {
        VidiconProtocol::UPNPParameter param;

        isOK = ParseXML::getInstance()->parseUPNPParameter(&param, data);
        if (isOK) {
            static_cast<QComboBox *>(UPNPMap["UPNP"])->setCurrentIndex(param.Enabled);
        }
        break;
    }
    case HTTPS: {
        VidiconProtocol::HTTPsParameter param;

        isOK = ParseXML::getInstance()->parseHTTPsParameter(&param, data);
        if (isOK) {
            static_cast<QRadioButton *>(HTTPsMap["Enable"])->setChecked(param.Enabled ? true : false);
            static_cast<QLineEdit *>(HTTPsMap["HTTPs Port"])->setText(QString::number(param.HTTPsPort));
        }
        break;
    }
    case P2P: {
        VidiconProtocol::P2PParameter param;

        isOK = ParseXML::getInstance()->parseP2PParameter(&param, data);
        if (isOK) {
            static_cast<QRadioButton *>(P2PMap["P2P"])->setChecked(param.Enabled ? true : false);
            static_cast<QLineEdit *>(P2PMap["UUID"])->setText(param.P2PUUID);
        }
        break;
    }
    default:
        return;
    }

    if (isOK)
        qDebug() << "#NetworkWidget# handleReceiveData, ParameterType:" << type << "parse data success...";
    else
        qDebug() << "#NetworkWidget# handleReceiveData, ParameterType:" << type << "parse data error...";
}
