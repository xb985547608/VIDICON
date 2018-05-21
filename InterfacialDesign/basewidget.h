#ifndef BASICWIDGET_H
#define BASICWIDGET_H

/**
 * @brief         将所有需要与摄像机通信的类共同部分抽象出来
 * @author        xiaobin <xiaobin@sunniwell.com>
 * @date          20180504
 */

#include <QWidget>
#include "Protocol/vidiconprotocol.h"
#include "parsexml.h"

class BaseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BaseWidget(QWidget *parent = nullptr);
    /**
     * 对Layout或Widget进行布局的一个封装
     */
    static void setAlignment(QWidget *parentWidget, QWidget *widget, Qt::Alignment alignment = Qt::AlignCenter);
    static void setAlignment(QWidget *parentWidget, QLayout *layout, Qt::Alignment alignment = Qt::AlignCenter);

signals:
    /**
     * 与设备进行通信的信号
     */
    void signalSetParameter(VidiconProtocol::Type type, const QVariant param = QVariant(), QString SessionID = "R00001");
    void signalGetParameter(VidiconProtocol::Type type, const QVariant param = QVariant(), QString SessionID = "R00001");

public slots:
    /**
     * 接收设备发出的数据
     */
    virtual void handleReceiveData(VidiconProtocol::Type type, QByteArray data) = 0;

};

#endif // BASICWIDGET_H
