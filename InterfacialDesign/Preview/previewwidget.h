#ifndef PREVIEWWIDGET_H
#define PREVIEWWIDGET_H

/**
 * @brief         实时预览当前监控设备的画面，根据警报信息在界面上闪烁对应的图标做出提示
 * @author        xiaobin <xiaobin@sunniwell.com>
 * @date          20180504
 */

#include <QWidget>
#include "Protocol/vidiconprotocol.h"
#include "basewidget.h"

namespace Ui{
class PreviewForm;
}

class PreviewWidget : public BaseWidget
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
