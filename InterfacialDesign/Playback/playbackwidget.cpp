#include "playbackwidget.h"
#include "ui_playbackform.h"
#include "Protocol/vidiconprotocol.h"
#include "control/vlccontrol.h"

#include <QVBoxLayout>
#include <QPushButton>

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

    ui->rightBar->setStyleSheet("QWidget#rightBar{background-color:darkgray}");
    fileDialog = new FileManagerDialog(this);
    dateWidget = new DateWidget(ui->rightBar);
    QPushButton *btn = new QPushButton(ui->rightBar);
    btn->setFixedSize(80, 25);
    btn->setStyleSheet("QPushButton{border-image:url(:images/query.png)0 80 0 0}"
                       "QPushButton:pressed{border-image:url(:images/query.png)0 0 0 80}");
    connect(btn, &QPushButton::clicked, this, [this](){
        VidiconProtocol::BackUpQueryParameter *param = new VidiconProtocol::BackUpQueryParameter;
        param->Date = dateWidget->getDate();
        param->Type = 1;
        emit signalSetParameter(BACKQUERY, param);
    });

    QVBoxLayout *layout1 = new QVBoxLayout(ui->rightBar);
    layout1->setContentsMargins(0, 0, 0, 0);
    layout1->addWidget(dateWidget, 0, Qt::AlignCenter);
    layout1->addStretch();
    layout1->addWidget(btn, 0, Qt::AlignCenter);

    QPushButton *btn1 = new QPushButton(ui->timeslider);
    connect(btn1, &QPushButton::clicked, this, [this](){
        VidiconProtocol::PlayStateParameter *param1 = new VidiconProtocol::PlayStateParameter;
        param1->htmlid = htmlid;
        param1->StateValue = 0;
        emit signalSetParameter(PLAYSTATE, param1);

        VidiconProtocol::StartPlayingParameter *param = new VidiconProtocol::StartPlayingParameter;
        param->htmlid = htmlid;
        param->playing = 1;
        param->Time = QDateTime(QDate(2018, 1, 22), QTime(21, 43, 00));
        emit signalSetParameter(STARTPLAYING, param);
    });
}

PlaybackWidget::~PlaybackWidget()
{

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

    }
}
