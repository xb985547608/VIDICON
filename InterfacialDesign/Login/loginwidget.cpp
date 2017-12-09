#include "loginwidget.h"
#include "ui_loginform.h"
#include <QMouseEvent>
#include <QDebug>

QString factoryAccount("root");
QString factoryPassword("sunniwell");
QString userAccount("user");
QString userPassword("sunniwell");

LoginWidget *LoginWidget::_instance = NULL;
LoginWidget::LoginWidget(QWidget *parent) :
    QWidget(parent) ,
    ui(new Ui::LoginForm), windowMove(false)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    ui->setupUi(this);
    ui->account->setContextMenuPolicy(Qt::NoContextMenu);
    ui->account->setPlaceholderText("Username");
    ui->account->setMaxLength(8);
    ui->password->setContextMenuPolicy(Qt::NoContextMenu);
    ui->password->setPlaceholderText("Password");
    ui->password->setEchoMode(QLineEdit::Password);
    ui->password->setMaxLength(16);

    switchButton = new SwitchButton(ui->sbFactoryMode);
    ui->sbFactoryMode->resize(switchButton->size());

    connect(ui->btnLogin, SIGNAL(clicked()), this, SLOT(onLoginBtn()));
    connect(ui->btnClose, SIGNAL(clicked()), this, SLOT(onCloseBtn()));
    connect(ui->btnMinimize, SIGNAL(clicked()), this, SLOT(onMinimizeBtn()));
}

LoginWidget::~LoginWidget()
{

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
    QString account = ui->account->text();
    QString password = ui->password->text();
    bool factorySchema = switchButton->getState();

    if(account.length() < 4 || password.length() < 8){
        ui->lblHint->setText("账号/密码格式错误");
    }else if(factorySchema && account == factoryAccount && password == factoryPassword){
        ui->account->clear();
        ui->password->clear();
        ui->lblHint->clear();
        emit signalLoginState(LoginWidget::FactoryLogin);
    }else if(!factorySchema && account == userAccount && password == userPassword){
        ui->account->clear();
        ui->password->clear();
        ui->lblHint->clear();
        emit signalLoginState(LoginWidget::UserLogin);
    }else{
        ui->lblHint->setText("账号与密码不匹配");
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
