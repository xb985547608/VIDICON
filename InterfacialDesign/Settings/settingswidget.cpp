#include "settingswidget.h"
#include "ui_settingsform.h"
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
                               background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\
                                                           stop: 0 #E1E1E1, stop: 0.4 #DDDDDD,\
                                                           stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3);\
                               border-radius: 5px;\
                               color: darkgray;\
                           }\
                           QToolBox::tab:selected { /* italicize selected tabs */\
                               font: italic bold ;\
                               border-bottom:2px solid #ffa02f;\
                               color: black;\
                           }");
    vLayout->addWidget(toolbox);



    initSideBar();
    initDisplayArea();

    connect(toolbox, &QToolBox::currentChanged, this, &SettinsWidget::handlerToolBoxClicked);

    connect(boxMedia, &BoxView::clicked, tabMedia, &TabMedia::handlerSwitchTab);
    connect(tabMedia, &TabMedia::currentChanged, boxMedia, &BoxView::hanlderSwitchRow);

    connect(boxNetwork, &BoxView::clicked, tabNetwork, &TabNetwork::handlerSwitchTab);
    connect(tabNetwork, &TabNetwork::currentChanged, boxNetwork, &BoxView::hanlderSwitchRow);

    connect(boxAlarm, &BoxView::clicked, tabAlarm, &TabAlarm::handlerSwitchTab);
    connect(tabAlarm, &TabAlarm::currentChanged, boxAlarm, &BoxView::hanlderSwitchRow);

    connect(boxRecord, &BoxView::clicked, tabRecord, &TabRecord::handlerSwitchTab);
    connect(tabRecord, &TabRecord::currentChanged, boxRecord, &BoxView::hanlderSwitchRow);

    connect(boxSystem, &BoxView::clicked, tabSystem, &TabSystem::handlerSwitchTab);
    connect(tabSystem, &TabSystem::currentChanged, boxSystem, &BoxView::hanlderSwitchRow);
}

SettinsWidget::~SettinsWidget()
{

}

void SettinsWidget::initSideBar()
{
    QStringList list;

    boxMedia = new BoxView(toolbox);
    boxMedia->setFrameShape(QFrame::NoFrame);
    list << "Audio Video" << "Privacy" << "Image" << "ROI" << "OSD";
    boxMedia->setData(list);
    toolbox->addItem(boxMedia, QIcon(":/images/media.png"), "Media Config");

    list.clear();
    boxNetwork = new BoxView(toolbox);
    list << "TCP/IP" << "PPPOE" << "DDNS Client" << "Email" << "FTP"
         << "Bonjour" << "SNMP" << "UPNP" << "HTTPs" << "P2P" << "RTSP";
    boxNetwork->setData(list);
    toolbox->addItem(boxNetwork, QIcon(":/images/network.png"), "Network Config");

    list.clear();
    boxAlarm = new BoxView(toolbox);
    list << "Motion Detection" << "Video Blind" << "Alarm";
    boxAlarm->setData(list);
    toolbox->addItem(boxAlarm, QIcon(":/images/alarm.png"), "Alarm Config");

    list.clear();
    boxRecord = new BoxView(toolbox);
    list << "Schedule" << "SD Storage" << "Snapshot" /*<< "Destination" << "NAS"*/;
    boxRecord->setData(list);
    toolbox->addItem(boxRecord, QIcon(":/images/record.png"), "Record");

    list.clear();
    boxSystem = new BoxView(toolbox);
    list << "Maintenance" << "Device Info" << "Set Time" << "User Admin";
    boxSystem->setData(list);
    toolbox->addItem(boxSystem, QIcon(":/images/system.png"), "System Config");
}

void SettinsWidget::initDisplayArea()
{
    QVBoxLayout *layout = new QVBoxLayout(ui->displayArea);
    layout->setContentsMargins(0, 0, 0, 0);

    tabMedia = new TabMedia(ui->displayArea);
    tabMedia->setVisible(true);
    layout->addWidget(tabMedia);

    tabNetwork = new TabNetwork(ui->displayArea);
    tabNetwork->setVisible(false);
    layout->addWidget(tabNetwork);

    tabAlarm = new TabAlarm(ui->displayArea);
    tabAlarm->setVisible(false);
    layout->addWidget(tabAlarm);

    tabRecord = new TabRecord(ui->displayArea);
    tabRecord->setVisible(false);
    layout->addWidget(tabRecord);

    tabSystem = new TabSystem(ui->displayArea);
    tabSystem->setVisible(false);
    layout->addWidget(tabSystem);
}

void SettinsWidget::handlerToolBoxClicked(int index)
{
    tabMedia->setVisible(false);
    tabNetwork->setVisible(false);
    tabAlarm->setVisible(false);
    tabRecord->setVisible(false);
    tabSystem->setVisible(false);
    switch (index) {
        case 0:
        {
            tabMedia->setVisible(true);
            boxMedia->setCurrentIndex(boxMedia->getModel()->index(tabMedia->currentIndex(), 0));
            //触发数据更新
            tabMedia->handlerSwitchTab(boxMedia->getModel()->index(tabMedia->currentIndex(), 0));
            break;
        }
        case 1:
        {
            tabNetwork->setVisible(true);
            boxNetwork->setCurrentIndex(boxNetwork->getModel()->index(tabNetwork->currentIndex(), 0));
            tabNetwork->handlerSwitchTab(boxNetwork->getModel()->index(tabNetwork->currentIndex(), 0));
            break;
        }
        case 2:
        {
            tabAlarm->setVisible(true);
            boxAlarm->setCurrentIndex(boxAlarm->getModel()->index(tabAlarm->currentIndex(), 0));
            tabAlarm->handlerSwitchTab(boxAlarm->getModel()->index(tabAlarm->currentIndex(), 0));
            break;
        }
        case 3:
        {
            tabRecord->setVisible(true);
            boxRecord->setCurrentIndex(boxRecord->getModel()->index(tabRecord->currentIndex(), 0));
            tabRecord->handlerSwitchTab(boxRecord->getModel()->index(tabRecord->currentIndex(), 0));
            break;
        }
        case 4:
        {
            tabSystem->setVisible(true);
            boxSystem->setCurrentIndex(boxSystem->getModel()->index(tabSystem->currentIndex(), 0));
            tabSystem->handlerSwitchTab(boxSystem->getModel()->index(tabSystem->currentIndex(), 0));
            break;
        }
        default:
            break;
    }
}
