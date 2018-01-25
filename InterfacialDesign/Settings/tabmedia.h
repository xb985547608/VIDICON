#ifndef TABMEDIA_H
#define TABMEDIA_H

#include <QWidget>
#include <QMap>
#include <QTabWidget>

#define TABMEDIANUM 5

class TabMedia : public QTabWidget
{
    Q_OBJECT
public:
    explicit TabMedia(QWidget *parent = nullptr);
    ~TabMedia();

    void initAudioVideoWidget();
    void initPrivacyWidget();
    void initImageWidget();
    void initROIWidget();
    void initOSDWidget();

    void refreshParameter();

signals:
    void signalSetParameter(int type, void *param, QString SessionID = "R00001");
    void signalGetParameter(int type, int StreamType = 0, int Channel = 0, QString SessionID = "R00001");

public slots:
    void handlerSwitchTab(const QModelIndex &index);
    void handlerPrepareData();
    void handlerReceiveData(int type, QByteArray data);

private:
    QWidget *audioVideoWidget;
    QMap<QString, QWidget *> audioVideoMap;

    QWidget *privacyWidget;
    QMap<QString, QWidget *> privacyMap;

    QWidget *imageWidget;
    QMap<QString, QWidget *> imageMap;

    QWidget *roiWidget;
    QMap<QString, QWidget *> ROIMap;

    QWidget *osdWidget;
    QMap<QString, QWidget *> OSDMap;
};

#endif // TABMEDIA_H
