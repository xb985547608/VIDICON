#include "mainwindow.h"
#include "ui/ui_mainwindow.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include "Protocol/vidiconprotocol.h"
#include "parsexml.h"
#include "Settings/waitingshade.h"
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

    WaitingShade::getInstance(this);
    StatusTip::getInstance(this);

    connect(m_playbackWidget, &PlaybackWidget::signalAddDownloadTask, m_downloadWidget, &DownloadWidget::enqueue);
    connect(VidiconProtocol::getInstance(), &VidiconProtocol::signalReceiveData, this, &MainWindow::handleReceiveData);
    connect(m_navigationWidget, &NavigationWidget::currentChanged, this, &MainWindow::handleCurrentChange);

    setMinimumSize(850, 700);
    resize(850, 700);
}

MainWindow::~MainWindow()
{
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

void MainWindow::handleReceiveData(VidiconProtocol::Type type, QByteArray data)
{
    WaitingShade *w = WaitingShade::getInstance();
    StatusTip *s = StatusTip::getInstance();
    if(!isVisible()) {
        return;
    }

    switch(type) {
    case VidiconProtocol::RESPONSESTATUS: {
        ResponseStatus reply;
        if(ParseXML::getInstance()->parseResponseStatus(&reply, data)) {
            QString info;
            if(reply.StatusCode == 1) {
                info = "参数设置成功";
            }else {
                info = "参数设置失败";
            }
            s->showStatusTip(info);
        }
        if(w->isVisible()) {
            w->hide();
        }
        break;
    }
    case VidiconProtocol::NETWORKERROR: {
        if (data.isNull())
            s->showStatusTip("夭寿啦~~网络出现未知状况");
        else
            s->showStatusTip(data);
        if(w->isVisible()) {
            w->hide();
        }
        break;
    }
    default:
        break;
    }
}

void MainWindow::handleCurrentChange(int index)
{
    if (index == 1 || index == 2 || index == 3) {
        /*if (!m_homeWidget->isLogin()) {
            m_navigationWidget->setCurrentIndex(0);
            QMessageBox::warning(m_navigationWidget, "警告", "您还未登录，请登录后重试");
        } else if (!m_homeWidget->isAuthorization()) {
            m_navigationWidget->setCurrentIndex(0);
            QMessageBox::warning(m_navigationWidget, "警告", "U盾授权失败");
        } else */{
            if(index == 1) {
                m_previewWidget->refresh();
            } else if (index == 2) {
                m_playbackWidget->refresh();
            } else if (index == 3) {
                m_settinsWidget->refresh();
            }
        }
    }
}


