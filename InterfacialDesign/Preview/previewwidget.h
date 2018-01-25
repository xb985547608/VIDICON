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
    void signalSetParameter(int type, void *param, QString SessionID = "R00001");
    void signalGetParameter(int type, int StreamType = 0, int Channel = 0, QString SessionID = "R00001");
    void signalVlcControl(int type, QString url = "", WId id = 0);

public slots:
    void handlerWidgetSwitch();
    void handlerStreamSwitch(bool checked);
    void handlerTimeout();
    void handlerReceiveData(int type, QByteArray data);

private:
    Ui::PreviewForm *ui;
    VidiconProtocol::AlarmParameter msgs[3];
    VidiconProtocol::PullMessage param;
    bool sensorAlarmFlicker;
    bool motionAlarmFlicker;
    bool blindAlarmFlicker;
};

#endif // PREVIEWWIDGET_H
