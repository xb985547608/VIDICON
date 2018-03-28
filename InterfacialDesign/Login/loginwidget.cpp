#include "loginwidget.h"
#include "ui/ui_loginform.h"
#include "settings.h"
#include <QCryptographicHash>
#include <QMouseEvent>
#include <QDebug>

LoginWidget::LoginWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginForm),
    windowMove(false)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    ui->setupUi(this);
    ui->password->setContextMenuPolicy(Qt::NoContextMenu);
    ui->password->setPlaceholderText("Password");
    ui->password->setEchoMode(QLineEdit::Password);
    ui->password->setMaxLength(15);

    switchButton = new SwitchButton(ui->sbFactoryMode);
    ui->sbFactoryMode->resize(switchButton->size());

    setWindowOpacity(0.9);

    connect(ui->btnLogin, SIGNAL(clicked()), this, SLOT(onLoginBtn()));
    connect(ui->btnClose, SIGNAL(clicked()), this, SLOT(onCloseBtn()));
    connect(ui->btnMinimize, SIGNAL(clicked()), this, SLOT(onMinimizeBtn()));
}

LoginWidget::~LoginWidget()
{

}

void LoginWidget::clear()
{
    ui->password->clear();
    ui->lblHint->clear();
}

void LoginWidget::mousePressEvent(QMouseEvent *event)
{
    windowMove = true;
    moveStartPos = event->globalPos() - pos();
    QWidget::mousePressEvent(event);
}

void LoginWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (windowMove && (event->buttons() && Qt::LeftButton)
        && (event->globalPos() - moveStartPos).manhattanLength() > QApplication::startDragDistance())
    {
        move(event->globalPos() - moveStartPos);
        moveStartPos = event->globalPos() - pos();
    }
    QWidget::mouseMoveEvent(event);
}

void LoginWidget::mouseReleaseEvent(QMouseEvent *event)
{
    windowMove = false;
    QWidget::mouseReleaseEvent(event);
}

void LoginWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
        onLoginBtn();
    QWidget::keyPressEvent(event);
}

void LoginWidget::onLoginBtn()
{
    QString passwd;
#if 1
    passwd = ui->password->text();
#else
    passwd = "admin";
#endif
    LoginState mode = switchButton->getState() ? FactoryLogin : NormalLogin;
    QString level = mode == FactoryLogin ? FACTORY_USER : NORMAL_USER;
    QCryptographicHash c(QCryptographicHash::Md5);
    c.addData(passwd.toLocal8Bit().data());
    if (SettingsObject::mixMD5(c.result()).toHex().toStdString().data() ==
            SettingsObject::getInstance()->getUserMapPasswd(level)) {
        qDebug() << "#LoginWidget# MODE:" << mode << ", Login OK";
        emit signalLoginState(mode);
        hide();
//        deleteLater();
    } else {
        ui->lblHint->setText("密码错误");
    }
}

void LoginWidget::onCloseBtn()
{
    close();
}

void LoginWidget::onMinimizeBtn()
{
    showMinimized();
}
