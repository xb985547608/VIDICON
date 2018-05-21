#include "systemwidget.h"
#include <QGridLayout>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QSlider>
#include <QDebug>
#include <QModelIndex>
#include <QSpinBox>
#include <QDateEdit>
#include <QTimeEdit>
#include "userinfoview.h"
#include <QTimer>
#include "Protocol/vidiconprotocol.h"
#include "parsexml.h"
#include <QMessageBox>

SystemWidget::SystemWidget(QWidget *parent) :
    StackedWidget(parent)
{
    initMaintenanceWidget();
    initDeviceInfoWidget();
    initSetTimeWidget();
    initUserConfigWidget();
    initWifiConfigWidget();
}

SystemWidget::~SystemWidget()
{
}

void SystemWidget::initMaintenanceWidget()
{
    QStringList list;
    m_maintenanceWidget = new QWidget(this);

    QLabel *lbl1 = new QLabel("恢复出厂设置", m_maintenanceWidget);
    QPushButton *btn1 = new QPushButton("确定");
    btn1->setFixedWidth(80);
    connect(btn1, &QPushButton::clicked, this, [this]() {
        if(QMessageBox::question(this, "警告", "确定需要恢复出厂设置吗？") == QMessageBox::Yes) {
            emit signalSetParameter(VidiconProtocol::RECOVEDEFAULT);
        }
    });

    QLabel *lbl2 = new QLabel("重启设备", m_maintenanceWidget);
    QPushButton *btn2 = new QPushButton("确定");
    btn2->setFixedWidth(80);
    connect(btn2, &QPushButton::clicked, this, [this]() {
        if(QMessageBox::question(this, "警告", "确定需要重启吗？") == QMessageBox::Yes) {
            emit signalSetParameter(VidiconProtocol::REBOOT);
        }
    });

    QRadioButton *rBtn = new QRadioButton("自动重启", m_maintenanceWidget);

    QComboBox *comboBox = new QComboBox(m_maintenanceWidget);
    list << "每天" << "周一" << "周二" << "周三"
         << "周四" << "周五" << "周六" << "周天";
    comboBox->addItems(list);
    QTimeEdit *time1 = new QTimeEdit(m_maintenanceWidget);
    QPushButton *btn3 = new QPushButton("确定");
    btn3->setFixedWidth(80);

    QGridLayout *layout1 = new QGridLayout;
    layout1->addWidget(lbl1,      0, 0, 1, 1);
    layout1->addWidget(btn1,      0, 1, 1, 1);

    layout1->addWidget(lbl2,      1, 0, 1, 1);
    layout1->addWidget(btn2,      1, 1, 1, 1);

    layout1->addWidget(rBtn,      2, 0, 1, 1);
    layout1->addWidget(comboBox,  2, 1, 1, 1);
    layout1->addWidget(time1,     2, 2, 1, 1);
    layout1->addWidget(btn3,      2, 3, 1, 1);

    setAlignment(m_maintenanceWidget, layout1, Qt::AlignTop | Qt::AlignHCenter);
    addWidget(m_maintenanceWidget);
}

