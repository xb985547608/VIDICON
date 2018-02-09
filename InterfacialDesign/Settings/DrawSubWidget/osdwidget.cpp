#include "osdwidget.h"
#include "parsexml.h"
#include <QTimer>
#include "Network/httpdownload.h"
#include <QPainter>
#include <QFont>
#include <QMouseEvent>
#include <QDebug>

OSDWidget::OSDWidget(QWidget *parent) : QWidget(parent) ,
    validPress(false)
{
    QFont f = font();
    f.setPixelSize(16);
    setFont(f);

    parameters = new VidiconProtocol::OSDParameter[4];

    QTimer *timer = new QTimer(this);
    timer->start(100);
    connect(timer, &QTimer::timeout, this, &OSDWidget::handlerTimeout);
    connect(HttpDownload::getInstance(), &HttpDownload::signalImage, this, &OSDWidget::handlerReceiveImage);
}

OSDWidget::~OSDWidget()
{
    delete []parameters;
}

void OSDWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);
    QPen pen;

    p.fillRect(rect(), Qt::black);
    p.drawPixmap(rect(), backgroundPixmap);

    for(int i=0; i<4; i++) {
        QRect rect(parameters[i].x * size().width() / XSCALEMAX,
                   parameters[i].y * size().height() / YSCALEMAX,
                   size().width() / 2, fontMetrics().height() + 4);
        pen.setColor(Qt::yellow);
        p.setPen(pen);
        p.drawRect(rect);
        pen.setColor(Qt::red);
        p.setPen(pen);
        rect.adjust(4, 0, 0, -2);
        switch(parameters[i].OSDType) {
        case 0: {
            p.drawText(rect.bottomLeft() , parameters[i].OSDText);
            break;
        }
        case 1: {
            p.drawText(rect.bottomLeft() , "Show Clock");
            break;
        }
        case 2: {
            p.drawText(rect.bottomLeft() , "Show FPS");
            break;
        }
        case 3: {
            p.drawText(rect.bottomLeft() , parameters[i].OSDText);
            break;
        }
        default:
            break;
        }
    }
}

void OSDWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) {
        for(int i=0; i<4; i++) {
            QRect rect(parameters[i].x * size().width() / XSCALEMAX,
                       parameters[i].y * size().height() / YSCALEMAX,
                       size().width() / 2, fontMetrics().height() + 4);
            if(rect.contains(event->pos())) {
                validPress = true;
                diffValue = event->pos() - rect.topLeft();
                currentMoveIndex = i;
            }
        }
    }
}

void OSDWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(validPress){
        QRect rect(parameters[currentMoveIndex].x * size().width() / XSCALEMAX,
                   parameters[currentMoveIndex].y * size().height() / YSCALEMAX,
                   size().width() / 2, fontMetrics().height() + 4);
        qDebug() << rect;
        rect.moveTo(event->pos() - diffValue);
        qDebug() << rect;
        parameters[currentMoveIndex].x = rect.x() * XSCALEMAX / size().width();
        parameters[currentMoveIndex].y = rect.y() * YSCALEMAX / size().height();

        update();
    }
}

void OSDWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    validPress = false;
}

void OSDWidget::handlerReceiveImage(QPixmap pixmap)
{
    if(!pixmap.isNull()){
        backgroundPixmap = pixmap;
        update();
    }
}

void OSDWidget::handlerTimeout()
{
    if(isVisible()) {
        if(HttpDownload::getInstance()->isLeisure()){
            HttpDownload::getInstance()->getImage();
        }
    }
}
