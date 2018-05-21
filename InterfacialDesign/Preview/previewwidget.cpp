#include "previewwidget.h"
#include "ui/ui_previewform.h"
#include "Control/vlccontrol.h"
#include <QTimer>
#include "parsexml.h"
#include "soundeffect.h"

PreviewWidget::PreviewWidget(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::PreviewForm)
{
    ui->setupUi(this);

    param.AlarmCount = 0;
    param.msgs = msgs;

    ui->alarm->setToolTip("传感器报警");
    ui->alarm->setProperty("State", "disable");
    ui->motion->setToolTip("运动报警");
    ui->motion->setProperty("State", "disable");
    ui->blind->setToolTip("遮挡报警");
    ui->blind->setProperty("State", "disable");
    ui->autoRecord->setToolTip("定时录制");
    ui->autoRecord->setVisible(false);
    ui->snapshot->setToolTip("抓拍");
    ui->voice->setToolTip("启用音频");
    ui->voice->setVisible(false);
    ui->manualRecord->setToolTip("手动录制");
    ui->manualRecord->setVisible(false);
    ui->mainStream->setToolTip("主码流");
    ui->subStream->setToolTip("子码流");

    connect(ui->mainStream, &QRadioButton::toggled, this, &PreviewWidget::refresh);
    connect(ui->refresh, &QPushButton::clicked, this, &PreviewWidget::refresh);
    connect(ui->snapshot, &QPushButton::clicked, this, &PreviewWidget::onSnapshotBtn);

    connect(this, &PreviewWidget::signalVlcControl, VlcControl::getInstance(), &VlcControl::handleVlcControl);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &PreviewWidget::handleTimeout);
    timer->start(1000);

    ui->subStream->setChecked(true);
}

PreviewWidget::~PreviewWidget()
{

}

void PreviewWidget::updateDynamicProperty(QWidget *w)
{
    w->style()->unpolish(w);
    w->style()->polish(w);
    w->update();
}

//每次导航栏切换界面时触发该函数，根据界面的可见性决定VLC的状态
void PreviewWidget::refresh()
{
    if(isVisible()){
        handleStreamSwitch(ui->mainStream->isChecked());
    }else{
        emit signalVlcControl(VLCCONTROLSTOP);
    }
}

void PreviewWidget::handleStreamSwitch(bool checked)
{
    if(checked){
        emit signalVlcControl(VLCCONTROLINIT, MAINSTREAMTYPE, ui->displayArea->winId());
    }else{
        emit signalVlcControl(VLCCONTROLINIT, SUBSTREAMTYPE, ui->displayArea->winId());
    }
    replySuccess = true;
}

void PreviewWidget::handleTimeout()
{
    if(isVisible()) {
        if (replySuccess) {
            emit signalGetParameter(VidiconProtocol::PULLMESSAGE);
            replySuccess = false;
        }

        //处理标志闪烁情况
        if(motionAlarmFlicker) {
            ui->motion->setProperty("State", ui->motion->property("State").toString() == "flicker" ? "enable" : "flicker");
        }else {
            ui->motion->setProperty("State", param.MotionAlarmEnabled ? "enable" : "disable");
        }
        if(sensorAlarmFlicker) {
            ui->alarm->setProperty("State", ui->alarm->property("State").toString() == "flicker" ? "enable" : "flicker");
        }else {
            ui->alarm->setProperty("State", param.SensorAlarmEnabled ? "enable" : "disable");
        }
        if(blindAlarmFlicker) {
            ui->blind->setProperty("State", ui->blind->property("State").toString() == "flicker" ? "enable" : "flicker");
        }else {
            ui->blind->setProperty("State", param.VideoBlindAlarmEnabled ? "enable" : "disable");
        }
        updateDynamicProperty(ui->alarm);
        updateDynamicProperty(ui->motion);
        updateDynamicProperty(ui->blind);

        emit signalVlcControl(VLCCONTROLPLAY);
    }
}

void PreviewWidget::handleReceiveData(VidiconProtocol::Type type, QByteArray data)
{
    bool isOK = false;

    switch(type) {
    case VidiconProtocol::PULLMESSAGE: {
        replySuccess = true;
        isOK = ParseXML::getInstance()->parsePullMsg(&param, data);
        if (isOK) {
            motionAlarmFlicker = false;
            sensorAlarmFlicker = false;
            blindAlarmFlicker = false;
            //根据摄像机的应答数据来确定报警是否闪烁
            for(int i=0; i<param.AlarmCount; i++) {
                if(param.msgs[i].AlarmType.compare("MotionAlarm", Qt::CaseInsensitive) == 0) {
                    motionAlarmFlicker = true;
                }else if(param.msgs[i].AlarmType.compare("SensorAlarm", Qt::CaseInsensitive) == 0) {
                    sensorAlarmFlicker = true;
                }else if(param.msgs[i].AlarmType.compare("VideoBlindAlarm", Qt::CaseInsensitive) == 0) {
                    blindAlarmFlicker = true;
                }
            }
        }
        break;
    }
    default:
        return;
    }

    if (isOK)
        qDebug() << "#PreviewWidget# handleReceiveData, ParameterType:" << type << "parse data success...";
    else
        qDebug() << "#PreviewWidget# handleReceiveData, ParameterType:" << type << "parse data error...";
}

void PreviewWidget::onSnapshotBtn()
{
    if (SoundEffect::getInstance()->getSoundEffect(SoundEffect::Snapshot)->isFinished())
        SoundEffect::getInstance()->triggerSoundEffect(SoundEffect::Snapshot);
}
