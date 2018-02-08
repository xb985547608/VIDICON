#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include "Protocol/vidiconprotocol.h"
#include "parsexml.h"
#include "Settings/waitingshade.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    switchWidget = new SwitchWidget(ui->frameSwitch);
    QVBoxLayout *layout1 = new QVBoxLayout(ui->frameSwitch);
    layout1->setSpacing(0);
    layout1->setContentsMargins(0, 0, 0, 0);
    layout1->addWidget(switchWidget);

    homeWidget = new HomeWidget(ui->frameContent);
    homeWidget->setVisible(false);

    previewWidget = new PreviewWidget(ui->frameContent);
    previewWidget->setVisible(false);

    playbackWidget = new PlaybackWidget(ui->frameContent);
    playbackWidget->setVisible(false);

    photoWidget = new PhotoWidget(ui->frameContent);
    photoWidget->setVisible(false);

    settinsWidget = new SettinsWidget(ui->frameContent);
    settinsWidget->setVisible(false);

    WaitingShade::getInstance(this);

    QVBoxLayout *layout2 = new QVBoxLayout(ui->frameContent);
    layout2->setSpacing(0);
    layout2->setContentsMargins(0, 0, 0, 0);
    layout2->addWidget(homeWidget);
    layout2->addWidget(previewWidget);
    layout2->addWidget(playbackWidget);
    layout2->addWidget(photoWidget);
    layout2->addWidget(settinsWidget);

    connect(VidiconProtocol::getInstance(), &VidiconProtocol::signalSendData, this, &MainWindow::handlerReceiveData);
    connect(LoginWidget::getInstance(), SIGNAL(signalLoginState(LoginWidget::LoginState)), this, SLOT(loginHandler(LoginWidget::LoginState)));
    connect(switchWidget, SIGNAL(signalLogout()), this, SLOT(logoutHandler()));
    connect(switchWidget, SIGNAL(signalSwitchState(SwitchWidget::SwitchState)), this, SLOT(switchStateHandler(SwitchWidget::SwitchState)));
    connect(switchWidget, &SwitchWidget::signalSwitchState, previewWidget, &PreviewWidget::handlerWidgetSwitch);
    connect(switchWidget, &SwitchWidget::signalSwitchState, playbackWidget, &PlaybackWidget::handlerWidgetSwitch);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loginHandler(LoginWidget::LoginState state)
{
    if(state == LoginWidget::InvalidLogin){
        return;
    }else if(state == LoginWidget::FactoryLogin){
        switchWidget->setSettingsBtnVisible(true);
    }else if(state == LoginWidget::NormalLogin){
        switchWidget->setSettingsBtnVisible(false);
    }
    LoginWidget::getInstance()->setVisible(false);
    activateWindow();
    setVisible(true);
    switchStateHandler(SwitchWidget::Preview);
}

void MainWindow::logoutHandler()
{
    LoginWidget::getInstance()->setVisible(true);
    setVisible(false);
}

void MainWindow::switchStateHandler(SwitchWidget::SwitchState form)
{
    resetWidgetState();
    switch(form){
        case SwitchWidget::Home:
            homeWidget->setVisible(true);
            break;
        case SwitchWidget::Preview:
            previewWidget->setVisible(true);
            previewWidget->handlerWidgetSwitch();
            break;
        case SwitchWidget::Playback:
            playbackWidget->setVisible(true);
            break;
        case SwitchWidget::Photo:
            photoWidget->setVisible(true);
            break;
        case SwitchWidget::Settings:
            settinsWidget->setVisible(true);
            settinsWidget->handlerToolBoxClicked(0);
            break;
        default:
            break;
    }
}

void MainWindow::handlerReceiveData(int type, QByteArray data)
{
    WaitingShade *w = WaitingShade::getInstance();
    if(!isVisible()) {
        return;
    }

    switch(type) {
    case RESPONSESTATUS: {
        VidiconProtocol::ResponseStatus reply;
        if(ParseXML::getInstance()->parseResponseStatus(&reply, data)) {
            if(reply.StatusCode != 1) {
                QMessageBox::information(this, "参数设置", QString("%1 错误").arg(reply.StatusCode));
            }else {
                QMessageBox::information(this, "参数设置", QString("参数设置成功"));
            }
        }
        if(w->isVisible()) {
            w->hide();
        }
        break;
    }
    case NETWORKERROR: {
        QMessageBox::information(this, "参数设置", QString("网络错误，请稍后重试"));
        if(w->isVisible()) {
            w->hide();
        }
        break;
    }
    default:
        break;
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
}

void MainWindow::resetWidgetState()
{
    homeWidget->setVisible(false);
    previewWidget->setVisible(false);
    playbackWidget->setVisible(false);
    photoWidget->setVisible(false);
    settinsWidget->setVisible(false);
}



