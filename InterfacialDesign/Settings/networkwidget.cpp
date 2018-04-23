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

NetworkWidget::NetworkWidget(QWidget *parent) :
    StackedWidget(parent)
{
    initTCPIPWidget();

//    initPPPOEWidget();
//    initDDNSClientWidget();
//    initEmailWidget();
//    initFTPWidget();
//    initBonjourWidget();
//    initSNMPWidget();
//    initUPNPWidget();
//    initHTTPsWidget();
//    initP2PWidget();
//    initRTSPWidget();
}

NetworkWidget::~NetworkWidget()
{
}

void NetworkWidget::initTCPIPWidget()
{
    QStringList list;
    m_tcpIpWidget = new QWidget(this);

    QString ipRegExpStr = "((25[0-5]|2[0-4]\\d|1\\d{2}|[1-9]?\\d)\\.){3}(25[0-5]|2[0-4]\\d|1\\d{2}|[1-9]?\\d)";
    QString macRegExpStr = "(([0-9a-z]{2}):){5}([0-9a-z]{2})";
    QRegExpValidator *ipValidator = new QRegExpValidator(QRegExp(ipRegExpStr), this);
    QRegExpValidator *macValidator = new QRegExpValidator(QRegExp(macRegExpStr), this);

    QLabel *lbl1 = new QLabel("最大连接数：", m_tcpIpWidget);
    LineEdit *lineEdit1 = new LineEdit(m_tcpIpWidget);
    m_tcpIpMap.insert("Max connection", lineEdit1);

    QLabel *lbl2 = new QLabel("DHCP启用：", m_tcpIpWidget);
    QComboBox *comboBox1 = new QComboBox(m_tcpIpWidget);
    list << "Disable" << "Enable";
    comboBox1->addItems(list);
    list.clear();
    m_tcpIpMap.insert("DHCP", comboBox1);

    QLabel *lbl3 = new QLabel("IPv4地址：", m_tcpIpWidget);
    LineEdit *lineEdit2 = new LineEdit(m_tcpIpWidget);
    lineEdit2->setValidator(ipValidator);
    connect(lineEdit2, &LineEdit::textChanged, this, &NetworkWidget::checkInput);
    m_tcpIpMap.insert("IPv4 Address", lineEdit2);

    QLabel *lbl4 = new QLabel("IPv4掩码地址：", m_tcpIpWidget);
    LineEdit *lineEdit3 = new LineEdit(m_tcpIpWidget);
    lineEdit3->setValidator(ipValidator);
    connect(lineEdit3, &LineEdit::textChanged, this, &NetworkWidget::checkInput);
    m_tcpIpMap.insert("IPv4 Subnet", lineEdit3);

    QLabel *lbl5 = new QLabel("IPv4网关地址：", m_tcpIpWidget);
    LineEdit *lineEdit4 = new LineEdit(m_tcpIpWidget);
    lineEdit4->setValidator(ipValidator);
    connect(lineEdit4, &LineEdit::textChanged, this, &NetworkWidget::checkInput);
    m_tcpIpMap.insert("IPv4 Gateway", lineEdit4);

    QLabel *lbl6 = new QLabel("IPv4主DNS：", m_tcpIpWidget);
    LineEdit *lineEdit5 = new LineEdit(m_tcpIpWidget);
    lineEdit5->setValidator(ipValidator);
    connect(lineEdit5, &LineEdit::textChanged, this, &NetworkWidget::checkInput);
    m_tcpIpMap.insert("IPv4 DNS 1", lineEdit5);

    QLabel *lbl7 = new QLabel("IPv4备用DNS：", m_tcpIpWidget);
    LineEdit *lineEdit6 = new LineEdit(m_tcpIpWidget);
    lineEdit6->setValidator(ipValidator);
    connect(lineEdit6, &LineEdit::textChanged, this, &NetworkWidget::checkInput);
    m_tcpIpMap.insert("IPv4 DNS 2", lineEdit6);

    QLabel *lbl8 = new QLabel("MAC地址：", m_tcpIpWidget);
    LineEdit *lineEdit7 = new LineEdit(m_tcpIpWidget);
    lineEdit7->setValidator(macValidator);
    connect(lineEdit7, &LineEdit::textChanged, this, &NetworkWidget::checkInput);
    m_tcpIpMap.insert("IPv4 MacAddr", lineEdit7);

    QLabel *lbl9 = new QLabel("IPv6地址：", m_tcpIpWidget);
    LineEdit *lineEdit8 = new LineEdit(m_tcpIpWidget);
    m_tcpIpMap.insert("IPv6 Address", lineEdit8);

    QLabel *lbl10 = new QLabel("IPv6掩码地址：", m_tcpIpWidget);
    LineEdit *lineEdit9 = new LineEdit(m_tcpIpWidget);
    m_tcpIpMap.insert("IPv6 Gateway", lineEdit9);

    QLabel *lbl11 = new QLabel("IPv6主DNS：", m_tcpIpWidget);
    LineEdit *lineEdit10 = new LineEdit(m_tcpIpWidget);
    m_tcpIpMap.insert("IPv6 DNS 1", lineEdit10);

    QLabel *lbl12 = new QLabel("IPv6备用DNS：", m_tcpIpWidget);
    LineEdit *lineEdit11 = new LineEdit(m_tcpIpWidget);
    m_tcpIpMap.insert("IPv6 DNS 2", lineEdit11);

    //目前ipv6还未完全推广，暂不使用
    lbl9->setVisible(false);
    lbl10->setVisible(false);
    lbl11->setVisible(false);
    lbl12->setVisible(false);

    lineEdit8->setVisible(false);
    lineEdit9->setVisible(false);
    lineEdit10->setVisible(false);
    lineEdit11->setVisible(false);

    QLabel *lbl13 = new QLabel("HTTP端口(1-65535)：", m_tcpIpWidget);
    LineEdit *lineEdit12 = new LineEdit(m_tcpIpWidget);
    lineEdit12->setValidator(new QIntValidator(1, 65535, m_tcpIpWidget));
    connect(lineEdit12, &LineEdit::textChanged, this, &NetworkWidget::checkInput);
    m_tcpIpMap.insert("HTTP Port", lineEdit12);

    QLabel *lbl14 = new QLabel("Onvif端口(1-65535)：", m_tcpIpWidget);
    LineEdit *lineEdit13 = new LineEdit(m_tcpIpWidget);
    lineEdit13->setValidator(new QIntValidator(1, 65535, m_tcpIpWidget));
    connect(lineEdit13, &LineEdit::textChanged, this, &NetworkWidget::checkInput);
    m_tcpIpMap.insert("Onvif Port", lineEdit13);

    QLabel *lbl15 = new QLabel("RTSP端口(1-65535)：", m_tcpIpWidget);
    LineEdit *lineEdit14 = new LineEdit(m_tcpIpWidget);
    lineEdit14->setValidator(new QIntValidator(1, 65535, m_tcpIpWidget));
    connect(lineEdit14, &LineEdit::textChanged, this, &NetworkWidget::checkInput);
    m_tcpIpMap.insert("RTSP Port", lineEdit14);

    QPushButton *btn = new QPushButton("保存", m_tcpIpWidget);
    connect(btn, &QPushButton::clicked, this, &NetworkWidget::handlePrepareData);
    m_tcpIpMap.insert("save", btn);

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

    setAlignment(m_tcpIpWidget, layout1, Qt::AlignTop | Qt::AlignHCenter);
    addWidget(m_tcpIpWidget);
}

