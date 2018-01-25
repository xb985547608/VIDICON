#include "loginwidget.h"
#include "ui_loginform.h"
#include <QMouseEvent>
#include <QDebug>
#include "settings.h"
#include <QCryptographicHash>
#include "Protocol/vidiconprotocol.h"
#include <QTimer>

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
    ui->password->setMaxLength(16);

    switchButton = new SwitchButton(ui->sbFactoryMode);
    ui->sbFactoryMode->resize(switchButton->size());

    setWindowOpacity(0.9);

    connect(ui->btnLogin, SIGNAL(clicked()), this, SLOT(onLoginBtn()));
    connect(ui->btnClose, SIGNAL(clicked()), this, SLOT(onCloseBtn()));
    connect(ui->btnMinimize, SIGNAL(clicked()), this, SLOT(onMinimizeBtn()));

    show();
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
//    if(ui->btnLogin->rect().contains(event->pos()))
//        return QWidget::mousePressEvent(event);
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
    QString password = ui->password->text();
    QCryptographicHash c(QCryptographicHash::Md5);
    c.addData(password.toLocal8Bit());
    password = QString(c.result().toHex());
    bool factorySchema = switchButton->getState();
    bool isSuccess = false;

    if(factorySchema){
        if(SettingsObject::getInstance()->getUserMapPasswd(FACTORY_USER).compare(password) == 0){
            isSuccess = true;
            emit signalLoginState(LoginWidget::FactoryLogin);
        }
    }else if(!factorySchema){
        if(SettingsObject::getInstance()->getUserMapPasswd(NORMAL_USER).compare(password) == 0){
            isSuccess = true;
            emit signalLoginState(LoginWidget::NormalLogin);
        }
    }
    emit signalLoginState(LoginWidget::FactoryLogin);
    clear();
    if(!isSuccess){
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
