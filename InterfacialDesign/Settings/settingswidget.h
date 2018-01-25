#ifndef SETUPWIDGET_H
#define SETUPWIDGET_H

#include <QWidget>
#include <QToolBox>
#include "boxwidget.h"
#include <QTabWidget>
#include "tabmedia.h"
#include "tabnetwork.h"
#include "tabalarm.h"
#include "tabrecord.h"
#include "tabsystem.h"

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
    void handlerToolBoxClicked(int index);

private:
    Ui::SettingsForm *ui;
    QToolBox   *toolbox;
    BoxView    *boxMedia;
    BoxView    *boxNetwork;
    BoxView    *boxAlarm;
    BoxView    *boxRecord;
    BoxView    *boxSystem;

    TabMedia   *tabMedia;
    TabNetwork *tabNetwork;
    TabAlarm   *tabAlarm;
    TabRecord  *tabRecord;
    TabSystem  *tabSystem;
};

#endif // SETUPWIDGET_H
