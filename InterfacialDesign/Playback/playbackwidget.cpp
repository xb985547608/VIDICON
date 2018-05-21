#include "playbackwidget.h"
#include "ui/ui_playbackform.h"
#include "Protocol/vidiconprotocol.h"
#include "control/vlccontrol.h"
#include <QVBoxLayout>
#include <QPushButton>
#include "soundeffect.h"
#include "parsexml.h"
#include "statustip.h"

PlaybackWidget::PlaybackWidget(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::PlaybackForm),
    m_stateValue(0),
    m_isPlaying(false)
{
    qsrand(QTime::currentTime().msec());
    m_htmlid = qrand();

    ui->setupUi(this);
    ui->playBtn->setProperty("State", "pause");

    connect(this, &PlaybackWidget::signalVlcControl, VlcControl::getInstance(), &VlcControl::handleVlcControl);

    m_fileDialog = new FileManagerDialog(this);
    connect(m_fileDialog, &FileManagerDialog::signalAddDownloadTask, this, [this](QStringList files){
        emit signalAddDownloadTask(files);
    });

    m_dateWidget = new DateWidget(ui->rightBar);
    QVBoxLayout *layout1 = new QVBoxLayout(ui->rightBar);
    layout1->addWidget(m_dateWidget);
    layout1->setContentsMargins(0, 0, 0, 0);

    m_timerShaft = new TimerShaft(m_htmlid, ui->timeslider);
    connect(m_dateWidget, &DateWidget::signalDateChange, m_timerShaft, &TimerShaft::hanlderDateChange);
    QVBoxLayout *layout2 = new QVBoxLayout(ui->timeslider);
    layout2->addWidget(m_timerShaft);
    layout2->setContentsMargins(0, 0, 0, 0);

    connect(ui->playBtn, &QPushButton::clicked, this, &PlaybackWidget::onPlayBtnClicked);
    connect(ui->fastPlayBtn, &QPushButton::clicked, this, &PlaybackWidget::onFastPlayBtnClicked);
    connect(ui->slowForwardBtn, &QPushButton::clicked, this, &PlaybackWidget::onSlowForwardBtnClicked);
    connect(ui->snapShotBtn, &QPushButton::clicked, this, &PlaybackWidget::onSnapshotBtnClicked);
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
    if (value == m_stateValue || value > 2 || value < -2)
        return;
    m_stateValue = value;
    PlayStateParameter param;
    param.htmlid = m_htmlid;
    param.StateValue = m_stateValue;
    emit signalSetParameter(VidiconProtocol::PLAYSTATE, QVariant::fromValue(param));
}

void PlaybackWidget::refresh()
{
    if (!isVisible()) {
        m_isPlaying = false;
        checkPlayState();
        emit signalVlcControl(VLCCONTROLSTOP);
    } else {
        m_dateWidget->refresh();
        emit signalVlcControl(VLCCONTROLINIT, BACKUPSTREAMTYPE, ui->displayArea->winId());
    }
}

void PlaybackWidget::handleReceiveData(VidiconProtocol::Type type, QByteArray data)
{
    StatusTip *s = StatusTip::getInstance();
    bool isOK = false;

    switch(type) {
    case VidiconProtocol::STARTPLAYING: {
        ResponseStatus reply;
        isOK = ParseXML::getInstance()->parseResponseStatus(&reply, data);
        if (isOK) {
            QString info;
            if(reply.StatusCode == 1) {
                info = "(*^▽^*)  开始回放";
                m_isPlaying = true;
            }else {
                m_isPlaying = false;
                info = "(╯﹏╰)  回放失败";
            }
            checkPlayState();
            s->showStatusTip(info);
        }
        break;
    }
    case VidiconProtocol::CURRENTPLAYINGTIME: {
        PlayingTimeParameter param;
        isOK = ParseXML::getInstance()->parsePlayingTimeParameter(&param, data);
        if(isOK) {
            if (param.htmlid == m_htmlid) {
                if (m_isPlaying != (param.Playend == 0)) {
                    m_isPlaying = param.Playend != 0;
                    checkPlayState();
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
    m_isPlaying = !m_isPlaying;


    checkPlayState();
}

void PlaybackWidget::checkPlayState()
{
    ui->playBtn->setProperty("State", m_isPlaying ? "play" : "pause");
    refreshPolish(ui->playBtn);

    if (m_isPlaying) {
        emit signalVlcControl(VLCCONTROLPLAY);
    } else {
        emit signalVlcControl(VLCCONTROLSTOP);
    }
}

void PlaybackWidget::onSlowForwardBtnClicked()
{
    setStateValue(m_stateValue + 1);
}

void PlaybackWidget::onFastPlayBtnClicked()
{
    setStateValue(m_stateValue - 1);
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
                                  Q_ARG(int, m_htmlid), Q_ARG(QString, "r00001"));
}
