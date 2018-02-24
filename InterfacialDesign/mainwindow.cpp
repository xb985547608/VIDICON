#include "mainwindow.h"
#include "ui/ui_mainwindow.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include "Protocol/vidiconprotocol.h"
#include "parsexml.h"
#include "Settings/waitingshade.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    navigationWidget = new NavigationWidget(this);
    setCentralWidget(navigationWidget);

//    homeWidget      = new HomeWidget(navigationWidget);
    previewWidget   = new PreviewWidget(navigationWidget);
    playbackWidget  = new PlaybackWidget(navigationWidget);
//    photoWidget     = new PhotoWidget(navigationWidget);
    settinsWidget   = new SettinsWidget(navigationWidget);
    downloadWidget  = new DownloadWidget(navigationWidget);

    navigationWidget->addTab(previewWidget,  tr("预览"));
    navigationWidget->addTab(playbackWidget, tr("回放"));
    navigationWidget->addTab(settinsWidget,  tr("设置"));
    navigationWidget->addTab(downloadWidget, tr("下载"));

    WaitingShade::getInstance(this);

    connect(playbackWidget, &PlaybackWidget::signalAddDownloadTask, downloadWidget, &DownloadWidget::enqueue);
    connect(VidiconProtocol::getInstance(), &VidiconProtocol::signalSendData, this, &MainWindow::handlerReceiveData);
    connect(LoginWidget::getInstance(), SIGNAL(signalLoginState(LoginWidget::LoginState)), this, SLOT(loginHandler(LoginWidget::LoginState)));
    connect(navigationWidget, &NavigationWidget::currentChanged, this, [this](int index) {
        previewWidget->handlerWidgetSwitch();
        playbackWidget->handlerWidgetSwitch();
        if(index == 2) {
            settinsWidget->handlerToolBoxClicked(0);
        }
    });

    setMinimumSize(800, 600);
    resize(800, 600);
}

MainWindow::~MainWindow()
{
}

void MainWindow::loginHandler(LoginWidget::LoginState state)
{
    Q_UNUSED(state);
    LoginWidget::getInstance()->setVisible(false);
    setVisible(true);
}

void MainWindow::logoutHandler()
{
    LoginWidget::getInstance()->setVisible(true);
    setVisible(false);
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


