#ifndef TABSYATEM_H
#define TABSYATEM_H

#include <QTabWidget>
#include <QMap>

class TabSystem : public QTabWidget
{
    Q_OBJECT
public:
    explicit TabSystem(QWidget *parent = nullptr);
    ~TabSystem();

    void initMaintenanceWidget();
    void initDeviceInfoWidget();
    void initSetTimeWidget();
    void initUserAdminWidget();

signals:
    void signalSetParameter(int type, void *param, QString SessionID = "R00001");
    void signalGetParameter(int type, int StreamType = 0, int Channel = 0, QString SessionID = "R00001");

public slots:
    void handlerSwitchTab(const QModelIndex &index);
    void handlerPrepareData();
    void handlerReceiveData(int type, QByteArray data);

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
