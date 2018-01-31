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

    QObject::connect(s, &SearchDeviceThread::signalDeviceInfo, lbl, [lbl](SearchDeviceThread::DeviceInfo *info){
        QThread *t = new QThread();

        VidiconProtocol *vp = VidiconProtocol::getInstance(info->IPAddr, info->HTTPPort);
        vp->moveToThread(t);
        QObject::connect(t, &QThread::started, vp, &VidiconProtocol::init);

        VlcControl::getInstance()->setHost(info->IPAddr);
        VlcControl::getInstance()->setPort(QString::number(info->RTSPPort));
        VlcControl::getInstance()->moveToThread(t);

        QObject::connect(t, SIGNAL(finished()), t, SLOT(deleteLater()));
        t->start();
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
