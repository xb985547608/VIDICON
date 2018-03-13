#ifndef TABMEDIA_H
#define TABMEDIA_H

#include <QWidget>
#include <QMap>
#include <QStackedWidget>

#define TABMEDIANUM 5

class MediaWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit MediaWidget(QWidget *parent = nullptr);
    ~MediaWidget();

    void initAudioVideoWidget();
    void initPrivacyWidget();
    void initImageWidget();
    void initROIWidget();
    void initOSDWidget();

    void refreshParameter();

signals:
    void signalSetParameter(int type, void *param = NULL, QString SessionID = "R00001");
    void signalGetParameter(int type, void *param = NULL, QString SessionID = "R00001");

public slots:
    void handleSwitchTab(const QModelIndex &index);
    void handlePrepareData();
    void handleReceiveData(int type, QByteArray data);

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
