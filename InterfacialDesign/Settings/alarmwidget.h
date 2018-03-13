#ifndef TABALARM_H
#define TABALARM_H

#include <QStackedWidget>
#include <QMap>
#include <QDialog>

class AlarmWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit AlarmWidget(QWidget *parent = nullptr);
    ~AlarmWidget();

    void initMotionDetectionWidget();
    void initVideoBlindWidget();
    void initAlarmWidget();
    void initRegionEditDialog();

signals:
    void signalSetParameter(int type, void *param = NULL, QString SessionID = "R00001");
    void signalGetParameter(int type, void *param = NULL, QString SessionID = "R00001");

public slots:
    void handleSwitchTab(const QModelIndex &index);
    void handleTimeSelect(int state);
    void handleSelectAllWeek();
    void handlePrepareData();
    void handleReceiveData(int type, QByteArray data);

private:
    QWidget *motionDetectionWidget;
    QMap<QString, QWidget *> motionDetectionMap;

    QDialog *regionEditDialog;
    QMap<QString, QWidget *> regionEditMap;

    QWidget *videoBlindWidget;
    QMap<QString, QWidget *> videoBlindMap;

    QWidget *alarmWidget;
    QMap<QString, QWidget *> alarmMap;
};

#endif // TABALARM_H
