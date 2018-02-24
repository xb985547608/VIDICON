#include "mainwindow.h"
#include <QApplication>
#include "Login/loginwidget.h"
#include "customstyle.h"
#include "Protocol/vidiconprotocol.h"
#include "Control/vlccontrol.h"
#include "settings.h"
#include <QDebug>
#include <QCryptographicHash>
#include <QThread>
#include <QMetaObject>
#include "searchdevicethread.h"
#include "selectcurrentipdialog.h"
#include <QLabel>
#include <QPropertyAnimation>
#include "Network/httpdownload.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    a.setStyle(new CustomStyle);

    SettingsObject::getInstance();

    SearchDeviceThread *s = new SearchDeviceThread;
    {
        //网卡地址选择
        SelectCurrentIpDialog d;
        if(d.exec()) {
            qDebug() << d.getCurrentSelectIp().toString();
            s->setSpecifiedIP(d.getCurrentSelectIp());
        }else {
            exit(-1);
        }
    }
    //开始搜寻设备
    s->start();
    QLabel *lbl = new QLabel("正在搜寻设备中...");
    lbl->setAlignment(Qt::AlignCenter);
    lbl->setFixedSize(200, 100);
    lbl->show();

    //等待成功寻找到设备
    QObject::connect(s, &SearchDeviceThread::signalDeviceInfo, lbl, [lbl](SearchDeviceThread::DeviceInfo *info){
        //将一些较为耗时的类放入次线程中运行
        QThread *t1 = new QThread;
        VidiconProtocol *vp = VidiconProtocol::getInstance(info->IPAddr, info->HTTPPort);
        vp->moveToThread(t1);
        QObject::connect(t1, &QThread::started, vp, &VidiconProtocol::init);

        QThread *t2 = new QThread;
        VlcControl *vc = VlcControl::getInstance(info->IPAddr, info->RTSPPort);
        vc->moveToThread(t2);

        QThread *t3 = new QThread;
        HttpDownload *hd = HttpDownload::getInstance(info->IPAddr, info->HTTPPort);
        hd->moveToThread(t3);
        QObject::connect(t3, &QThread::started, hd, &HttpDownload::init);

        t1->start();
        t2->start();
        t3->start();
        delete info;

        lbl->setText("搜寻设备成功");
        QPropertyAnimation *animation = new QPropertyAnimation(lbl, "windowOpacity");
        animation->setDuration(2000);
        animation->setStartValue(1.0);
        animation->setEndValue(0.0);
        animation->setLoopCount(1);
        QObject::connect(animation, &QPropertyAnimation::finished, lbl, [lbl](){
            LoginWidget::getInstance();
            MainWindow *w = new MainWindow;
            Q_UNUSED(w);
            lbl->deleteLater();
        });
        QObject::connect(animation, &QPropertyAnimation::finished, animation, &QPropertyAnimation::deleteLater);
        animation->start();
    });

    qRegisterMetaType<WId>("WId");
    qDebug() << "------------------start------------------";

    return a.exec();
}
