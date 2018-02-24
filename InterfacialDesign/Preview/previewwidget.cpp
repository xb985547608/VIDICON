#include "previewwidget.h"
#include "ui/ui_previewform.h"
#include "Control/vlccontrol.h"
#include <QTimer>
#include "parsexml.h"

PreviewWidget::PreviewWidget(QWidget *parent) :
    QWidget(parent),
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
    ui->snapshot->setToolTip("抓拍");
    ui->voice->setToolTip("启用音频");
    ui->manualRecord->setToolTip("手动录制");
    ui->mainStream->setToolTip("主码流");
    ui->subStream->setToolTip("子码流");

    connect(ui->mainStream, &QRadioButton::toggled, this, &PreviewWidget::handlerWidgetSwitch);
    connect(ui->refresh, &QPushButton::clicked, this, &PreviewWidget::handlerWidgetSwitch);
    connect(this, &PreviewWidget::signalVlcControl, VlcControl::getInstance(), &VlcControl::handlerVlcControl);
    connect(VidiconProtocol::getInstance(), &VidiconProtocol::signalSendData, this, &PreviewWidget::handlerReceiveData);
    connect(this, &PreviewWidget::signalGetParameter, VidiconProtocol::getInstance(), &VidiconProtocol::handlerGetParameter);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &PreviewWidget::handlerTimeout);
    timer->start(1000);

    ui->mainStream->setChecked(true);
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

void PreviewWidget::handlerWidgetSwitch()
{
    if(isVisible()){
        handlerStreamSwitch(ui->mainStream->isChecked());
    }else{
        emit signalVlcControl(VLCCONTROLSTOP);
    }
}

void PreviewWidget::handlerStreamSwitch(bool checked)
{
    if(checked){
        emit signalVlcControl(VLCCONTROLINIT, MAINSTREAMTYPE, ui->displayArea->winId());
    }else{
        emit signalVlcControl(VLCCONTROLINIT, SUBSTREAMTYPE, ui->displayArea->winId());
    }
    replySuccess = true;
}

void PreviewWidget::handlerTimeout()
{
    if(isVisible()) {
        if(replySuccess || timeoutSec > 4) {
            emit signalGetParameter(PULLMESSAGE);
            replySuccess = false;
            timeoutSec = 0;
        }else {
            timeoutSec++;
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
    }else {
        replySuccess = true;
    }
}

void PreviewWidget::handlerReceiveData(int type, QByteArray data)
{
    switch(type) {
    case PULLMESSAGE: {        
        replySuccess = true;
        if(ParseXML::getInstance()->parsePullMsg(&param, data)) {
            motionAlarmFlicker = false;
            sensorAlarmFlicker = false;
            blindAlarmFlicker = false;
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
        qDebug() << "#PreviewWidget# handlerReceiveData, ParameterType:" << type << "parse data success...";
        break;
    }
    default:
        break;
    }
}
