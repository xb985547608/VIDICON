#ifndef SETUPWIDGET_H
#define SETUPWIDGET_H

#include <QWidget>
#include <QToolBox>
#include "boxview.h"
#include <QTabWidget>
#include "mediawidget.h"
#include "networkwidget.h"
#include "alarmwidget.h"
#include "recordwidget.h"
#include "systemwidget.h"

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
    void handleToolBoxClicked(int index);

private:
    Ui::SettingsForm *ui;
    QToolBox   *toolbox;
    BoxView    *boxMedia;
    BoxView    *boxNetwork;
    BoxView    *boxAlarm;
    BoxView    *boxRecord;
    BoxView    *boxSystem;

    MediaWidget   *tabMedia;
    NetworkWidget *tabNetwork;
    AlarmWidget   *tabAlarm;
    RecordWidget  *tabRecord;
    SystemWidget  *tabSystem;
};

#endif // SETUPWIDGET_H
