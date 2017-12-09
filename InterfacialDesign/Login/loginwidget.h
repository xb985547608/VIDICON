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
        UserLogin
    };

    static LoginWidget * getInstance()
    {
        if(NULL == _instance)
        {
            if (NULL == _instance)
                _instance = new LoginWidget();
        }
        return _instance;
    }


    ~LoginWidget();

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
    explicit LoginWidget(QWidget *parent = nullptr);
    static LoginWidget* _instance;

    Ui::LoginForm *ui;
    bool windowMove;
    QPoint moveStartPos;
    SwitchButton *switchButton;
};

#endif // LOGINWIDGET_H
