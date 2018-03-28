#ifndef TABALARM_H
#define TABALARM_H

#include <QMap>
#include <QDialog>
#include "stackedwidget.h"

class AlarmWidget : public StackedWidget
{
    Q_OBJECT
public:
    explicit AlarmWidget(QWidget *parent = nullptr);
    ~AlarmWidget();

    void initMotionDetectionWidget();
    void initVideoBlindWidget();
    void initAlarmWidget();
    void initRegionEditDialog();

public slots:
    void setCurrentIndex(const QModelIndex &index) override;
    void handlePrepareData() override;
    void handleReceiveData(VidiconProtocol::Type type, QByteArray data) override;

    void handleTimeSelect(int state);
    void handleSelectAllWeek();

private:
    QWidget *m_motionDetectionWidget;
    QMap<QString, QWidget *> m_motionDetectionMap;

    QDialog *m_regionEditDialog;
    QMap<QString, QWidget *> m_regionEditMap;

    QWidget *m_videoBlindWidget;
    QMap<QString, QWidget *> m_videoBlindMap;

    QWidget *m_alarmWidget;
    QMap<QString, QWidget *> m_alarmMap;
};

#endif // TABALARM_H
