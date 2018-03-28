#ifndef PREVIEWWIDGET_H
#define PREVIEWWIDGET_H

#include <QWidget>
#include "Protocol/vidiconprotocol.h"
#include "basicwidget.h"

namespace Ui{
class PreviewForm;
}

class PreviewWidget : public BasicWidget
{
    Q_OBJECT
public:
    explicit PreviewWidget(QWidget *parent = nullptr);
    ~PreviewWidget();

    void updateDynamicProperty(QWidget *w);

signals:
    void signalVlcControl(int type, int subtype = -1, WId id = 0);

public slots:
    void refresh();
    void handleStreamSwitch(bool checked);
    void handleTimeout();
    void handleReceiveData(VidiconProtocol::Type type, QByteArray data);
    void onSnapshotBtn();

private:
    Ui::PreviewForm *ui;
    AlarmParameter msgs[3];
    PullMessage param;
    //闪烁标志
    bool sensorAlarmFlicker;
    bool motionAlarmFlicker;
    bool blindAlarmFlicker;
    bool replySuccess;   //用来同步的拉取警报
};

#endif // PREVIEWWIDGET_H
