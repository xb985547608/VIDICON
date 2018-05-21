#ifndef TABSYATEM_H
#define TABSYATEM_H

/**
 * @brief         主要对设备系统参数的可视化和设置
 * @author        xiaobin <xiaobin@sunniwell.com>
 * @date          20180504
 */

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
    void initWifiConfigWidget();

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
