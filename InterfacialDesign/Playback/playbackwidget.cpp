#include "playbackwidget.h"
#include "ui/ui_playbackform.h"
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
    ui->playBtn->setProperty("State", "pause");

    connect(this, &PlaybackWidget::signalSetParameter, VidiconProtocol::getInstance(), &VidiconProtocol::handlerSetParameter);
    connect(this, &PlaybackWidget::signalGetParameter, VidiconProtocol::getInstance(), &VidiconProtocol::handlerGetParameter);
    connect(VidiconProtocol::getInstance(), &VidiconProtocol::signalSendData, this, &PlaybackWidget::handlerReceiveData);
    connect(this, &PlaybackWidget::signalVlcControl, VlcControl::getInstance(), &VlcControl::handlerVlcControl);

    fileDialog = new FileManagerDialog(this);
    connect(fileDialog, &FileManagerDialog::signalAddDownloadTask, this, [this](QStringList files){
        emit signalAddDownloadTask(files);
    });

    ui->rightBar->setStyleSheet("QWidget#rightBar{background-color:darkgray}");
    dateWidget = new DateWidget(ui->rightBar);

    TimerShaft *view = new TimerShaft(htmlid, ui->timeslider);
    connect(dateWidget, &DateWidget::signalDateChange, view, &TimerShaft::hanlderDateChange);
    QVBoxLayout *layout2 = new QVBoxLayout(ui->timeslider);
    layout2->addWidget(view);
    layout2->setContentsMargins(0, 0, 0, 0);

    connect(ui->playBtn, &QPushButton::clicked, this, &PlaybackWidget::onPlayBtnClicked);
}

PlaybackWidget::~PlaybackWidget()
{

}

void PlaybackWidget::refreshPolish(QWidget *w)
{
    w->style()->unpolish(w);
    w->style()->polish(w);
    update();
}

void PlaybackWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    dateWidget->setGeometry(5, 5, ui->rightBar->size().width() - 10, ui->rightBar->size().height() - 5);
}

void PlaybackWidget::handlerWidgetSwitch()
{
    if(!isVisible()) {
        emit signalVlcControl(VLCCONTROLSTOP);
    }
}

void PlaybackWidget::handlerReceiveData(int type, QByteArray data)
{
    switch(type) {
    case PLAYSTATE: {
        qDebug() << data;

        break;
    }
    case STARTPLAYING: {
        emit signalVlcControl(VLCCONTROLINIT, BACKUPSTREAMTYPE, ui->displayArea->winId());
        break;
    }
    }
}

void PlaybackWidget::onPlayBtnClicked()
{
    QString value = ui->playBtn->property("State").toString();
    ui->playBtn->setProperty("State", value.compare("pause") ? "pause" : "play");
    refreshPolish(ui->playBtn);
}