void NetworkWidget::initPPPOEWidget()
{
    m_PPPOEWidget = new QWidget(this);

    QRadioButton *rBtn = new QRadioButton("启用PPPOE", m_PPPOEWidget);
    m_PPPOEMap.insert("Enable", rBtn);

    QLabel *lbl1 = new QLabel("用户名：", m_PPPOEWidget);
    LineEdit *lineEdit1 = new LineEdit(m_PPPOEWidget);
    m_PPPOEMap.insert("Username", lineEdit1);

    QLabel *lbl2 = new QLabel("密码：", m_PPPOEWidget);
    LineEdit *lineEdit2 = new LineEdit(m_PPPOEWidget);
    m_PPPOEMap.insert("Password", lineEdit1);

    QPushButton *btn = new QPushButton("保存", m_PPPOEWidget);
    btn->setFixedWidth(50);

    QGridLayout *layout1 = new QGridLayout;
    layout1->addWidget(rBtn,      0, 0, 1, 1);

    layout1->addWidget(lbl1,      1, 0, 1, 1);
    layout1->addWidget(lineEdit1, 1, 1, 1, 2);

    layout1->addWidget(lbl2,      2, 0, 1, 1);
    layout1->addWidget(lineEdit2, 2, 1, 1, 2);

    layout1->addWidget(btn,       3, 0, 1, 3, Qt::AlignCenter);

    setAlignment(m_PPPOEWidget, layout1, Qt::AlignTop | Qt::AlignHCenter);
    addWidget(m_PPPOEWidget);
}

