#ifndef TABMEDIA_H
#define TABMEDIA_H

#include <QWidget>
#include <QMap>
#include "stackedwidget.h"

#define TABMEDIANUM 5

class MediaWidget : public StackedWidget
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

public slots:
    void setCurrentIndex(const QModelIndex &index) override;
    void handlePrepareData() override;
    void handleReceiveData(VidiconProtocol::Type type, QByteArray data) override;

private:
    QWidget *m_audioVideoWidget;
    QMap<QString, QWidget *> m_audioVideoMap;

    QWidget *m_privacyWidget;
    QMap<QString, QWidget *> m_privacyMap;

    QWidget *m_imageWidget;
    QMap<QString, QWidget *> m_imageMap;

    QWidget *m_roiWidget;
    QMap<QString, QWidget *> m_ROIMap;

    QWidget *m_osdWidget;
    QMap<QString, QWidget *> m_OSDMap;
};

#endif // TABMEDIA_H