void SystemWidget::initDeviceInfoWidget()
{
    m_deviceInfoWidget = new QWidget(this);

    QLabel *lbl1 = new QLabel("设备名:", m_deviceInfoWidget);
    LineEdit *lineEdit1 = new LineEdit(m_deviceInfoWidget);
    lineEdit1->setReadOnly(true);
    m_deviceInfoMap.insert("DeviceName", lineEdit1);

    QLabel *lbl2 = new QLabel("设备模式:", m_deviceInfoWidget);
    LineEdit *lineEdit2 = new LineEdit(m_deviceInfoWidget);
    lineEdit2->setReadOnly(true);
    m_deviceInfoMap.insert("DeviceModel", lineEdit2);

    QLabel *lbl3 = new QLabel("软件版本:", m_deviceInfoWidget);
    LineEdit *lineEdit3 = new LineEdit(m_deviceInfoWidget);
    lineEdit3->setReadOnly(true);
    m_deviceInfoMap.insert("SoftwareVer", lineEdit3);

    QLabel *lbl4 = new QLabel("设备ID:", m_deviceInfoWidget);
    LineEdit *lineEdit4 = new LineEdit(m_deviceInfoWidget);
    lineEdit4->setReadOnly(true);
    m_deviceInfoMap.insert("DeviceID", lineEdit4);

    QGridLayout *layout1 = new QGridLayout;
    layout1->addWidget(lbl1,      0, 0, 1, 1);
    layout1->addWidget(lineEdit1, 0, 1, 1, 3);

    layout1->addWidget(lbl2,      2, 0, 1, 1);
    layout1->addWidget(lineEdit2, 2, 1, 1, 3);

    layout1->addWidget(lbl3,      4, 0, 1, 1);
    layout1->addWidget(lineEdit3, 4, 1, 1, 3);

    layout1->addWidget(lbl4,      1, 0, 1, 1);
    layout1->addWidget(lineEdit4, 1, 1, 1, 3);

    setAlignment(m_deviceInfoWidget, layout1, Qt::AlignTop | Qt::AlignHCenter);
    addWidget(m_deviceInfoWidget);
}

void SystemWidget::initSetTimeWidget()
{
    QStringList list;
    m_setTimeWidget = new QWidget(this);

    QLabel *lbl1 = new QLabel("时区：", m_setTimeWidget);
    QComboBox *comboBox1 = new QComboBox(m_setTimeWidget);
    for(int i=12; i>0; i--){
        list.append(QString("GMT-%1:00").arg(i, 2, 10, QChar('0')));
    }
    for(int i=0; i<=12; i++){
        list.append(QString("GMT+%1:00").arg(i, 2, 10, QChar('0')));
    }
    comboBox1->addItems(list);
    list.clear();
    m_setTimeMap.insert("Time Zone", comboBox1);

    QLabel *lbl2 = new QLabel("时间设置：", m_setTimeWidget);
    QDateEdit *date = new QDateEdit(m_setTimeWidget);
    date->setDate(QDate::currentDate());
    date->setCalendarPopup(true);
    m_setTimeMap.insert("date", date);
    QTimeEdit *time = new QTimeEdit(m_setTimeWidget);
    time->setDisplayFormat("hh:mm:ss");
    time->setCalendarPopup(true);
    m_setTimeMap.insert("time", time);
    QPushButton *btn1 = new QPushButton(m_setTimeWidget);
    btn1->setIcon(QIcon(":/images/refresh.png"));
    btn1->setFixedSize(25, 25);
    connect(btn1, &QPushButton::clicked, this, [date, time](){
        date->setDate(QDate::currentDate());
        time->setTime(QTime::currentTime());
    });

    QLabel *lbl3 = new QLabel("同步电脑时间：", m_setTimeWidget);
    QComboBox *comboBox2 = new QComboBox(m_setTimeWidget);
    list << "NO" << "YES";
    comboBox2->addItems(list);
    list.clear();
    m_setTimeMap.insert("PC Time Sync", comboBox2);

    QLabel *lbl4 = new QLabel("是否启用NTP：", m_setTimeWidget);
    QComboBox *comboBox3 = new QComboBox(m_setTimeWidget);
    list << "Disable" << "Enable";
    comboBox3->addItems(list);
    list.clear();
    m_setTimeMap.insert("NTP", comboBox3);

    QLabel *lbl5 = new QLabel("NTP服务器：", m_setTimeWidget);
    LineEdit *lineEdit1 = new LineEdit(m_setTimeWidget);
    m_setTimeMap.insert("NTP Server", lineEdit1);

    QPushButton *btn2 = new QPushButton("保存", m_setTimeWidget);
    btn2->setFixedWidth(50);
    connect(btn2, &QPushButton::clicked, this, &SystemWidget::handlePrepareData);

    QGridLayout *layout1 = new QGridLayout;
    layout1->addWidget(lbl1,      0, 0, 1, 1);
    layout1->addWidget(comboBox1, 0, 1, 1, 4);

    layout1->addWidget(lbl2,      1, 0, 1, 1);
    layout1->addWidget(date,      1, 1, 1, 2);
    layout1->addWidget(time,      1, 3, 1, 2);
    layout1->addWidget(btn1,      1, 5, 1, 1);

    layout1->addWidget(lbl3,      2, 0, 1, 1);
    layout1->addWidget(comboBox2, 2, 1, 1, 4);

    layout1->addWidget(lbl4,      3, 0, 1, 1);
    layout1->addWidget(comboBox3, 3, 1, 1, 4);

    layout1->addWidget(lbl5,      4, 0, 1, 1);
    layout1->addWidget(lineEdit1, 4, 1, 1, 4);

    layout1->addWidget(btn2, 5, 0, 1, 5, Qt::AlignCenter);

    setAlignment(m_setTimeWidget, layout1, Qt::AlignTop | Qt::AlignHCenter);
    addWidget(m_setTimeWidget);
}

