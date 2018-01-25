#include "tabsystem.h"
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
#include "tableview.h"
#include <QTimer>
#include "Protocol/vidiconprotocol.h"
#include "parsexml.h"
#include <QMessageBox>

TabSystem::TabSystem(QWidget *parent) : QTabWidget(parent)
{
    initMaintenanceWidget();
    initDeviceInfoWidget();
    initSetTimeWidget();
    initUserAdminWidget();

    connect(VidiconProtocol::getInstance(), &VidiconProtocol::signalSendData, this, &TabSystem::handlerReceiveData);
    connect(this, &TabSystem::signalSetParameter, VidiconProtocol::getInstance(), &VidiconProtocol::handlerSetParameter);
    connect(this, &TabSystem::signalGetParameter, VidiconProtocol::getInstance(), &VidiconProtocol::handlerGetParameter);
    connect(this, &TabSystem::currentChanged, this, [this](){
        handlerSwitchTab(QModelIndex());
    });
}

TabSystem::~TabSystem()
{
    qDebug("delete TabSystem");
}

void TabSystem::initMaintenanceWidget()
{
    QStringList list;
    maintenanceWidget = new QWidget(this);

    QLabel *lbl1 = new QLabel("Factory Default", maintenanceWidget);
    QPushButton *btn1 = new QPushButton("Default");
    btn1->setFixedWidth(80);
    connect(btn1, &QPushButton::clicked, this, [this]() {
        if(QMessageBox::question(this, "警告", "确定需要恢复出厂设置吗？") == QMessageBox::Yes) {
            emit signalSetParameter(RECOVEDEFAULT, NULL);
        }
    });

    QLabel *lbl2 = new QLabel("Reboot", maintenanceWidget);
    QPushButton *btn2 = new QPushButton("OK");
    btn2->setFixedWidth(80);
    connect(btn2, &QPushButton::clicked, this, [this]() {
        if(QMessageBox::question(this, "警告", "确定需要重启吗？") == QMessageBox::Yes) {
            emit signalSetParameter(REBOOT, NULL);
        }
    });

    QRadioButton *rBtn = new QRadioButton("Auto Reboot", maintenanceWidget);

    QComboBox *comboBox = new QComboBox(maintenanceWidget);
    list << "Every Day" << "Monday" << "Tuesday" << "Wednesday"
         << "Thursday" << "Friday" << "Saturday" << "Sunday";
    comboBox->addItems(list);
    QTimeEdit *time1 = new QTimeEdit(maintenanceWidget);
    QPushButton *btn3 = new QPushButton("OK");
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

    QVBoxLayout *layout2 = new QVBoxLayout;
    layout2->addLayout(layout1);
    layout2->addStretch();

    QHBoxLayout *layout3 = new QHBoxLayout(maintenanceWidget);
    layout3->addStretch();
    layout3->addLayout(layout2);
    layout3->addStretch();

    addTab(maintenanceWidget, "Maintenance");
}

void TabSystem::initDeviceInfoWidget()
{
    deviceInfoWidget = new QWidget(this);

    QLabel *lbl1 = new QLabel("DeviceName:", deviceInfoWidget);
    QLineEdit *lineEdit1 = new QLineEdit(deviceInfoWidget);
    lineEdit1->setReadOnly(true);
    deviceInfoMap.insert("DeviceName", lineEdit1);

    QLabel *lbl2 = new QLabel("DeviceModel:", deviceInfoWidget);
    QLineEdit *lineEdit2 = new QLineEdit(deviceInfoWidget);
    lineEdit2->setReadOnly(true);
    deviceInfoMap.insert("DeviceModel", lineEdit2);

    QLabel *lbl3 = new QLabel("SoftwareVer:", deviceInfoWidget);
    QLineEdit *lineEdit3 = new QLineEdit(deviceInfoWidget);
    lineEdit3->setReadOnly(true);
    deviceInfoMap.insert("SoftwareVer", lineEdit3);

    QLabel *lbl4 = new QLabel("DeviceID:", deviceInfoWidget);
    QLineEdit *lineEdit4 = new QLineEdit(deviceInfoWidget);
    lineEdit4->setReadOnly(true);
    deviceInfoMap.insert("DeviceID", lineEdit4);

    QGridLayout *layout1 = new QGridLayout;
    layout1->addWidget(lbl1,      0, 0, 1, 1);
    layout1->addWidget(lineEdit1, 0, 1, 1, 3);

    layout1->addWidget(lbl2,      2, 0, 1, 1);
    layout1->addWidget(lineEdit2, 2, 1, 1, 3);

    layout1->addWidget(lbl3,      4, 0, 1, 1);
    layout1->addWidget(lineEdit3, 4, 1, 1, 3);

    layout1->addWidget(lbl4,      1, 0, 1, 1);
    layout1->addWidget(lineEdit4, 1, 1, 1, 3);

    QVBoxLayout *layout2 = new QVBoxLayout;
    layout2->addLayout(layout1);
    layout2->addStretch();

    QHBoxLayout *layout3 = new QHBoxLayout(deviceInfoWidget);
    layout3->addStretch();
    layout3->addLayout(layout2);
    layout3->addStretch();

    addTab(deviceInfoWidget, "Device Info");
}

