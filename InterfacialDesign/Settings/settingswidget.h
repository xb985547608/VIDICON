#ifndef SETUPWIDGET_H
#define SETUPWIDGET_H

/**
 * @brief         创建并管理各类参数设置界面
 * @author        xiaobin <xiaobin@sunniwell.com>
 * @date          20180504
 */

#include <QWidget>
#include <QToolBox>
#include <QListWidget>
#include "listview.h"
#include <QTabWidget>
#include "mediawidget.h"
#include "networkwidget.h"
#include "alarmwidget.h"
#include "recordwidget.h"
#include "systemwidget.h"
#include <QStackedLayout>

namespace Ui{
class SettingsForm;
}

class SettinsWidget : public BaseWidget
{
    Q_OBJECT
public:
    explicit SettinsWidget(QWidget *parent = nullptr);
    ~SettinsWidget();

private:
    void initSideBar();
    void initDisplayArea();
    void initMediaWidget();

signals:

public slots:
    void handleReceiveData(VidiconProtocol::Type type, QByteArray data) override;

    void refresh(int index = 0);

private:
    Ui::SettingsForm *m_ui;
    QToolBox    *m_toolbox;
    ListView    *m_listMedia;
    ListView    *m_listNetwork;
    ListView    *m_listAlarm;
    ListView    *m_listRecord;
    ListView    *m_listSystem;

    MediaWidget   *m_tabMedia;
    NetworkWidget *m_tabNetwork;
    AlarmWidget   *m_tabAlarm;
    RecordWidget  *m_tabRecord;
    SystemWidget  *m_tabSystem;

    QStackedLayout *m_stackedLayout;
};

#endif // SETUPWIDGET_H
