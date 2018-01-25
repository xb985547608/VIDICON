#ifndef TABALARM_H
#define TABALARM_H

#include <QTabWidget>
#include <QMap>
#include <QDialog>

class TabAlarm : public QTabWidget
{
    Q_OBJECT
public:
    explicit TabAlarm(QWidget *parent = nullptr);
    ~TabAlarm();

    void initMotionDetectionWidget();
    void initVideoBlindWidget();
    void initAlarmWidget();
    void initRegionEditDialog();

signals:
    void signalSetParameter(int type, void *param, QString SessionID = "R00001");
    void signalGetParameter(int type, int StreamType = 0, int Channel = 0, QString SessionID = "R00001");

public slots:
    void handlerSwitchTab(const QModelIndex &index);
    void handlerTimeSelect(int state);
    void handlerSelectAllWeek();
    void handlerPrepareData();
    void handlerReceiveData(int type, QByteArray data);


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
