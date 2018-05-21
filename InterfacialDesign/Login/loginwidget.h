#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

/**
 * @brief         登录界面
 * @author        xiaobin <xiaobin@sunniwell.com>
 * @date          20180504
 */

#include <QWidget>
#include <QPoint>

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
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

public slots:
    void onLoginBtn();
    void onCloseBtn();
    void onMinimizeBtn();

private:
    Ui::LoginForm *ui;
    bool windowMove;
    QPoint moveStartPos;
};

#endif // LOGINWIDGET_H
