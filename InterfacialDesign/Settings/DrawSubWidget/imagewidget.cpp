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
    connect(timer, &QTimer::timeout, this, &ImageWidget::handleTimeout);
    connect(HttpDownload::getInstance(), &HttpDownload::signalImage, this, &ImageWidget::handleReceiveImage);
}

void ImageWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);

    p.fillRect(rect(), Qt::black);
    p.drawPixmap(rect(), m_backgroundPixmap);
}

void ImageWidget::handleReceiveImage(QPixmap *pixmap)
{
    if(isVisible() && !pixmap->isNull()){
        m_backgroundPixmap = *pixmap;
        update();
        delete pixmap;
    }
}

void ImageWidget::handleTimeout()
{
    if(isVisible()) {
        if(HttpDownload::getInstance()->isLeisure()){
            QMetaObject::invokeMethod(HttpDownload::getInstance(), "getImage", Qt::QueuedConnection);
        }
    }
}
