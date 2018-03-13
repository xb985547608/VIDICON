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
    void signalSetParameter(int type, void *param = NULL, QString SessionID = "R00001");
    void signalGetParameter(int type, void *param = NULL, QString SessionID = "R00001");

public slots:
    void handleSwitchTab(const QModelIndex &index);
    void handleTimeSelect(int state);
    void handleSelectAllWeek();
    void handlePrepareData();
    void handleReceiveData(int type, QByteArray data);

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
