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
    VidiconProtocol *v = VidiconProtocol::getInstance();
    v->init();

    PrepareThread *t = new PrepareThread;
    t->start();

    LoginWidget::getInstance();
    MainWindow w;



//    VidiconProtocol v("192.168.0.66", "80");

//    v.login("admin", "admin");

//    v.getDeviceInfomation("R00001");
//    v.getAudioEncodingParameter("R00001");
//    v.getAudioEncodingCapability("R00001");
//    v.getROIParameter("R00001");
//    v.getPrivacyMaskParameter("R00001");
//    v.getOSDParameter("9f474a58");
//    VidiconProtocol::OSDParameter param[4] = {
//        {0, 1, 2, "cam01",      20, 10 , 16, "#ffff00"},
//        {1, 1, 2, "",           10, 60 },
//        {2, 1, 2, "",           10, 110},
//        {3, 1, 2, "sunniwell",  10, 160},
//    };
//    v.setOSDParameter("R00002", param[0], param[1], param[2], param[3]);

//    v.getBasicParameter("R00001");
//    v.getEmailParameter("R00001");
//    v.getFTPParameter("R00001");
//    v.getDDNSParameter("R00001");
//    v.getPPPOEParameter("R00001");
//    v.getSNMPParameter("R00001");
//    v.getP2PParameter("R00001");
//    v.setP2PParameter("R00001", 1);
//    v.getBonjourParameter("R00001");
//    v.getHTTPsParameter("R00001");
//    v.getUPNPParameter("R00001");
//    v.getOtherParameter("R00001");

//    v.getRemoteRecordingPlan("R00001");
//    v.getSDCardStatusQuery("R00001");
//    v.setSDCardOperation("R00001", 1);
//    v.getSDStorage("R00001");
//    v.setSDStorage("R00001", 0);
//    v.getSnapshotPlanParameter("R00001");
//    v.getMotionDetectionParameter("R00001");
//    v.getSensorAlarmParameter("R00001");
//    v.getVideoBlindAlarmParameter("R00001");
//    v.getPullAlarmRequest("R00001", 0);
//    v.getMotionDetectionChanged("R00001", 0);

//    v.getImageParameter("R00001");

//    v.getNTP("R00001");
//    v.setRestoreDefaultParameter("R00001", 1);

//    v.PTZControl("R00001", -1, -1);
//    v.getInformationOfLatestRecognizedPlates("R00001");
//    v.getNTP("R00001");
//    VidiconProtocol::NTPParameter ntp = {"GMT+08:00", 1, 1, "1994-09-24T00:00:00", "time.windows.com"};
//    v.setNTP("R00001", ntp);

//    v.deviceDiscover();


    qDebug() << "------------------OK------------------";

    return a.exec();
}