void TabSystem::initSetTimeWidget()
{
    QStringList list;
    setTimeWidget = new QWidget(this);

    QLabel *lbl1 = new QLabel("Device Model：", setTimeWidget);
    QComboBox *comboBox1 = new QComboBox(setTimeWidget);
    for(int i=12; i>0; i--){
        list.append(QString("GMT-%1:00").arg(i, 2, 10, QChar('0')));
    }
    for(int i=0; i<=12; i++){
        list.append(QString("GMT+%1:00").arg(i, 2, 10, QChar('0')));
    }
    comboBox1->addItems(list);
    list.clear();
    setTimeMap.insert("Time Zone", comboBox1);

    QLabel *lbl2 = new QLabel("Date/Time：", setTimeWidget);
    QDateEdit *date = new QDateEdit(setTimeWidget);
    date->setDate(QDate::currentDate());
    date->setCalendarPopup(true);
    setTimeMap.insert("date", date);
    QTimeEdit *time = new QTimeEdit(setTimeWidget);
    time->setDisplayFormat("hh:mm:ss");
    time->setCalendarPopup(true);
    setTimeMap.insert("time", time);

    QLabel *lbl3 = new QLabel("PC Time Sync：", setTimeWidget);
    QComboBox *comboBox2 = new QComboBox(setTimeWidget);
    list << "NO" << "YES";
    comboBox2->addItems(list);
    list.clear();
    setTimeMap.insert("PC Time Sync", comboBox2);

    QLabel *lbl4 = new QLabel("NTP：", setTimeWidget);
    QComboBox *comboBox3 = new QComboBox(setTimeWidget);
    list << "Disable" << "Enable";
    comboBox3->addItems(list);
    list.clear();
    setTimeMap.insert("NTP", comboBox3);

    QLabel *lbl5 = new QLabel("NTP Server：", setTimeWidget);
    QLineEdit *lineEdit1 = new QLineEdit(setTimeWidget);
    setTimeMap.insert("NTP Server", lineEdit1);

    QPushButton *btn = new QPushButton("Save", setTimeWidget);
    btn->setFixedWidth(50);
    connect(btn, &QPushButton::clicked, this, &TabSystem::handlerPrepareData);

    QGridLayout *layout1 = new QGridLayout;
    layout1->addWidget(lbl1,      0, 0, 1, 1);
    layout1->addWidget(comboBox1, 0, 1, 1, 4);

    layout1->addWidget(lbl2,      1, 0, 1, 1);
    layout1->addWidget(date,      1, 1, 1, 2);
    layout1->addWidget(time,      1, 3, 1, 2);

    layout1->addWidget(lbl3,      2, 0, 1, 1);
    layout1->addWidget(comboBox2, 2, 1, 1, 4);

    layout1->addWidget(lbl4,      3, 0, 1, 1);
    layout1->addWidget(comboBox3, 3, 1, 1, 4);

    layout1->addWidget(lbl5,      4, 0, 1, 1);
    layout1->addWidget(lineEdit1, 4, 1, 1, 4);

    layout1->addWidget(btn, 5, 0, 1, 5, Qt::AlignCenter);

    QVBoxLayout *layout2 = new QVBoxLayout;
    layout2->addLayout(layout1);
    layout2->addStretch();

    QHBoxLayout *layout3 = new QHBoxLayout(setTimeWidget);
    layout3->addStretch();
    layout3->addLayout(layout2);
    layout3->addStretch();

    addTab(setTimeWidget, "Set Time");
}

