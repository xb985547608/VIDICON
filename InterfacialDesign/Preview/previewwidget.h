#ifndef PREVIEWWIDGET_H
#define PREVIEWWIDGET_H

#include <QWidget>
#include "Protocol/vidiconprotocol.h"

namespace Ui{
class PreviewForm;
}

class PreviewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PreviewWidget(QWidget *parent = nullptr);
    ~PreviewWidget();

    void updateDynamicProperty(QWidget *w);

signals:
    void signalSetParameter(int type, void *param = NULL, QString SessionID = "R00001");
    void signalGetParameter(int type, void *param = NULL, QString SessionID = "R00001");
    void signalVlcControl(int type, int subtype = -1, WId id = 0);

public slots:
    void handleWidgetSwitch();
    void handleStreamSwitch(bool checked);
    void handleTimeout();
    void handleReceiveData(int type, QByteArray data);
    void onSnapshotBtn();

private:
    Ui::PreviewForm *ui;
    VidiconProtocol::AlarmParameter msgs[3];
    VidiconProtocol::PullMessage param;
    //闪烁标志
    bool sensorAlarmFlicker;
    bool motionAlarmFlicker;
    bool blindAlarmFlicker;
    bool replySuccess;   //用来同步的拉取警报
};

#endif // PREVIEWWIDGET_H
