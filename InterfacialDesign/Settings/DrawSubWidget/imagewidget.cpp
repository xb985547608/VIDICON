#include "imagewidget.h"
#include <QTimer>
#include "Network/httpdownload.h"
#include <QPainter>
#include <QFont>
#include <QMouseEvent>
#include <QDebug>
#include <QMetaObject>

ImageWidget::ImageWidget(QWidget *parent) : QWidget(parent)
{    
    QTimer *timer = new QTimer(this);
    timer->start(100);
    connect(timer, &QTimer::timeout, this, &ImageWidget::handlerTimeout);
    connect(HttpDownload::getInstance(), &HttpDownload::signalImage, this, &ImageWidget::handlerReceiveImage);
}

void ImageWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);

    p.fillRect(rect(), Qt::black);
    p.drawPixmap(rect(), backgroundPixmap);
}

void ImageWidget::handlerReceiveImage(QPixmap *pixmap)
{
    if(isVisible() && !pixmap->isNull()){
        backgroundPixmap = *pixmap;
        update();
        delete pixmap;
    }
}

void ImageWidget::handlerTimeout()
{
    if(isVisible()) {
        if(HttpDownload::getInstance()->isLeisure()){
            QMetaObject::invokeMethod(HttpDownload::getInstance(), "getImage", Qt::QueuedConnection);
        }
    }
}
