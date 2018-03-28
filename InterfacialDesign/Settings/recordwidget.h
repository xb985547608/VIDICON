#ifndef TABRECORD_H
#define TABRECORD_H

#include <QMap>
#include "stackedwidget.h"

class RecordWidget : public StackedWidget
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

public slots:
    void setCurrentIndex(const QModelIndex &index) override;
    void handlePrepareData() override;
    void handleReceiveData(VidiconProtocol::Type type, QByteArray data) override;

    void handleTimeSelect(int state);
    void handleSelectAllWeek();

private:
    QWidget *m_scheduleWidget;
    QMap<QString, QWidget *> m_scheduleMap;

    QWidget *m_SDStorageWidget;
    QMap<QString, QWidget *> m_SDStorageMap;

    QWidget *m_snapshotWidget;
    QMap<QString, QWidget *> m_snapshotMap;

    QWidget *m_destinationWidget;
    QMap<QString, QWidget *> m_destinationMap;

    QWidget *m_NASWidget;
    QMap<QString, QWidget *> m_NASMap;
};

#endif // TABRECORD_H