void NetworkWidget::initDDNSClientWidget()
{
    QStringList list;
    m_DDNSClientWidget = new QWidget(this);

    QRadioButton *rBtn = new QRadioButton("启用DDNS", m_DDNSClientWidget);
    m_DDNSClientMap.insert("Enable", rBtn);

    QLabel *lbl1 = new QLabel("协议类型：", m_DDNSClientWidget);
    QComboBox *comboBox = new QComboBox();
    list << "3322.org" << "DynDDNS" << "Noip";
    comboBox->addItems(list);
    list.clear();
    m_DDNSClientMap.insert("Provider", comboBox);

    QLabel *lbl2 = new QLabel("DDNS服务器：", m_DDNSClientWidget);
    LineEdit *lineEdit1 = new LineEdit(m_DDNSClientWidget);
    m_DDNSClientMap.insert("Server", lineEdit1);

    QLabel *lbl3 = new QLabel("DDNS域名：", m_DDNSClientWidget);
    LineEdit *lineEdit2 = new LineEdit(m_DDNSClientWidget);
    m_DDNSClientMap.insert("Hostname", lineEdit2);

    QLabel *lbl4 = new QLabel("用户名：", m_DDNSClientWidget);
    LineEdit *lineEdit3 = new LineEdit(m_DDNSClientWidget);
    m_DDNSClientMap.insert("Username", lineEdit3);

    QLabel *lbl5 = new QLabel("密码：", m_DDNSClientWidget);
    LineEdit *lineEdit4 = new LineEdit(m_DDNSClientWidget);
    m_DDNSClientMap.insert("Password", lineEdit4);

    QPushButton *btn = new QPushButton("保存", m_PPPOEWidget);
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

    setAlignment(m_DDNSClientWidget, layout1, Qt::AlignTop | Qt::AlignHCenter);
    addWidget(m_DDNSClientWidget);
}

void NetworkWidget::initEmailWidget()
{
    m_EmailWidget = new QWidget(this);

    QRadioButton *rBtn = new QRadioButton("启用邮箱", m_EmailWidget);
    m_EmailMap.insert("Enable", rBtn);

    QLabel *lbl1 = new QLabel("移动侦测标题：", m_EmailWidget);
    LineEdit *lineEdit1 = new LineEdit(m_EmailWidget);
    m_EmailMap.insert("Motion Subject", lineEdit1);

    QLabel *lbl2 = new QLabel("探头报警标题：", m_EmailWidget);
    LineEdit *lineEdit2 = new LineEdit(m_EmailWidget);
    m_EmailMap.insert("Alarm Subject", lineEdit2);

    QLabel *lbl3 = new QLabel("SMTP服务器：", m_EmailWidget);
    LineEdit *lineEdit3 = new LineEdit(m_EmailWidget);
    m_EmailMap.insert("SMTP Server", lineEdit3);

    QLabel *lbl4 = new QLabel("SMTP端口：", m_EmailWidget);
    LineEdit *lineEdit4 = new LineEdit(m_EmailWidget);
    m_EmailMap.insert("SMTP Port", lineEdit4);

    QLabel *lbl5 = new QLabel("发送人E-mail：", m_EmailWidget);
    LineEdit *lineEdit5 = new LineEdit(m_EmailWidget);
    m_EmailMap.insert("Sender Address", lineEdit5);

    QLabel *lbl6 = new QLabel("密码：", m_EmailWidget);
    LineEdit *lineEdit6 = new LineEdit(m_EmailWidget);
    m_EmailMap.insert("Sender Password", lineEdit6);

    QLabel *lbl7 = new QLabel("收件人E-mail：", m_EmailWidget);
    LineEdit *lineEdit7 = new LineEdit(m_EmailWidget);
    m_EmailMap.insert("Recipient Address-1", lineEdit7);

    QLabel *lbl8 = new QLabel("收件人E-mail：", m_EmailWidget);
    LineEdit *lineEdit8 = new LineEdit(m_EmailWidget);
    m_EmailMap.insert("Recipient Address-2", lineEdit8);

    QLabel *lbl9 = new QLabel("收件人E-mail：", m_EmailWidget);
    LineEdit *lineEdit9 = new LineEdit(m_EmailWidget);
    m_EmailMap.insert("Recipient Address-3", lineEdit9);

    QLabel *lbl10 = new QLabel("收件人E-mail：", m_EmailWidget);
    LineEdit *lineEdit10 = new LineEdit(m_EmailWidget);
    m_EmailMap.insert("Recipient Address-4", lineEdit10);

    QPushButton *btn1 = new QPushButton("保存", m_EmailWidget);
    btn1->setFixedWidth(50);

    QPushButton *btn2 = new QPushButton("测试", m_EmailWidget);
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

    setAlignment(m_EmailWidget, layout1, Qt::AlignTop | Qt::AlignHCenter);
    addWidget(m_EmailWidget);
}

