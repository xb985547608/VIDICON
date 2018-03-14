#include "loginwidget.h"
#include "ui/ui_loginform.h"
#include <QMouseEvent>
#include <QDebug>
#include "settings.h"
#include <QCryptographicHash>
#include "Protocol/vidiconprotocol.h"
#include "Control/vlccontrol.h"
#include "parsexml.h"
#include <QThread>
#include <QTimer>
#include <QMetaObject>

LoginWidget *LoginWidget::_instance = NULL;
LoginWidget::LoginWidget(QWidget *parent) :
    QWidget(parent) ,
    ui(new Ui::LoginForm), windowMove(false)
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
    connect(VidiconProtocol::getInstance(), &VidiconProtocol::signalReceiveData, this, &LoginWidget::handleReceiveData);

    show();
}

LoginWidget::~LoginWidget()
{

}

void LoginWidget::clear()
{
    ui->trainNum->clear();
    ui->coachNum->clear();
    ui->seatNum->clear();
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
#if 0
    user = ui->trainNum->text() + ui->coachNum->text() + ui->seatNum->text();
    passwd = ui->password->text();
#else
    user = "admin";
    passwd = "admin";
#endif
    QMetaObject::invokeMethod(VidiconProtocol::getInstance(), "login", Q_ARG(QString, user), Q_ARG(QString, passwd));
}

void LoginWidget::onCloseBtn()
{
    close();
}

void LoginWidget::onMinimizeBtn()
{
    showMinimized();
}

void LoginWidget::handleReceiveData(int type, QByteArray data)
{
    switch(type) {
    case LOGIN: {
        VidiconProtocol::ResponseStatus reply;
        if(ParseXML::getInstance()->parseResponseStatus(&reply, data)) {
            if(reply.StatusString.compare("OK", Qt::CaseInsensitive) == 0) {
                VlcControl::getInstance()->setUser(user);
                VlcControl::getInstance()->setPasswd(passwd);

                emit signalLoginState(switchButton->getState() ? LoginWidget::FactoryLogin : LoginWidget::NormalLogin);
                clear();
            }else {
                ui->lblHint->setText("密码错误");
            }
        }
        break;
    }
    default:
        break;
    }
}
