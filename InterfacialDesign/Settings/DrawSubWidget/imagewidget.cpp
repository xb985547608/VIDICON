#include "imagewidget.h"
#include <QTimer>
#include "Network/http.h"
#include <QPainter>
#include <QFont>
#include <QMouseEvent>
#include <QDebug>

ImageWidget::ImageWidget(QWidget *parent) : QWidget(parent)
{    
    QTimer *timer = new QTimer(this);
    timer->start(100);
    connect(timer, &QTimer::timeout, this, &ImageWidget::handlerTimeout);
    connect(Http::getInstance(), &Http::signalImage, this, &ImageWidget::handlerReceiveImage);
}

void ImageWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);

    p.fillRect(rect(), Qt::black);
    p.drawPixmap(rect(), backgroundPixmap);
}

void ImageWidget::handlerReceiveImage(QPixmap pixmap)
{
    if(!pixmap.isNull()){
        backgroundPixmap = pixmap;
        update();
    }
}

void ImageWidget::handlerTimeout()
{
    if(isVisible()) {
        if(Http::getInstance()->isLeisure()){
            Http::getInstance()->getImage();
        }
    }
}