void NetworkWidget::initFTPWidget()
{
    QStringList list;
    m_FTPWidget = new QWidget(this);

    QRadioButton *rBtn = new QRadioButton("启用FTP服务器", m_FTPWidget);
    m_FTPMap.insert("Enable", rBtn);

    QLabel *lbl1 = new QLabel("FTP服务器", m_FTPWidget);
    LineEdit *lineEdit1 = new LineEdit(m_FTPWidget);
    m_FTPMap.insert("Server", lineEdit1);

    QLabel *lbl2 = new QLabel("FTP端口：", m_FTPWidget);
    LineEdit *lineEdit2 = new LineEdit(m_FTPWidget);
    m_FTPMap.insert("Port", lineEdit2);

    QLabel *lbl3 = new QLabel("模式：", m_FTPWidget);
    QComboBox *comboBox = new QComboBox();
    list << "被动" << "主动";
    comboBox->addItems(list);
    list.clear();
    m_FTPMap.insert("Mode", comboBox);

    QLabel *lbl4 = new QLabel("用户名：", m_FTPWidget);
    LineEdit *lineEdit3 = new LineEdit(m_FTPWidget);
    m_FTPMap.insert("Username", lineEdit3);

    QLabel *lbl5 = new QLabel("密码：", m_FTPWidget);
    LineEdit *lineEdit4 = new LineEdit(m_FTPWidget);
    m_FTPMap.insert("Password", lineEdit4);

    QLabel *lbl6 = new QLabel("上传路径：", m_FTPWidget);
    LineEdit *lineEdit5 = new LineEdit(m_FTPWidget);
    m_FTPMap.insert("Upload Directory", lineEdit5);

    QPushButton *btn1 = new QPushButton("保存", m_EmailWidget);
    btn1->setFixedWidth(50);

    QPushButton *btn2 = new QPushButton("测试", m_EmailWidget);
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

    setAlignment(m_FTPWidget, layout1, Qt::AlignTop | Qt::AlignHCenter);
    addWidget(m_FTPWidget);
}

void NetworkWidget::initBonjourWidget()
{
    m_BonjourWidget = new QWidget(this);

    QRadioButton *rBtn = new QRadioButton("启用Bonjour", m_BonjourWidget);
    m_BonjourMap.insert("Enable", rBtn);

    QLabel *lbl1 = new QLabel("Bonjour：", m_BonjourWidget);
    LineEdit *lineEdit1 = new LineEdit(m_BonjourWidget);
    m_BonjourMap.insert("Bonjour", lineEdit1);

    QPushButton *btn1 = new QPushButton("恢复默认值", m_EmailWidget);
    btn1->setFixedWidth(100);

    QPushButton *btn2 = new QPushButton("更新", m_EmailWidget);
    btn2->setFixedWidth(50);

    QPushButton *btn3 = new QPushButton("保存", m_EmailWidget);
    btn3->setFixedWidth(50);

    QGridLayout *layout1 = new QGridLayout;
    layout1->addWidget(rBtn,      0, 0, 1, 1);

    layout1->addWidget(lbl1,      1, 0, 1, 1);
    layout1->addWidget(lineEdit1, 1, 1, 1, 2);

    layout1->addWidget(btn1,      2, 1, 1, 1, Qt::AlignLeft);
    layout1->addWidget(btn2,      2, 2, 1, 1, Qt::AlignRight);

    layout1->addWidget(btn3,      3, 1, 1, 1, Qt::AlignCenter);

    setAlignment(m_BonjourWidget, layout1, Qt::AlignTop | Qt::AlignHCenter);
    addWidget(m_BonjourWidget);
}

void NetworkWidget::initSNMPWidget()
{
    m_SNMPWidget = new QWidget(this);

    QCheckBox *cb1 = new QCheckBox("SNMP v1", m_SNMPWidget);
    m_SNMPMap.insert("SNMP v1", cb1);

    QCheckBox *cb2 = new QCheckBox("SNMP v2", m_SNMPWidget);
    m_SNMPMap.insert("SNMP v2", cb2);

    QLabel *lbl1 = new QLabel("SNMP端口(1-65535)：", m_SNMPWidget);
    LineEdit *lineEdit1 = new LineEdit(m_SNMPWidget);
    m_SNMPMap.insert("SNMP Port", lineEdit1);

    QLabel *lbl2 = new QLabel("Read Community：", m_SNMPWidget);
    LineEdit *lineEdit2 = new LineEdit(m_SNMPWidget);
    m_SNMPMap.insert("Read Community", lineEdit2);

    QLabel *lbl3 = new QLabel("Write Community：", m_SNMPWidget);
    LineEdit *lineEdit3 = new LineEdit(m_SNMPWidget);
    m_SNMPMap.insert("Write Community", lineEdit3);

    QLabel *lbl4 = new QLabel("Trap地址：", m_SNMPWidget);
    LineEdit *lineEdit4 = new LineEdit(m_SNMPWidget);
    m_SNMPMap.insert("Trap Address", lineEdit4);

    QLabel *lbl5 = new QLabel("Trap端口(1-65535)：", m_SNMPWidget);
    LineEdit *lineEdit5 = new LineEdit(m_SNMPWidget);
    m_SNMPMap.insert("Trap Port", lineEdit5);

    QPushButton *btn = new QPushButton("保存", m_EmailWidget);
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

    setAlignment(m_SNMPWidget, layout1, Qt::AlignTop | Qt::AlignHCenter);
    addWidget(m_SNMPWidget);
}