void SystemWidget::initUserConfigWidget()
{
    m_userConfigWidget = new QWidget(this);

#if 0
    UserInfoView *view = new UserInfoView(m_userConfigWidget);
    m_userConfigMap.insert("view", view);

    QPushButton *btn = new QPushButton("新增用户", m_setTimeWidget);
    btn->setFixedWidth(100);
    connect(btn, &QPushButton::clicked, this, [view](){
        view->handleAddUserInfo();
    });

    QGridLayout *layout1 = new QGridLayout;
    layout1->addWidget(view, 0, 0, 1, 1);
    layout1->addWidget(btn,  1, 0, 1, 1);
#else
    QLabel *lbl1 = new QLabel("车号：", m_userConfigWidget);
    LineEdit *lineEdit1 = new LineEdit(m_userConfigWidget);
    lineEdit1->setReadOnly(false);
    lineEdit1->setMaxLength(6);
    m_userConfigMap.insert("train", lineEdit1);

    QLabel *lbl2 = new QLabel("车厢号：", m_userConfigWidget);
    LineEdit *lineEdit2 = new LineEdit(m_userConfigWidget);
    lineEdit2->setReadOnly(false);
    lineEdit2->setMaxLength(2);
    m_userConfigMap.insert("coach", lineEdit2);

    QLabel *lbl3 = new QLabel("车位号：", m_userConfigWidget);
    LineEdit *lineEdit3 = new LineEdit(m_userConfigWidget);
    lineEdit3->setReadOnly(false);
    lineEdit2->setMaxLength(2);
    m_userConfigMap.insert("seat", lineEdit3);

    QPushButton *btn = new QPushButton("保存", m_userConfigWidget);
    connect(btn, &QPushButton::clicked, this, [this](){
        QString train = static_cast<LineEdit *>(m_userConfigMap["train"])->text();
        QString coach = static_cast<LineEdit *>(m_userConfigMap["coach"])->text();
        QString seat  = static_cast<LineEdit *>(m_userConfigMap["seat"])->text();
        QString user = QString("CRH_%1_%2_%3")
                .arg(train)
                .arg(coach)
                .arg(seat);

        UserConfigInfo info;
        info.UserName = user;
        info.PassWord = user;
        info.Privilege = 0;
        emit signalSetParameter(VidiconProtocol::ADDUSER, QVariant::fromValue(info));
    });

    QGridLayout *layout1 = new QGridLayout;
    layout1->addWidget(lbl1,        0, 0, 1, 1);
    layout1->addWidget(lineEdit1,   0, 1, 1, 1);

    layout1->addWidget(lbl2,        1, 0, 1, 1);
    layout1->addWidget(lineEdit2,   1, 1, 1, 1);

    layout1->addWidget(lbl3,        2, 0, 1, 1);
    layout1->addWidget(lineEdit3,   2, 1, 1, 1);

    layout1->addWidget(btn,         3, 0, 1, 2, Qt::AlignCenter);
#endif

    setAlignment(m_userConfigWidget, layout1, Qt::AlignTop | Qt::AlignHCenter);
    addWidget(m_userConfigWidget);
}

