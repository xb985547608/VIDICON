#ifndef HOMEWIDGET_H
#define HOMEWIDGET_H

#include <QWidget>
#include "searchdevicethread.h"
#include "selectcurrentipdialog.h"
#include "basicwidget.h"
#include <QMenu>
#include <QDialog>
#include "Control/cmdongle.h"

namespace Ui {
class HomeForm;
}

class HomeWidget : public BasicWidget
{
    Q_OBJECT
public:
    explicit HomeWidget(QWidget *parent = nullptr);
    ~HomeWidget();

    inline bool isLogin()
    { return m_afterLoginRow != -1 && m_items.count(); }

    inline bool isAuthorization()
    { return m_cmDongle->isAuthorization(); }
signals:

public slots:
    void reset();
    void onSearchBtnClicked();
    void hanldeDeviceInfo(WholeDeviceInfo info);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    void handleReceiveData(VidiconProtocol::Type type, QByteArray data) override;
    void handleLogin(int row);
private:
    void initLoginDialog();
    void createActions();

private:
    Ui::HomeForm *ui;

    SearchDeviceThread *m_searchThread;
    SelectCurrentIpDialog *m_selectIP;
    QList<WholeDeviceInfo> m_items;

    QMenu *m_menu;
    int m_beforeLoginRow;
    int m_afterLoginRow;
    QAction *m_loginAction;
    QAction *m_logoutAction;

    QDialog *m_loginDialog;
    QMap<QString, QWidget *> m_loginMap;

    CMDongle *m_cmDongle;

    bool isWaiting;
};

#endif // SWITCHWIDGET_H
