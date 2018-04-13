#ifndef TABSYATEM_H
#define TABSYATEM_H

#include <QMap>
#include "stackedwidget.h"

class SystemWidget : public StackedWidget
{
    Q_OBJECT
public:
    explicit SystemWidget(QWidget *parent = nullptr);
    ~SystemWidget();

    void initMaintenanceWidget();
    void initDeviceInfoWidget();
    void initSetTimeWidget();
    void initUserConfigWidget();
    void initWifiSettinsWidget();

public slots:
    void setCurrentIndex(const QModelIndex &index) override;
    void handlePrepareData() override;
    void handleReceiveData(VidiconProtocol::Type type, QByteArray data) override;

private:
    QWidget *m_maintenanceWidget;
    QMap<QString, QWidget *> m_maintenanceMap;

    QWidget *m_deviceInfoWidget;
    QMap<QString, QWidget *> m_deviceInfoMap;

    QWidget *m_setTimeWidget;
    QMap<QString, QWidget *> m_setTimeMap;

    QWidget *m_userConfigWidget;
    QMap<QString, QWidget *> m_userConfigMap;

    QWidget *m_wifiSettinsWidget;
    QMap<QString, QWidget *> m_wifiSettinsMap;
};

#endif // TABSYATEM_H
