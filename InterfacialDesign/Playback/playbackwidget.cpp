#include "playbackwidget.h"
#include "ui/ui_playbackform.h"
#include "Protocol/vidiconprotocol.h"
#include "control/vlccontrol.h"
#include <QVBoxLayout>
#include <QPushButton>
#include "timershaft.h"
#include "soundeffect.h"
#include "parsexml.h"
#include "statustip.h"

PlaybackWidget::PlaybackWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlaybackForm),
    stateValue(0),
    isPlaying(false)
{
    qsrand(QTime::currentTime().msec());
    htmlid = qrand();

    ui->setupUi(this);
    ui->playBtn->setProperty("State", "pause");

    connect(this, &PlaybackWidget::signalSetParameter, VidiconProtocol::getInstance(), &VidiconProtocol::handleSetParameter);
    connect(this, &PlaybackWidget::signalGetParameter, VidiconProtocol::getInstance(), &VidiconProtocol::handleGetParameter);
    connect(VidiconProtocol::getInstance(), &VidiconProtocol::signalReceiveData, this, &PlaybackWidget::handleReceiveData);
    connect(this, &PlaybackWidget::signalVlcControl, VlcControl::getInstance(), &VlcControl::handleVlcControl);

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

    snapshotSoundEffect = new QSound(":/snapshot.wav");
    snapshotSoundEffect->setLoops(1);

    connect(ui->playBtn, &QPushButton::clicked, this, &PlaybackWidget::onPlayBtnClicked);
    connect(ui->fastPlayBtn, &QPushButton::clicked, this, &PlaybackWidget::onFastPlayBtnClicked);
    connect(ui->slowForwardBtn, &QPushButton::clicked, this, &PlaybackWidget::onSlowForwardBtnClicked);
    connect(ui->snapShotBtn, &QPushButton::clicked, this, &PlaybackWidget::onSnapshotBtnClicked);

//    QTimer *timer = new QTimer(this);
//    connect(timer, &QTimer::timeout, this, &PlaybackWidget::handleTimeout);
//    timer->start(1000);
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

void PlaybackWidget::setStateValue(int value)
{
    if (value == stateValue || value > 2 || value < -2)
        return;
    stateValue = value;
    VidiconProtocol::PlayStateParameter *param = new VidiconProtocol::PlayStateParameter;
    param->htmlid = htmlid;
    param->StateValue = stateValue;
    emit signalSetParameter(PLAYSTATE, param);
}

void PlaybackWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    dateWidget->setGeometry(5, 5, ui->rightBar->size().width() - 10, ui->rightBar->size().height() - 5);
}

void PlaybackWidget::handleWidgetSwitch()
{
    if(!isVisible()) {
        isPlaying = true;
        onPlayBtnClicked();
        emit signalVlcControl(VLCCONTROLSTOP);
    }
}

void PlaybackWidget::handleReceiveData(int type, QByteArray data)
{
    Q_UNUSED(data);
    StatusTip *s = StatusTip::getInstance();
    bool isOK = false;

    switch(type) {
    case STARTPLAYING: {
        VidiconProtocol::ResponseStatus reply;
        isOK = ParseXML::getInstance()->parseResponseStatus(&reply, data);
        if(isOK) {
            QString info;
            if(reply.StatusCode == 1) {
                info = "(*^▽^*)  开始回放";
                isPlaying = false;
            }else {
                isPlaying = true;
                info = "(╯﹏╰)  回放失败";
            }
            onPlayBtnClicked();
            s->showStatusTip(info);
        }
        break;
    }
    case CURRENTPLAYINGTIME: {
        VidiconProtocol::PlayingTimeParameter param;
        isOK = ParseXML::getInstance()->parsePlayingTimeParameter(&param, data);
        if(isOK) {
            if (param.htmlid == htmlid) {
                if (isPlaying != (param.Playend == 0)) {
                    isPlaying = param.Playend == 0;
                    onPlayBtnClicked();
                }
            }
        }
        break;
    }
    default:
        return;
    }

    if (isOK)
        qDebug() << "#PlaybackWidget# handleReceiveData, ParameterType:" << type << "parse data success...";
    else
        qDebug() << "#PlaybackWidget# handleReceiveData, ParameterType:" << type << "parse data error...";
}

void PlaybackWidget::onPlayBtnClicked()
{
    isPlaying = !isPlaying;
    ui->playBtn->setProperty("State", isPlaying ? "play" : "pause");
    refreshPolish(ui->playBtn);

    if (isPlaying) {
        emit signalVlcControl(VLCCONTROLINIT, BACKUPSTREAMTYPE, ui->displayArea->winId());
    } else {
        emit signalVlcControl(VLCCONTROLSTOP);
    }
}

void PlaybackWidget::onSlowForwardBtnClicked()
{
    setStateValue(stateValue + 1);
}

void PlaybackWidget::onFastPlayBtnClicked()
{
    setStateValue(stateValue - 1);
}

void PlaybackWidget::onSnapshotBtnClicked()
{
    if (SoundEffect::getInstance()->getSoundEffect(SoundEffect::Snapshot)->isFinished())
        SoundEffect::getInstance()->triggerSoundEffect(SoundEffect::Snapshot);
}

void PlaybackWidget::onVolumeBtnClicked()
{

}

void PlaybackWidget::handleTimeout()
{
    if (isVisible())
        QMetaObject::invokeMethod(VidiconProtocol::getInstance(), "getCurrentPlayingTime", Qt::QueuedConnection,
                                  Q_ARG(int, htmlid), Q_ARG(QString, "r00001"));
}