void NetworkWidget::initUPNPWidget()
{
    QStringList list;
    m_UPNPWidget = new QWidget(this);

    QLabel *lbl = new QLabel("是否启用UPNP：", m_UPNPWidget);
    QComboBox *comboBox = new QComboBox();
    list << "关闭" << "打开";
    comboBox->addItems(list);
    list.clear();
    m_UPNPMap.insert("UPNP", comboBox);

    QPushButton *btn = new QPushButton("保存", m_UPNPWidget);
    btn->setFixedWidth(50);

    QGridLayout *layout1 = new QGridLayout;
    layout1->addWidget(lbl,      0, 0, 1, 1);
    layout1->addWidget(comboBox, 0, 1, 1, 2);

    layout1->addWidget(btn,      1, 1, 1, 1, Qt::AlignCenter);

    setAlignment(m_UPNPWidget, layout1, Qt::AlignTop | Qt::AlignHCenter);
    addWidget(m_UPNPWidget);
}

void NetworkWidget::initHTTPsWidget()
{
    m_HTTPsWidget = new QWidget(this);

    QRadioButton *rBtn = new QRadioButton("启用HTTPs", m_HTTPsWidget);
    m_HTTPsMap.insert("Enable", rBtn);

    QLabel *lbl = new QLabel("HTTPs端口(1-65535)：", m_HTTPsWidget);
    LineEdit *lineEdit = new LineEdit(m_HTTPsWidget);
    m_HTTPsMap.insert("HTTPs Port", lineEdit);

    QPushButton *btn = new QPushButton("保存", m_HTTPsWidget);
    btn->setFixedWidth(50);

    QGridLayout *layout1 = new QGridLayout;
    layout1->addWidget(rBtn,     0, 0, 1, 1);
    layout1->addWidget(lbl,      1, 0, 1, 1);
    layout1->addWidget(lineEdit, 1, 1, 1, 2);

    layout1->addWidget(btn,      2, 1, 1, 1, Qt::AlignCenter);

    setAlignment(m_HTTPsWidget, layout1, Qt::AlignTop | Qt::AlignHCenter);
    addWidget(m_HTTPsWidget);
}

void NetworkWidget::initP2PWidget()
{
    m_P2PWidget = new QWidget(this);

    QRadioButton *rBtn = new QRadioButton("启用P2P", m_P2PWidget);
    m_P2PMap.insert("P2P", rBtn);

    QLabel *lbl1 = new QLabel("UUID：", m_P2PWidget);
    LineEdit *lineEdit1 = new LineEdit(m_P2PWidget);
    lineEdit1->setFixedWidth(400);
    m_P2PMap.insert("UUID", lineEdit1);

    QLabel *lbl2 = new QLabel("安卓应用：", m_P2PWidget);
    LineEdit *lineEdit2 = new LineEdit("http://www.zwcloud.wang/p2p_direct.html", m_P2PWidget);

    QLabel *lbl3 = new QLabel("IOS应用：", m_P2PWidget);
    LineEdit *lineEdit3 = new LineEdit("https://itunes.apple.com/us/app/p2pcmscam/id1173582301?mt=8", m_P2PWidget);

    QPushButton *btn = new QPushButton("Save", m_P2PWidget);
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

    setAlignment(m_P2PWidget, layout1, Qt::AlignTop | Qt::AlignHCenter);
    addWidget(m_P2PWidget);
}

void NetworkWidget::initRTSPWidget()
{
    m_RTSPWidget = new QWidget(this);

    QRadioButton *rBtn = new QRadioButton("启用RTSP", m_RTSPWidget);
    m_RTSPMap.insert("Enable", rBtn);

    QLabel *lbl1 = new QLabel("RTSP端口：", m_RTSPWidget);
    LineEdit *lineEdit1 = new LineEdit(m_RTSPWidget);
    m_RTSPMap.insert("RTSP Port", lineEdit1);

    QPushButton *btn = new QPushButton("保存", m_P2PWidget);
    btn->setFixedWidth(50);

    QLabel *lbl2 = new QLabel("URL例子：", m_RTSPWidget);
    QLabel *lbl3 = new QLabel("rtsp://IP:PORT/H264?channel=1&subtype=A&unicast=true&proto=Onvif/video\n"
                              "A:0(Main Steam) 1(Sub Stream)", m_RTSPWidget);
    lbl3->setFixedWidth(450);

    QGridLayout *layout1 = new QGridLayout;
    layout1->addWidget(rBtn,      0, 0, 1, 1);

    layout1->addWidget(lbl1,      1, 0, 1, 1);
    layout1->addWidget(lineEdit1, 1, 1, 1, 2);

    layout1->addWidget(btn,       2, 0, 1, 3, Qt::AlignCenter);

    layout1->addWidget(lbl2,      3, 0, 1, 1, Qt::AlignTop);
    layout1->addWidget(lbl3,      3, 1, 1, 2, Qt::AlignTop);

    setAlignment(m_RTSPWidget, layout1, Qt::AlignTop | Qt::AlignHCenter);
    addWidget(m_RTSPWidget);
}

