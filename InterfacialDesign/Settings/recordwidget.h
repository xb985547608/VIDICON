#ifndef TABRECORD_H
#define TABRECORD_H

#include <QStackedWidget>
#include <QMap>

class RecordWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit RecordWidget(QWidget *parent = nullptr);
    ~RecordWidget();

    void initScheduleWidget();
    void initSDStorageWidget();
    void initSnapshotWidget();
    void initDestinationWidget();
    void initNASWidget();

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
    QWidget *scheduleWidget;
    QMap<QString, QWidget *> scheduleMap;

    QWidget *SDStorageWidget;
    QMap<QString, QWidget *> SDStorageMap;

    QWidget *snapshotWidget;
    QMap<QString, QWidget *> snapshotMap;

    QWidget *destinationWidget;
    QMap<QString, QWidget *> destinationMap;

    QWidget *NASWidget;
    QMap<QString, QWidget *> NASMap;
};

#endif // TABRECORD_H
