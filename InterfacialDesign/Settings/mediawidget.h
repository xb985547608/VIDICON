#ifndef TABMEDIA_H
#define TABMEDIA_H

/**
 * @brief         主要对媒体参数的可视化和设置
 * @author        xiaobin <xiaobin@sunniwell.com>
 * @date          20180504
 */

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

    /**
     * 检查当前界面的一些输入是否合法，从而控制当前"save"按键的使能or失能
     */
    void checkInput(QString text);

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

    QList<QLineEdit *> m_notPass;
};

#endif // TABMEDIA_H