void NetworkWidget::setCurrentIndex(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    m_notPass.clear();

    switch(index.row()){
    case 0: {
        emit signalGetParameter(VidiconProtocol::TCPIP);
        emit signalGetParameter(VidiconProtocol::OTHER);
        break;
    }
    case 1: {
        emit signalGetParameter(VidiconProtocol::PPPOE);
        break;
    }
    case 2: {
        emit signalGetParameter(VidiconProtocol::DDNS);
        break;
    }
    case 3: {
        emit signalGetParameter(VidiconProtocol::EMAIL);
        break;
    }
    case 4: {
        emit signalGetParameter(VidiconProtocol::FTP);
        break;
    }
    case 5: {
        emit signalGetParameter(VidiconProtocol::BONJOUR);
        break;
    }
    case 6: {
        emit signalGetParameter(VidiconProtocol::SNMP);
        break;
    }
    case 7: {
        emit signalGetParameter(VidiconProtocol::UPNP);
        break;
    }
    case 8: {
        emit signalGetParameter(VidiconProtocol::HTTPS);
        break;
    }
    case 9: {
        emit signalGetParameter(VidiconProtocol::P2P);
        break;
    }
    case 10: {
        emit signalGetParameter(VidiconProtocol::RTSP);
        break;
    }
    default:
        break;
    }

    StackedWidget::setCurrentIndex(index.row());
}

void NetworkWidget::handlePrepareData()
{
    switch(currentIndex()) {
    case 0: {
        BasicParameter param1;
        param1.MaxLink = static_cast<LineEdit *>(m_tcpIpMap["Max connection"])->text().toInt();
        param1.DHCP = static_cast<QComboBox *>(m_tcpIpMap["DHCP"])->currentIndex();
        param1.ipv4.IpAddr = static_cast<LineEdit *>(m_tcpIpMap["IPv4 Address"])->text();
        param1.ipv4.SubnetMask = static_cast<LineEdit *>(m_tcpIpMap["IPv4 Subnet"])->text();
        param1.ipv4.Gateway = static_cast<LineEdit *>(m_tcpIpMap["IPv4 Gateway"])->text();
        param1.ipv4.DNS1 = static_cast<LineEdit *>(m_tcpIpMap["IPv4 DNS 1"])->text();
        param1.ipv4.DNS2 = static_cast<LineEdit *>(m_tcpIpMap["IPv4 DNS 2"])->text();
        param1.ipv6.IpAddr = static_cast<LineEdit *>(m_tcpIpMap["IPv6 Address"])->text();
        param1.ipv6.Gateway = static_cast<LineEdit *>(m_tcpIpMap["IPv6 Gateway"])->text();
        param1.ipv6.DNS1 = static_cast<LineEdit *>(m_tcpIpMap["IPv6 DNS 1"])->text();
        param1.ipv6.DNS2 = static_cast<LineEdit *>(m_tcpIpMap["IPv6 DNS 2"])->text();
        param1.MACAddress = static_cast<LineEdit *>(m_tcpIpMap["IPv4 MacAddr"])->text();
        emit signalSetParameter(VidiconProtocol::TCPIP, QVariant::fromValue(param1));

        OtherParameter param2;
        param2.type1.Enabled = 1;
        param2.type1.ServerType = 1;
        param2.type1.Port = static_cast<LineEdit *>(m_tcpIpMap["HTTP Port"])->text().toInt();
        param2.type2.Enabled = 1;
        param2.type2.ServerType = 2;
        param2.type2.Port = static_cast<LineEdit *>(m_tcpIpMap["Onvif Port"])->text().toInt();
        param2.type3.Enabled = 1;
        param2.type3.ServerType = 3;
        param2.type3.Port = static_cast<LineEdit *>(m_tcpIpMap["RTSP Port"])->text().toInt();
        emit signalSetParameter(VidiconProtocol::OTHER, QVariant::fromValue(param2));

        qDebug() << "#TabMedia# handleSendData send signal, ParameterType:" << VidiconProtocol::TCPIP << VidiconProtocol::OTHER;
        break;
    }
    default:
        break;
    }
}

