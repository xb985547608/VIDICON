#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>

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

    QVBoxLayout *layout2 = new QVBoxLayout(ui->frameContent);
    layout2->setSpacing(0);
    layout2->setContentsMargins(0, 0, 0, 0);
    layout2->addWidget(homeWidget);
    layout2->addWidget(previewWidget);
    layout2->addWidget(playbackWidget);
    layout2->addWidget(photoWidget);
    layout2->addWidget(settinsWidget);

    connect(LoginWidget::getInstance(), SIGNAL(signalLoginState(LoginWidget::LoginState)), this, SLOT(loginHandler(LoginWidget::LoginState)));
    connect(switchWidget, SIGNAL(signalLogout()), this, SLOT(logoutHandler()));
    connect(switchWidget, SIGNAL(signalSwitchState(SwitchWidget::SwitchState)), this, SLOT(switchStateHandler(SwitchWidget::SwitchState)));
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
    }else if(state == LoginWidget::UserLogin){
        switchWidget->setSettingsBtnVisible(false);
    }
    LoginWidget::getInstance()->setVisible(false);
    show();
}

void MainWindow::logoutHandler()
{
    this->setVisible(false);
    LoginWidget::getInstance()->setVisible(true);
}

void MainWindow::switchStateHandler(SwitchWidget::SwitchState form)
{
    resetWidgetState();
    switch(form){
        case SwitchWidget::Main:
            homeWidget->setVisible(true);
            break;
        case SwitchWidget::Preview:
            previewWidget->setVisible(true);
            break;
        case SwitchWidget::Playback:
            playbackWidget->setVisible(true);
            break;
        case SwitchWidget::Photo:
            photoWidget->setVisible(true);
            break;
        case SwitchWidget::Settings:
            settinsWidget->setVisible(true);
            break;
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
