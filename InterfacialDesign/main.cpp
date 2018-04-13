#include "mainwindow.h"
#include <QApplication>
#include "Login/loginwidget.h"
#include "customstyle.h"
#include "Protocol/vidiconprotocol.h"
#include "Control/vlccontrol.h"
#include <QDebug>
#include <QThread>
#include "settings.h"
#include <QPropertyAnimation>
#include "Network/httpdownload.h"
#include "log4qt/logmanager.h"
#include "log4qt/propertyconfigurator.h"
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    a.setStyle(new CustomStyle);

//    qRegisterMetaType<WholeDeviceInfo>("WholeDeviceInfo");
    qRegisterMetaType<WId>("WId");

    //永久性配置信息管理
    SettingsObject::getInstance();
    //启动日志记录器
    Log4Qt::PropertyConfigurator::configure(":/logger.configure");
    Log4Qt::LogManager::setHandleQtMessages(true);

    //将一些较为耗时的操作放入次线程中运行
//    QThread *t1 = new QThread;
    VidiconProtocol *vp = VidiconProtocol::getInstance();
    vp->init();
//    vp->moveToThread(t1);
//    QObject::connect(t1, &QThread::started, vp, &VidiconProtocol::init);
//    t1->start();

    QThread *t2 = new QThread;
    VlcControl *vc = VlcControl::getInstance();
    vc->moveToThread(t2);
    t2->start();

    QThread *t3 = new QThread;
    HttpDownload *hd = HttpDownload::getInstance();
    hd->moveToThread(t3);
    QObject::connect(t3, &QThread::started, hd, &HttpDownload::init);
    t3->start();

    //本地登录成功后方可进入软件
    LoginWidget loginWidget;
    MainWindow mainWindow;
    QObject::connect(&loginWidget, SIGNAL(signalLoginState(LoginWidget::LoginState)), &mainWindow, SLOT(loginHandler(LoginWidget::LoginState)));

    loginWidget.show();

    qDebug() << "------------------start------------------";

    return a.exec();
}