void TabSystem::initUserAdminWidget()
{
    userAdminWidget = new QWidget(this);

    TableView *view = new TableView(userAdminWidget);
    QStringList list;
    for(int i=0; i<10; i++){
        list << QString::number(i) << "admin" << "Manager";
        view->addData(list);
        list.clear();
    }

    QPushButton *btn = new QPushButton("AddUser", setTimeWidget);
    btn->setFixedWidth(100);

    QGridLayout *layout1 = new QGridLayout;
    layout1->addWidget(view, 0, 0, 1, 1);

    layout1->addWidget(btn,  1, 0, 1, 1);

    QVBoxLayout *layout2 = new QVBoxLayout;
    layout2->addLayout(layout1);
    layout2->addStretch();

    QHBoxLayout *layout3 = new QHBoxLayout(userAdminWidget);
    layout3->addStretch(1);
    layout3->addLayout(layout2, 8);
    layout3->addStretch(1);

    addTab(userAdminWidget, "User Admin");
}

void TabSystem::handlerSwitchTab(const QModelIndex &index)
{
    int type = index.row();
    if(sender() != this) {
        setCurrentIndex(index.row());
    }else {
        type = currentIndex();
    }

    switch(type){
    case 0: {
//        emit signalGetParameter(SCHEDULEPARAMETER);
        break;
    }
    case 1: {
        emit signalGetParameter(DEVICEINFO);
        break;
    }
    case 2: {
        emit signalGetParameter(NTPPARAMETER);
        break;
    }
    case 3: {
//        emit signalGetParameter(DESTINATIONPARAMETER);
        break;
    }
    default:
        break;
    }
}

void TabSystem::handlerPrepareData()
{
    switch(currentIndex()) {
    case 2: {
        VidiconProtocol::NTPParameter *param = new VidiconProtocol::NTPParameter;
        param->TZ = static_cast<QComboBox *>(setTimeMap["Time Zone"])->currentText();
        param->UTCDateTime = QString("%1T%2Z").arg(static_cast<QDateEdit *>(setTimeMap["date"])->date().toString("yyyy-MM-dd"))
                .arg(static_cast<QDateEdit *>(setTimeMap["time"])->time().toString("HH:mm:ss"));
        param->IsUpdateTime = static_cast<QComboBox *>(setTimeMap["PC Time Sync"])->currentIndex();
        param->Enabled = static_cast<QComboBox *>(setTimeMap["NTP"])->currentIndex();
        param->NTPServer = static_cast<QLineEdit *>(setTimeMap["NTP Server"])->text();
        emit signalSetParameter(NTPPARAMETER, param);
        break;
    }
    default:
        break;
    }
}

void TabSystem::handlerReceiveData(int type, QByteArray data)
{
    switch (type) {
    case NTPPARAMETER: {
        VidiconProtocol::NTPParameter param;
        if(ParseXML::getInstance()->parseNTPParameter(&param, data)) {
            static_cast<QComboBox *>(setTimeMap["Time Zone"])->setCurrentText(param.TZ);
            QStringList list = param.UTCDateTime.split(QRegExp("[^0-9]"), QString::SkipEmptyParts);
            static_cast<QDateEdit *>(setTimeMap["date"])->setDate(QDate(list.at(0).toInt(), list.at(1).toInt(), list.at(2).toInt()));
            static_cast<QDateEdit *>(setTimeMap["time"])->setTime(QTime(list.at(3).toInt(), list.at(4).toInt(), list.at(5).toInt()));
            static_cast<QComboBox *>(setTimeMap["PC Time Sync"])->setCurrentIndex(param.IsUpdateTime);
            static_cast<QComboBox *>(setTimeMap["NTP"])->setCurrentIndex(param.Enabled);
            static_cast<QLineEdit *>(setTimeMap["NTP Server"])->setText(param.NTPServer);
            qDebug() << "#TabSystem# handlerReceiveData, ParameterType:" << type << "parse data success...";
        }else{
            qDebug() << "#TabSystem# handlerReceiveData, ParameterType:" << type << "parse data error...";
        }
        break;
    }
    case DEVICEINFO: {
        VidiconProtocol::DeviceInfo param;
        if(ParseXML::getInstance()->parseDeviceInfo(&param, data)) {
            static_cast<QLineEdit *>(deviceInfoMap["DeviceName"])->setText(param.DeviceName);
            static_cast<QLineEdit *>(deviceInfoMap["DeviceModel"])->setText(param.DeviceModel);
            static_cast<QLineEdit *>(deviceInfoMap["SoftwareVer"])->setText(param.SoftwareVer);
            static_cast<QLineEdit *>(deviceInfoMap["DeviceID"])->setText(QString::number(param.DeviceID));

            qDebug() << "#TabSystem# handlerReceiveData, ParameterType:" << type << "parse data success...";
        }else{
            qDebug() << "#TabSystem# handlerReceiveData, ParameterType:" << type << "parse data error...";
        }
        break;
    }
    default:
        break;
    }
}
