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
    m_toolbox = new QToolBox(this);
    m_toolbox->setStyleSheet("QToolBox::tab {\
                               border-image:url(:/images/st_background.png) 0 220 0 0;\
                               color: white;font-size:14px;\
                           }\
                           QToolBox::tab:selected { /* italicize selected tabs */\
                               border-image:url(:/images/st_background.png) 0 0 0 220;font-weight:bold;\
                           }\
                           QToolBoxButton{min-height:30px;margin:1px}");
    m_toolbox->layout()->setSpacing(0);
    vLayout->addWidget(m_toolbox);
    ui->displayArea->setStyleSheet("background-color:white");

    initSideBar();
    initDisplayArea();

    connect(m_toolbox,    &QToolBox::currentChanged, this, &SettinsWidget::refresh);

    connect(m_boxMedia,   &ListView::clicked, m_tabMedia,    &MediaWidget::setCurrentIndex);
    connect(m_boxNetwork, &ListView::clicked, m_tabNetwork,  &NetworkWidget::setCurrentIndex);
    connect(m_boxAlarm,   &ListView::clicked, m_tabAlarm,    &AlarmWidget::setCurrentIndex);
    connect(m_boxRecord,  &ListView::clicked, m_tabRecord,   &RecordWidget::setCurrentIndex);
    connect(m_boxSystem,  &ListView::clicked, m_tabSystem,   &SystemWidget::setCurrentIndex);
}

SettinsWidget::~SettinsWidget()
{

}

void SettinsWidget::initSideBar()
{
    QStringList list;

    m_boxMedia = new ListView(m_toolbox);
    m_boxMedia->setFrameShape(QFrame::NoFrame);
    list << "音视频" << "隐私遮蔽" << "图像参数" << "ROI" << "OSD";
    m_boxMedia->addItems(list);
    m_toolbox->addItem(m_boxMedia, QIcon(":/images/media.png"), "媒体设置");

    list.clear();
    m_boxNetwork = new ListView(m_toolbox);
    list << "TCP/IP" << "PPPOE" << "DDNS Client" << "Email" << "FTP"
         << "Bonjour" << "SNMP" << "UPNP" << "HTTPs" << "P2P" << "RTSP";
    m_boxNetwork->addItems(list);
    m_toolbox->addItem(m_boxNetwork, QIcon(":/images/network.png"), "网络设置");

    list.clear();
    m_boxAlarm = new ListView(m_toolbox);
    list << "移动侦测" << "遮挡报警" << "探头报警";
    m_boxAlarm->addItems(list);
    m_toolbox->addItem(m_boxAlarm, QIcon(":/images/alarm.png"), "报警设置");

    list.clear();
    m_boxRecord = new ListView(m_toolbox);
    list << "远程录像计划" << "SD卡管理" << "抓拍设置" /*<< "Destination" << "NAS"*/;
    m_boxRecord->addItems(list);
    m_toolbox->addItem(m_boxRecord, QIcon(":/images/record.png"), "记录");

    list.clear();
    m_boxSystem = new ListView(m_toolbox);
    list << "维护" << "设备信息" << "时间设置" << "账号管理" << "WIFI设置";
    m_boxSystem->addItems(list);
    m_toolbox->addItem(m_boxSystem, QIcon(":/images/system.png"), "系统设置");
}

void SettinsWidget::initDisplayArea()
{
    m_stackedLayout = new QStackedLayout(ui->displayArea);
    m_stackedLayout->setContentsMargins(0, 0, 0, 0);

    m_tabMedia = new MediaWidget(ui->displayArea);
    m_stackedLayout->addWidget(m_tabMedia);

    m_tabNetwork = new NetworkWidget(ui->displayArea);
    m_stackedLayout->addWidget(m_tabNetwork);

    m_tabAlarm = new AlarmWidget(ui->displayArea);
    m_stackedLayout->addWidget(m_tabAlarm);

    m_tabRecord = new RecordWidget(ui->displayArea);
    m_stackedLayout->addWidget(m_tabRecord);

    m_tabSystem = new SystemWidget(ui->displayArea);
    m_stackedLayout->addWidget(m_tabSystem);
}

void SettinsWidget::refresh(int index)
{
    m_toolbox->setCurrentIndex(index);
    m_stackedLayout->setCurrentIndex(index);

    switch (index) {
        case 0:
        {
            m_boxMedia->setCurrentIndex(m_boxMedia->model()->index(m_tabMedia->currentIndex(), 0));
            //触发数据更新
            m_tabMedia->setCurrentIndex(m_boxMedia->model()->index(m_tabMedia->currentIndex(), 0));
            break;
        }
        case 1:
        {
            m_boxNetwork->setCurrentIndex(m_boxNetwork->model()->index(m_tabNetwork->currentIndex(), 0));
            m_tabNetwork->setCurrentIndex(m_boxNetwork->model()->index(m_tabNetwork->currentIndex(), 0));
            break;
        }
        case 2:
        {
            m_boxAlarm->setCurrentIndex(m_boxAlarm->model()->index(m_tabAlarm->currentIndex(), 0));
            m_tabAlarm->setCurrentIndex(m_boxAlarm->model()->index(m_tabAlarm->currentIndex(), 0));
            break;
        }
        case 3:
        {
            m_boxRecord->setCurrentIndex(m_boxRecord->model()->index(m_tabRecord->currentIndex(), 0));
            m_tabRecord->setCurrentIndex(m_boxRecord->model()->index(m_tabRecord->currentIndex(), 0));
            break;
        }
        case 4:
        {
            m_boxSystem->setCurrentIndex(m_boxSystem->model()->index(m_tabSystem->currentIndex(), 0));
            m_tabSystem->setCurrentIndex(m_boxSystem->model()->index(m_tabSystem->currentIndex(), 0));
            break;
        }
        default:
            break;
    }
}
