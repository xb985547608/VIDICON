#ifndef IMAGEBASEWIDGET_H
#define IMAGEBASEWIDGET_H

/**
 * @brief         通过获取图片的方式显示监控设备的画面
 * @author        xiaobin <xiaobin@sunniwell.com>
 * @date          20180504
 */

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QPixmap>
#include <QDebug>

#include "Network/httpdownload.h"

class ImageBaseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ImageBaseWidget(QWidget *parent) : QWidget(parent){
        QTimer *timer = new QTimer(this);
        timer->start(100);
        connect(timer, &QTimer::timeout, this, &ImageBaseWidget::handleTimeout);
        connect(HttpDownload::getInstance(), &HttpDownload::signalImage, this, &ImageBaseWidget::handleReceiveImage);
    }
public slots:
    void handleReceiveImage(QPixmap *pixmap) {
        if(isVisible() && !pixmap->isNull()){
            m_bgPixmap = *pixmap;
            update();
            delete pixmap;
        }
    }
    void handleTimeout() {
        if(isVisible()) {
            if(HttpDownload::getInstance()->isLeisure()){
                QMetaObject::invokeMethod(HttpDownload::getInstance(), "getImage", Qt::QueuedConnection);
            }
        }
    }
protected:
    void paintEvent(QPaintEvent *event) override{
        Q_UNUSED(event);
        QPainter p(this);
        p.fillRect(rect(), Qt::black);
        p.drawPixmap(rect(), m_bgPixmap);
    }

private:
    QPixmap m_bgPixmap;
};

#endif // IMAGEBASEWIDGET_H
