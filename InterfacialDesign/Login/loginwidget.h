#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QPoint>
#include "SwitchButton/switchbutton.h"

namespace Ui {
class LoginForm;
}

class LoginWidget : public QWidget
{
    Q_OBJECT
public:
    enum LoginState{
        InvalidLogin = -1,
        FactoryLogin,
        NormalLogin
    };
    Q_ENUM(LoginState)

    explicit LoginWidget(QWidget *parent = nullptr);
    ~LoginWidget();
    void clear();

signals:
    void signalLoginState(LoginWidget::LoginState state);

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);

public slots:
    void onLoginBtn();
    void onCloseBtn();
    void onMinimizeBtn();

private:
    Ui::LoginForm *ui;
    bool windowMove;
    QPoint moveStartPos;

    SwitchButton *switchButton;

};

#endif // LOGINWIDGET_H
