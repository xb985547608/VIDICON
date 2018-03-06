#include "settingswidget.h"
#include "ui/ui_settingsform.h"
#include <QDebug>
#include <QVBoxLayout>

SettinsWidget::SettinsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsForm)
{
    ui->setupUi(this);

    QVBoxLayout *vLayout = new QVBoxLayout(ui->sideBar);
    vLayout->setContentsMargins(0, 0, 0, 0);
    toolbox = new QToolBox(this);
    toolbox->setStyleSheet("QToolBox::tab {\
                               border-image:url(:/images/bgNormal.png);\
                               color: white;font-size:14px;\
                           }\
                           QToolBox::tab:selected { /* italicize selected tabs */\
                               border-image:url(:/images/bgActive.png);font-weight:bold;\
                           }\
                           QToolBoxButton{min-height:30px;margin:1px}");
    toolbox->layout()->setSpacing(0);
    vLayout->addWidget(toolbox);
    ui->displayArea->setStyleSheet("background-color:white");

    initSideBar();
    initDisplayArea();

    connect(toolbox,    &QToolBox::currentChanged, this, &SettinsWidget::handleToolBoxClicked);

    connect(boxMedia,   &BoxView::clicked, tabMedia,    &MediaWidget::handleSwitchTab);
    connect(boxNetwork, &BoxView::clicked, tabNetwork,  &NetworkWidget::handleSwitchTab);
    connect(boxAlarm,   &BoxView::clicked, tabAlarm,    &AlarmWidget::handleSwitchTab);
    connect(boxRecord,  &BoxView::clicked, tabRecord,   &RecordWidget::handleSwitchTab);
    connect(boxSystem,  &BoxView::clicked, tabSystem,   &SystemWidget::handleSwitchTab);
}

SettinsWidget::~SettinsWidget()
{

}

void SettinsWidget::initSideBar()
{
    QStringList list;

    boxMedia = new BoxView(toolbox);
    boxMedia->setFrameShape(QFrame::NoFrame);
    list << "音视频" << "隐私遮蔽" << "图像参数" << "ROI" << "OSD";
    boxMedia->setData(list);
    toolbox->addItem(boxMedia, QIcon(":/images/media.png"), "媒体设置");

    list.clear();
    boxNetwork = new BoxView(toolbox);
    list << "TCP/IP" << "PPPOE" << "DDNS Client" << "Email" << "FTP"
         << "Bonjour" << "SNMP" << "UPNP" << "HTTPs" << "P2P" << "RTSP";
    boxNetwork->setData(list);
    toolbox->addItem(boxNetwork, QIcon(":/images/network.png"), "网络设置");

    list.clear();
    boxAlarm = new BoxView(toolbox);
    list << "移动侦测" << "遮挡报警" << "探头报警";
    boxAlarm->setData(list);
    toolbox->addItem(boxAlarm, QIcon(":/images/alarm.png"), "报警设置");

    list.clear();
    boxRecord = new BoxView(toolbox);
    list << "远程录像计划" << "SD卡管理" << "抓拍设置" /*<< "Destination" << "NAS"*/;
    boxRecord->setData(list);
    toolbox->addItem(boxRecord, QIcon(":/images/record.png"), "记录");

    list.clear();
    boxSystem = new BoxView(toolbox);
    list << "维护" << "设备信息" << "时间设置"/* << "User Admin"*/;
    boxSystem->setData(list);
    toolbox->addItem(boxSystem, QIcon(":/images/system.png"), "系统设置");
}

void SettinsWidget::initDisplayArea()
{
    QVBoxLayout *layout = new QVBoxLayout(ui->displayArea);
    layout->setContentsMargins(0, 0, 0, 0);

    tabMedia = new MediaWidget(ui->displayArea);
    tabMedia->setVisible(true);
    layout->addWidget(tabMedia);

    tabNetwork = new NetworkWidget(ui->displayArea);
    tabNetwork->setVisible(false);
    layout->addWidget(tabNetwork);

    tabAlarm = new AlarmWidget(ui->displayArea);
    tabAlarm->setVisible(false);
    layout->addWidget(tabAlarm);

    tabRecord = new RecordWidget(ui->displayArea);
    tabRecord->setVisible(false);
    layout->addWidget(tabRecord);

    tabSystem = new SystemWidget(ui->displayArea);
    tabSystem->setVisible(false);
    layout->addWidget(tabSystem);
}

void SettinsWidget::handleToolBoxClicked(int index)
{
    tabMedia->setVisible(false);
    tabNetwork->setVisible(false);
    tabAlarm->setVisible(false);
    tabRecord->setVisible(false);
    tabSystem->setVisible(false);
    toolbox->setCurrentIndex(index);
    switch (index) {
        case 0:
        {
            tabMedia->setVisible(true);
            boxMedia->setCurrentIndex(boxMedia->getModel()->index(tabMedia->currentIndex(), 0));
            //触发数据更新
            tabMedia->handleSwitchTab(boxMedia->getModel()->index(tabMedia->currentIndex(), 0));
            break;
        }
        case 1:
        {
            tabNetwork->setVisible(true);
            boxNetwork->setCurrentIndex(boxNetwork->getModel()->index(tabNetwork->currentIndex(), 0));
            tabNetwork->handleSwitchTab(boxNetwork->getModel()->index(tabNetwork->currentIndex(), 0));
            break;
        }
        case 2:
        {
            tabAlarm->setVisible(true);
            boxAlarm->setCurrentIndex(boxAlarm->getModel()->index(tabAlarm->currentIndex(), 0));
            tabAlarm->handleSwitchTab(boxAlarm->getModel()->index(tabAlarm->currentIndex(), 0));
            break;
        }
        case 3:
        {
            tabRecord->setVisible(true);
            boxRecord->setCurrentIndex(boxRecord->getModel()->index(tabRecord->currentIndex(), 0));
            tabRecord->handleSwitchTab(boxRecord->getModel()->index(tabRecord->currentIndex(), 0));
            break;
        }
        case 4:
        {
            tabSystem->setVisible(true);
            boxSystem->setCurrentIndex(boxSystem->getModel()->index(tabSystem->currentIndex(), 0));
            tabSystem->handleSwitchTab(boxSystem->getModel()->index(tabSystem->currentIndex(), 0));
            break;
        }
        default:
            break;
    }
}