void NetworkWidget::handleReceiveData(VidiconProtocol::Type type, QByteArray data)
{
    bool isOK = false;

    switch (type) {
    case VidiconProtocol::TCPIP: {
        BasicParameter param;

        isOK = ParseXML::getInstance()->parseBasicParameter(&param, data);
        if (isOK) {
            static_cast<LineEdit *>(m_tcpIpMap["Max connection"])->setText(QString::number(param.MaxLink));
            static_cast<QComboBox *>(m_tcpIpMap["DHCP"])->setCurrentIndex(param.DHCP);
            static_cast<LineEdit *>(m_tcpIpMap["IPv4 Address"])->setText(param.ipv4.IpAddr);
            static_cast<LineEdit *>(m_tcpIpMap["IPv4 Subnet"])->setText(param.ipv4.SubnetMask);
            static_cast<LineEdit *>(m_tcpIpMap["IPv4 Gateway"])->setText(param.ipv4.Gateway);
            static_cast<LineEdit *>(m_tcpIpMap["IPv4 DNS 1"])->setText(param.ipv4.DNS1);
            static_cast<LineEdit *>(m_tcpIpMap["IPv4 DNS 2"])->setText(param.ipv4.DNS2);
            static_cast<LineEdit *>(m_tcpIpMap["IPv6 Address"])->setText(param.ipv6.IpAddr);
            static_cast<LineEdit *>(m_tcpIpMap["IPv6 Gateway"])->setText(param.ipv6.Gateway);
            static_cast<LineEdit *>(m_tcpIpMap["IPv6 DNS 1"])->setText(param.ipv6.DNS1);
            static_cast<LineEdit *>(m_tcpIpMap["IPv6 DNS 2"])->setText(param.ipv6.DNS2);
            static_cast<LineEdit *>(m_tcpIpMap["IPv4 MacAddr"])->setText(param.MACAddress);
        }
        break;
    }
    case VidiconProtocol::OTHER: {
        OtherParameter param;
        OtherBasicParameter *param1 = (OtherBasicParameter *)(&param);

        isOK = ParseXML::getInstance()->parseOtherParameter(&param, data);
        if (isOK) {
            for(int i=1; i<=3; i++) {
                if(param1->ServerType == 1) {
                    static_cast<LineEdit *>(m_tcpIpMap["HTTP Port"])->setText(QString::number(param1->Port));
                }else if(param1->ServerType == 2) {
                    static_cast<LineEdit *>(m_tcpIpMap["Onvif Port"])->setText(QString::number(param1->Port));
                }else if(param1->ServerType == 3) {
                    static_cast<LineEdit *>(m_tcpIpMap["RTSP Port"])->setText(QString::number(param1->Port));
//                    static_cast<QRadioButton *>(m_RTSPMap["Enable"])->setChecked(param1->Enabled ? true : false);
//                    static_cast<LineEdit *>(m_RTSPMap["RTSP Port"])->setText(QString::number(param1->Port));
                }
                param1++;
            }
        }
        break;
    }
    case VidiconProtocol::PPPOE: {
        PPPOEParameter param;

        isOK = ParseXML::getInstance()->parsePPPOEParameter(&param, data);
        if (isOK) {
            static_cast<QRadioButton *>(m_PPPOEMap["Enable"])->setChecked(param.Enabled ? true : false);
            static_cast<LineEdit *>(m_PPPOEMap["Username"])->setText(param.PPPOEName);
            static_cast<LineEdit *>(m_PPPOEMap["Password"])->setText(param.PPPOEPassword);
        }
        break;
    }
    case VidiconProtocol::DDNS: {
        DDNSParameter param;

        isOK = ParseXML::getInstance()->parseDDNSParameter(&param, data);
        if (isOK) {
            static_cast<QRadioButton *>(m_DDNSClientMap["Enable"])->setChecked(param.Enabled ? true : false);
            static_cast<QComboBox *>(m_DDNSClientMap["Provider"])->setCurrentText(param.DDNSType);
            static_cast<LineEdit *>(m_DDNSClientMap["Server"])->setText(param.DDNSServerName);
            static_cast<LineEdit *>(m_DDNSClientMap["Hostname"])->setText(param.DDNSName);
            static_cast<LineEdit *>(m_DDNSClientMap["Username"])->setText(param.DDNSUser);
            static_cast<LineEdit *>(m_DDNSClientMap["Password"])->setText(param.DDNSPassword);
        }
        break;
    }
    case VidiconProtocol::EMAIL: {
        EmailParameter param;

        isOK = ParseXML::getInstance()->parseEmailParameter(&param, data);
        if (isOK) {
            static_cast<QRadioButton *>(m_EmailMap["Enable"])->setChecked(param.Enabled ? true : false);
            static_cast<LineEdit *>(m_EmailMap["Motion Subject"])->setText(param.MotionAlarmTitle);
            static_cast<LineEdit *>(m_EmailMap["Alarm Subject"])->setText(param.SensorAlarmTitle);
            static_cast<LineEdit *>(m_EmailMap["SMTP Server"])->setText(param.SmtpServer);
            static_cast<LineEdit *>(m_EmailMap["SMTP Port"])->setText(QString::number(param.SmtpPort));
            static_cast<LineEdit *>(m_EmailMap["Sender Address"])->setText(param.SmtpServer);
            static_cast<LineEdit *>(m_EmailMap["Sender Password"])->setText(param.SmtpPassword);
            static_cast<LineEdit *>(m_EmailMap["Recipient Address-1"])->setText(param.Receiver_1);
            static_cast<LineEdit *>(m_EmailMap["Recipient Address-2"])->setText(param.Receiver_2);
            static_cast<LineEdit *>(m_EmailMap["Recipient Address-3"])->setText(param.Receiver_3);
            static_cast<LineEdit *>(m_EmailMap["Recipient Address-4"])->setText(param.Receiver_4);
        }
        break;
    }
    case VidiconProtocol::FTP: {
        FTPParameter param;

        isOK = ParseXML::getInstance()->parseFTPParameter(&param, data);
        if (isOK) {
            static_cast<QRadioButton *>(m_FTPMap["Enable"])->setChecked(param.Enabled ? true : false);
            static_cast<LineEdit *>(m_FTPMap["Server"])->setText(param.FTPServer);
            static_cast<LineEdit *>(m_FTPMap["Port"])->setText(QString::number(param.FTPPort));
            static_cast<QComboBox *>(m_FTPMap["Mode"])->setCurrentIndex(param.FTPMode);
            static_cast<LineEdit *>(m_FTPMap["Username"])->setText(param.FTPUser);
            static_cast<LineEdit *>(m_FTPMap["Password"])->setText(param.FTPPassword);
            static_cast<LineEdit *>(m_FTPMap["Upload Directory"])->setText(param.UploadDirectory);
        }
        break;
    }
    case VidiconProtocol::BONJOUR: {
        BonjourParameter param;

        isOK = ParseXML::getInstance()->parseBonjourParameter(&param, data);
        if (isOK) {
            static_cast<QRadioButton *>(m_BonjourMap["Enable"])->setChecked(param.Enabled ? true : false);
            static_cast<LineEdit *>(m_BonjourMap["Bonjour"])->setText(param.Name);
        }
        break;
    }
    case VidiconProtocol::SNMP: {
        SNMPParameter param;

        isOK = ParseXML::getInstance()->parseSNMPParameter(&param, data);
        if (isOK) {
            static_cast<QCheckBox *>(m_SNMPMap["SNMP v1"])->setChecked(param.EnabledVer1 ? true : false);
            static_cast<QCheckBox *>(m_SNMPMap["SNMP v2"])->setChecked(param.EnabledVer2 ? true : false);
            static_cast<LineEdit *>(m_SNMPMap["SNMP Port"])->setText(QString::number(param.SnmpPort));
            static_cast<LineEdit *>(m_SNMPMap["Read Community"])->setText(param.ReadPublic);
            static_cast<LineEdit *>(m_SNMPMap["Write Community"])->setText(param.WritePublic);
            static_cast<LineEdit *>(m_SNMPMap["Trap Address"])->setText(param.TrapAddress);
            static_cast<LineEdit *>(m_SNMPMap["Trap Port"])->setText(QString::number(param.TrapPort));
        }
        break;
    }
    case VidiconProtocol::UPNP: {
        UPNPParameter param;

        isOK = ParseXML::getInstance()->parseUPNPParameter(&param, data);
        if (isOK) {
            static_cast<QComboBox *>(m_UPNPMap["UPNP"])->setCurrentIndex(param.Enabled);
        }
        break;
    }
    case VidiconProtocol::HTTPS: {
        HTTPsParameter param;

        isOK = ParseXML::getInstance()->parseHTTPsParameter(&param, data);
        if (isOK) {
            static_cast<QRadioButton *>(m_HTTPsMap["Enable"])->setChecked(param.Enabled ? true : false);
            static_cast<LineEdit *>(m_HTTPsMap["HTTPs Port"])->setText(QString::number(param.HTTPsPort));
        }
        break;
    }
    case VidiconProtocol::P2P: {
        P2PParameter param;

        isOK = ParseXML::getInstance()->parseP2PParameter(&param, data);
        if (isOK) {
            static_cast<QRadioButton *>(m_P2PMap["P2P"])->setChecked(param.Enabled ? true : false);
            static_cast<LineEdit *>(m_P2PMap["UUID"])->setText(param.P2PUUID);
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

void NetworkWidget::checkInput(QString text)
{
    QLineEdit *le = dynamic_cast<QLineEdit *>(sender());
    if (le == NULL)
        return;

    QPushButton *btn = NULL;
    switch (currentIndex()) {
    case 0:
        btn = dynamic_cast<QPushButton *>(m_tcpIpMap.value("save", NULL));
        break;
    default:
        break;
    }

    if (btn == NULL)
        return;

    int pos = 0;
    if (le->validator()->validate(text, pos) == QValidator::Acceptable) {
        m_notPass.removeAll(le);
    } else {
        m_notPass.append(le);
    }

    btn->setEnabled(m_notPass.isEmpty());
}
