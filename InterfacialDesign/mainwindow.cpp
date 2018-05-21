#include "mainwindow.h"
#include "ui/ui_mainwindow.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QApplication>
#include "Protocol/vidiconprotocol.h"
#include "parsexml.h"
#include "statustip.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    m_navigationWidget = new NavigationWidget(this);
    setCentralWidget(m_navigationWidget);

    m_homeWidget      = new HomeWidget(m_navigationWidget);
    m_previewWidget   = new PreviewWidget(m_navigationWidget);
    m_playbackWidget  = new PlaybackWidget(m_navigationWidget);
//    photoWidget     = new PhotoWidget(navigationWidget);
    m_settinsWidget   = new SettinsWidget(m_navigationWidget);
    m_downloadWidget  = new DownloadWidget(m_navigationWidget);

    m_navigationWidget->addTab(m_homeWidget,     QIcon(":/images/mw_home.png"),     tr("主页"));
    m_navigationWidget->addTab(m_previewWidget,  QIcon(":/images/mw_preview.png"),  tr("预览"));
    m_navigationWidget->addTab(m_playbackWidget, QIcon(":/images/mw_playback.png"), tr("回放"));
    m_navigationWidget->addTab(m_settinsWidget,  QIcon(":/images/mw_settings.png"), tr("设置"));
    m_navigationWidget->addTab(m_downloadWidget, QIcon(":/images/mw_download.png"), tr("下载"));


    StatusTip::getInstance(this);

    CMDongle *cmDongle = new CMDongle(m_navigationWidget);

    //添加验证器
    m_authenticators.append(m_homeWidget);
    m_authenticators.append(cmDongle);

    connect(m_playbackWidget, &PlaybackWidget::signalAddDownloadTask, m_downloadWidget, &DownloadWidget::enqueue);
    connect(VidiconProtocol::getInstance(), &VidiconProtocol::error, this, &MainWindow::handleError);
    connect(m_navigationWidget, &NavigationWidget::currentChanged, this, &MainWindow::handleCurrentChange);

    setMinimumSize(880, 700);
    resize(880, 700);
}

MainWindow::~MainWindow()
{
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    qApp->exit();
}

void MainWindow::loginHandler(LoginWidget::LoginState state)
{
    Q_UNUSED(state);
    setVisible(true);
}

void MainWindow::logoutHandler()
{
    setVisible(false);
}

void MainWindow::handleError(QNetworkReply::NetworkError error)
{
    StatusTip *s = StatusTip::getInstance();
    QString msg;
    if (error == QNetworkReply::OperationCanceledError)
        msg = "网络超时";
    else
        msg = "网络出错";

    s->showStatusTip(msg);
    qDebug() << "#MainWindow# handleError, error: " << error;
}

void MainWindow::handleCurrentChange(int index)
{
    if (index == 1 || index == 2 || index == 3) {
        bool isOK = true;
        QString msg;

        foreach (Authenticator *authenticator, m_authenticators) {
            if (!authenticator->isAuthorization()) {

                if (authenticator->name().compare("摄像机") == 0) {
                    msg = "尚未登录设备，请登录后重试";
                    isOK = false;
                    break;
                } /*else if (authenticator->name().compare("U盾") == 0) {
                    if (index != 1) {
                        msg = "U盾授权失败，请稍后重试";
                        isOK = false;
                        break;
                    }
                }*/
            }
        }

        if (isOK){
            if(index == 1) {
                m_previewWidget->refresh();
            } else if (index == 2) {
                m_playbackWidget->refresh();
            } else if (index == 3) {
                m_settinsWidget->refresh();
            }
        } else {
            m_navigationWidget->setCurrentIndex(0);
            QMessageBox::warning(m_navigationWidget, "警告", msg);
        }
    }
}


