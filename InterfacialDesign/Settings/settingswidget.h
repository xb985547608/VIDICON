#ifndef SETUPWIDGET_H
#define SETUPWIDGET_H

#include <QWidget>
#include <QToolBox>
#include <QListWidget>
#include "boxview.h"
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

class SettinsWidget : public QWidget
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
    void refresh(int index = 0);

private:
    Ui::SettingsForm *ui;
    QToolBox    *m_toolbox;
    ListView    *m_boxMedia;
    ListView    *m_boxNetwork;
    ListView    *m_boxAlarm;
    ListView    *m_boxRecord;
    ListView    *m_boxSystem;

    MediaWidget   *m_tabMedia;
    NetworkWidget *m_tabNetwork;
    AlarmWidget   *m_tabAlarm;
    RecordWidget  *m_tabRecord;
    SystemWidget  *m_tabSystem;

    QStackedLayout *m_stackedLayout;
};

#endif // SETUPWIDGET_H
