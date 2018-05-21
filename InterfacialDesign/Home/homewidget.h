#ifndef HOMEWIDGET_H
#define HOMEWIDGET_H

/**
 * @brief         展示设备列表，对指定的设备进行登录、登出，状态展示
 * @author        xiaobin <xiaobin@sunniwell.com>
 * @date          20180504
 */

#include <QWidget>
#include "searchdevicethread.h"
#include "selectcurrentipdialog.h"
#include "basewidget.h"
#include <QMenu>
#include <QDialog>
#include "authenticator.h"

namespace Ui {
class HomeForm;
}

class HomeWidget : public BaseWidget, public Authenticator
{
    Q_OBJECT
public:
    explicit HomeWidget(QWidget *parent = nullptr);
    ~HomeWidget();

    bool isAuthorization() override;
    QString name() override;
signals:

public slots:
    void reset();
    void onSearchBtnClicked();
    void hanldeDeviceInfo(QVariant value);

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
    int m_beforeLoginRow;       //准备登录前所指定的行
    int m_afterLoginRow;        //登陆成功后对应项所在的行
    QAction *m_loginAction;
    QAction *m_logoutAction;

    QDialog *m_loginDialog;
    QMap<QString, QWidget *> m_loginMap;


    bool isWaiting;
};

#endif // SWITCHWIDGET_H
