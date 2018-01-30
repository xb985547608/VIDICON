#include "playbackwidget.h"
#include "ui_playbackform.h"
#include "Protocol/vidiconprotocol.h"
#include "control/vlccontrol.h"
#include <QVBoxLayout>
#include <QPushButton>
#include "timershaft.h"

PlaybackWidget::PlaybackWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlaybackForm)
{
    qsrand(QTime::currentTime().msec());
    htmlid = qrand();
    ui->setupUi(this);
    connect(this, &PlaybackWidget::signalSetParameter, VidiconProtocol::getInstance(), &VidiconProtocol::handlerSetParameter);
    connect(this, &PlaybackWidget::signalGetParameter, VidiconProtocol::getInstance(), &VidiconProtocol::handlerGetParameter);
    connect(VidiconProtocol::getInstance(), &VidiconProtocol::signalSendData, this, &PlaybackWidget::handlerReceiveData);
    connect(this, &PlaybackWidget::signalVlcControl, VlcControl::getInstance(), &VlcControl::handlerVlcControl);

    fileDialog = new FileManagerDialog(this);

    ui->rightBar->setStyleSheet("QWidget#rightBar{background-color:darkgray}");
    dateWidget = new DateWidget(ui->rightBar);

    TimerShaft *view = new TimerShaft(ui->timeslider);
    QVBoxLayout *layout2 = new QVBoxLayout(ui->timeslider);
    layout2->addWidget(view);
    layout2->setContentsMargins(0, 0, 0, 0);
}

PlaybackWidget::~PlaybackWidget()
{

}

void PlaybackWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    dateWidget->setGeometry(5, 5, ui->rightBar->size().width() - 10, ui->rightBar->size().height() - 5);
}

void PlaybackWidget::handlerWidgetSwitch()
{
//    VlcControl::getInstance()->init("rtsp://admin:admin@192.168.0.66/H264?channel=0&subtype=3&unicast=true&proto=Onvif", this->winId());
//    if(isVisible()){
//        VlcControl::getInstance()->play();
//    }else{
//        VlcControl::getInstance()->stop();
    //    }
}

void PlaybackWidget::handlerReceiveData(int type, QByteArray data)
{
    switch(type) {
    case PLAYSTATE: {
        qDebug() << data;
//        emit signalVlcControl(VLCCONTROLINIT, "rtsp://admin:admin@192.168.0.66/H264?channel=0&subtype=3&unicast=true&proto=Onvif", ui->displayArea->winId());
//        emit signalVlcControl(VLCCONTROLPLAY);

        break;
    }
    case STARTPLAYING: {
        qDebug() << data;
        break;
    }
    }
}
