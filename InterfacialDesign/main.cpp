#include "mainwindow.h"
#include <QApplication>
#include "Login/loginwidget.h"
#include "customstyle.h"
#include "Protocol/vidiconprotocol.h"
#include "Control/vlccontrol.h"
#include "settings.h"
#include <QDebug>
#include <QCryptographicHash>
#include "preparethread.h"
#include <QThread>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    a.setStyle(new CustomStyle);

    SettingsObject::getInstance();

    qRegisterMetaType<WId>("WId");
    PrepareThread *t = new PrepareThread;
    t->start();

    LoginWidget::getInstance();
    MainWindow w;

    qDebug() << "------------------OK------------------";

    return a.exec();
}