void SystemWidget::initWifiConfigWidget()
{
    m_wifiSettinsWidget = new QWidget(this);

    QLabel *lbl1 = new QLabel("WIFI名：", m_wifiSettinsWidget);
    LineEdit *lineEdit1 = new LineEdit(m_wifiSettinsWidget);
    lineEdit1->setMaxLength(32);
    m_wifiSettinsMap.insert("ssid", lineEdit1);

    QLabel *lbl2 = new QLabel("密码：", m_wifiSettinsWidget);
    LineEdit *lineEdit2 = new LineEdit(m_wifiSettinsWidget);
    lineEdit2->setMaxLength(16);
    m_wifiSettinsMap.insert("passwd", lineEdit2);

    QPushButton *btn = new QPushButton("保存", m_wifiSettinsWidget);
    btn->setFixedWidth(50);
    connect(btn, &QPushButton::clicked, this, &SystemWidget::handlePrepareData);

    QGridLayout *layout1 = new QGridLayout;
    layout1->addWidget(lbl1,        0, 0, 1, 1);
    layout1->addWidget(lineEdit1,   0, 1, 1, 2);

    layout1->addWidget(lbl2,        1, 0, 1, 1);
    layout1->addWidget(lineEdit2,   1, 1, 1, 2);

    layout1->addWidget(btn,         2, 2, 1, 1);

    setAlignment(m_wifiSettinsWidget, layout1, Qt::AlignTop | Qt::AlignHCenter);
    addWidget(m_wifiSettinsWidget);
}

void SystemWidget::setCurrentIndex(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    switch(index.row()){
    case 0: {
        emit signalGetParameter(VidiconProtocol::SCHEDULE);
        break;
    }
    case 1: {
        emit signalGetParameter(VidiconProtocol::GETDEVICEINFO);
        break;
    }
    case 2: {
        emit signalGetParameter(VidiconProtocol::NTP);
        break;
    }
    case 3: {
        emit signalGetParameter(VidiconProtocol::USERCONFIG);
        break;
    }
    case 4: {
        emit signalGetParameter(VidiconProtocol::WIFI);
        break;
    }
    default:
        break;
    }

    StackedWidget::setCurrentIndex(index.row());
}

void SystemWidget::handlePrepareData()
{
    switch(currentIndex()) {
    case 2: {
        NTPParameter param;
        param.TZ = static_cast<QComboBox *>(m_setTimeMap["Time Zone"])->currentText();
        param.UTCDateTime = QString("%1T%2Z").arg(static_cast<QDateEdit *>(m_setTimeMap["date"])->date().toString("yyyy-MM-dd"))
                .arg(static_cast<QDateEdit *>(m_setTimeMap["time"])->time().toString("HH:mm:ss"));
        param.IsUpdateTime = static_cast<QComboBox *>(m_setTimeMap["PC Time Sync"])->currentIndex();
        param.Enabled = static_cast<QComboBox *>(m_setTimeMap["NTP"])->currentIndex();
        param.NTPServer = static_cast<LineEdit *>(m_setTimeMap["NTP Server"])->text();
        emit signalSetParameter(VidiconProtocol::NTP, QVariant::fromValue(param));
        break;
    }
    case 4: {
        WifiConfigParameter param;
        param.ssid = static_cast<LineEdit *>(m_wifiSettinsMap["ssid"])->text();
        param.passwd = static_cast<LineEdit *>(m_wifiSettinsMap["passwd"])->text();
        emit signalSetParameter(VidiconProtocol::WIFI, QVariant::fromValue(param));
        break;
    }
    default:
        break;
    }
}

