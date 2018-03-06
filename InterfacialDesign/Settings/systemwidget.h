#ifndef TABSYATEM_H
#define TABSYATEM_H

#include <QStackedWidget>
#include <QMap>

class SystemWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit SystemWidget(QWidget *parent = nullptr);
    ~SystemWidget();

    void initMaintenanceWidget();
    void initDeviceInfoWidget();
    void initSetTimeWidget();
    void initUserAdminWidget();

signals:
    void signalSetParameter(int type, void *param, QString SessionID = "R00001");
    void signalGetParameter(int type, int StreamType = 0, int Channel = 0, QString SessionID = "R00001");

public slots:
    void handleSwitchTab(const QModelIndex &index);
    void handlePrepareData();
    void handleReceiveData(int type, QByteArray data);

private:
    QWidget *maintenanceWidget;
    QMap<QString, QWidget *> maintenanceMap;

    QWidget *deviceInfoWidget;
    QMap<QString, QWidget *> deviceInfoMap;

    QWidget *setTimeWidget;
    QMap<QString, QWidget *> setTimeMap;

    QWidget *userAdminWidget;
    QMap<QString, QWidget *> userAdminMap;
};

#endif // TABSYATEM_H
