#include "settingswidget.h"
#include "ui/ui_settingsform.h"
#include "waitingshade.h"
#include <QDebug>
#include <QVBoxLayout>
#include "statustip.h"

SettinsWidget::SettinsWidget(QWidget *parent) :
    BaseWidget(parent),
    m_ui(new Ui::SettingsForm)
{
    m_ui->setupUi(this);

    QVBoxLayout *vLayout = new QVBoxLayout(m_ui->sideBar);
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
    m_ui->displayArea->setStyleSheet("background-color:white");

    initSideBar();
    initDisplayArea();

    connect(m_toolbox,    &QToolBox::currentChanged, this, &SettinsWidget::refresh);

    connect(m_listMedia,   &ListView::clicked, m_tabMedia,    &MediaWidget::setCurrentIndex);
    connect(m_listNetwork, &ListView::clicked, m_tabNetwork,  &NetworkWidget::setCurrentIndex);
    connect(m_listAlarm,   &ListView::clicked, m_tabAlarm,    &AlarmWidget::setCurrentIndex);
    connect(m_listRecord,  &ListView::clicked, m_tabRecord,   &RecordWidget::setCurrentIndex);
    connect(m_listSystem,  &ListView::clicked, m_tabSystem,   &SystemWidget::setCurrentIndex);

    WaitingShade::getInstance(this);
}

SettinsWidget::~SettinsWidget()
{

}

void SettinsWidget::initSideBar()
{
    QStringList list;

    m_listMedia = new ListView(m_toolbox);
    m_listMedia->setFrameShape(QFrame::NoFrame);
    list << "音视频" << "隐私遮蔽" << "图像参数" /*<< "ROI"*/ << "OSD";
    m_listMedia->addItems(list);
    m_toolbox->addItem(m_listMedia, QIcon(":/images/media.png"), "媒体设置");

    list.clear();
    m_listNetwork = new ListView(m_toolbox);
    list << "TCP/IP" /*<< "PPPOE" << "DDNS Client" << "Email" << "FTP"
         << "Bonjour" << "SNMP" << "UPNP" << "HTTPs" << "P2P" << "RTSP"*/;
    m_listNetwork->addItems(list);
    m_toolbox->addItem(m_listNetwork, QIcon(":/images/network.png"), "网络设置");

    list.clear();
    m_listAlarm = new ListView(m_toolbox);
    list << "移动侦测" << "遮挡报警" << "探头报警";
    m_listAlarm->addItems(list);
    m_toolbox->addItem(m_listAlarm, QIcon(":/images/alarm.png"), "报警设置");

    list.clear();
    m_listRecord = new ListView(m_toolbox);
    list << "远程录像计划" << "SD卡管理" << "抓拍设置" /*<< "Destination" << "NAS"*/;
    m_listRecord->addItems(list);
    m_toolbox->addItem(m_listRecord, QIcon(":/images/record.png"), "记录");

    list.clear();
    m_listSystem = new ListView(m_toolbox);
    list << "维护" << "设备信息" << "时间设置" << "账号管理" << "WIFI设置";
    m_listSystem->addItems(list);
    m_toolbox->addItem(m_listSystem, QIcon(":/images/system.png"), "系统设置");
}

void SettinsWidget::initDisplayArea()
{
    m_stackedLayout = new QStackedLayout(m_ui->displayArea);
    m_stackedLayout->setContentsMargins(0, 0, 0, 0);

    m_tabMedia = new MediaWidget(m_ui->displayArea);
    m_stackedLayout->addWidget(m_tabMedia);

    m_tabNetwork = new NetworkWidget(m_ui->displayArea);
    m_stackedLayout->addWidget(m_tabNetwork);

    m_tabAlarm = new AlarmWidget(m_ui->displayArea);
    m_stackedLayout->addWidget(m_tabAlarm);

    m_tabRecord = new RecordWidget(m_ui->displayArea);
    m_stackedLayout->addWidget(m_tabRecord);

    m_tabSystem = new SystemWidget(m_ui->displayArea);
    m_stackedLayout->addWidget(m_tabSystem);
}

void SettinsWidget::handleReceiveData(VidiconProtocol::Type type, QByteArray data)
{
    WaitingShade *w = WaitingShade::getInstance();
    StatusTip *s = StatusTip::getInstance();

    switch(type) {
    case VidiconProtocol::RESPONSESTATUS: {
        QString info = "参数设置失败";
        ResponseStatus reply;
        if(ParseXML::getInstance()->parseResponseStatus(&reply, data)) {
            if(reply.StatusCode == 1) {
                info = "参数设置成功";
            }
        }
        s->showStatusTip(info);
        w->hide();
        break;
    }
    default:
        break;
    }
}

void SettinsWidget::refresh(int index)
{
    m_toolbox->setCurrentIndex(index);
    m_stackedLayout->setCurrentIndex(index);

    switch (index) {
        case 0:
        {
            m_listMedia->setCurrentIndex(m_listMedia->model()->index(m_tabMedia->currentIndex(), 0));
            m_tabMedia->setCurrentIndex(m_listMedia->model()->index(m_tabMedia->currentIndex(), 0));
            break;
        }
        case 1:
        {
            m_listNetwork->setCurrentIndex(m_listNetwork->model()->index(m_tabNetwork->currentIndex(), 0));
            m_tabNetwork->setCurrentIndex(m_listNetwork->model()->index(m_tabNetwork->currentIndex(), 0));
            break;
        }
        case 2:
        {
            m_listAlarm->setCurrentIndex(m_listAlarm->model()->index(m_tabAlarm->currentIndex(), 0));
            m_tabAlarm->setCurrentIndex(m_listAlarm->model()->index(m_tabAlarm->currentIndex(), 0));
            break;
        }
        case 3:
        {
            m_listRecord->setCurrentIndex(m_listRecord->model()->index(m_tabRecord->currentIndex(), 0));
            m_tabRecord->setCurrentIndex(m_listRecord->model()->index(m_tabRecord->currentIndex(), 0));
            break;
        }
        case 4:
        {
            m_listSystem->setCurrentIndex(m_listSystem->model()->index(m_tabSystem->currentIndex(), 0));
            m_tabSystem->setCurrentIndex(m_listSystem->model()->index(m_tabSystem->currentIndex(), 0));
            break;
        }
        default:
            break;
    }
}