void SystemWidget::handleReceiveData(VidiconProtocol::Type type, QByteArray data)
{
    bool isOK = false;

    switch (type) {
    case VidiconProtocol::NTP: {
        NTPParameter param;
        isOK = ParseXML::getInstance()->parseNTPParameter(&param, data);
        if (isOK) {
            static_cast<QComboBox *>(m_setTimeMap["Time Zone"])->setCurrentText(param.TZ);
            QStringList list = param.UTCDateTime.split(QRegExp("[^0-9]"), QString::SkipEmptyParts);
            static_cast<QDateEdit *>(m_setTimeMap["date"])->setDate(QDate(list.at(0).toInt(), list.at(1).toInt(), list.at(2).toInt()));
            static_cast<QDateEdit *>(m_setTimeMap["time"])->setTime(QTime(list.at(3).toInt(), list.at(4).toInt(), list.at(5).toInt()));
            static_cast<QComboBox *>(m_setTimeMap["PC Time Sync"])->setCurrentIndex(param.IsUpdateTime);
            static_cast<QComboBox *>(m_setTimeMap["NTP"])->setCurrentIndex(param.Enabled);
            static_cast<LineEdit *>(m_setTimeMap["NTP Server"])->setText(param.NTPServer);
        }
        break;
    }
    case VidiconProtocol::GETDEVICEINFO: {
        DeviceInfo param;
        isOK = ParseXML::getInstance()->parseDeviceInfo(&param, data);
        if (isOK) {
            static_cast<LineEdit *>(m_deviceInfoMap["DeviceName"])->setText(param.DeviceName);
            static_cast<LineEdit *>(m_deviceInfoMap["DeviceModel"])->setText(param.DeviceModel);
            static_cast<LineEdit *>(m_deviceInfoMap["SoftwareVer"])->setText(param.SoftwareVer);
            static_cast<LineEdit *>(m_deviceInfoMap["DeviceID"])->setText(QString::number(param.DeviceID));
        }
        break;
    }
    case VidiconProtocol::USERCONFIG: {
        QList<UserConfigInfo> param;
        isOK = ParseXML::getInstance()->parseUserConfgInfo(param, data);
        if (isOK) {
#if 0
            for (int i=0; i<param.size(); i++) {
                qDebug() << "#SystemWidget# "
                         << "UserName: " << param[i].UserName
                         << "PassWord: " << param[i].PassWord
                         << "Privilege: " << param[i].Privilege;
            }

            static_cast<UserInfoView *>(m_userConfigMap["view"])->setItems(param);
#else
            for (int i=0; i<param.size(); i++) {
                UserConfigInfo info = param.takeLast();
                if (info.UserName.length() == 16) {
                    if (info.UserName.left(3).compare("CRH") == 0) {
                        QStringList list = info.UserName.split('_');
                        if (list.count() == 4) {
                            static_cast<LineEdit *>(m_userConfigMap["train"])->setText(list.at(1));
                            static_cast<LineEdit *>(m_userConfigMap["coach"])->setText(list.at(2));
                            static_cast<LineEdit *>(m_userConfigMap["seat"])->setText(list.at(3));
                            break;
                        }
                    }
                }
            }
#endif
        }
        break;
    }
    case VidiconProtocol::WIFI: {
        WifiConfigParameter param;
        isOK = ParseXML::getInstance()->parseWifiConfgInfo(param, data);
        if (isOK) {
            static_cast<LineEdit *>(m_wifiSettinsMap["ssid"])->setText(param.ssid);
            static_cast<LineEdit *>(m_wifiSettinsMap["passwd"])->setText(param.passwd);
        }
        break;
    }
    default:
        return;
    }

    if (isOK)
        qDebug() << "#SystemWidget# handleReceiveData, ParameterType:" << type << "parse data success...";
    else
        qDebug() << "#SystemWidget# handleReceiveData, ParameterType:" << type << "parse data error...";
}
