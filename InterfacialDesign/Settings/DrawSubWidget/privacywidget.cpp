#include "privacywidget.h"
#include <QTimer>
#include "Network/httpdownload.h"
#include <QPainter>
#include <QFont>
#include <QMouseEvent>
#include <QDebug>

PrivacyWidget::PrivacyWidget(QWidget *parent) : QWidget(parent)
{
    rects = new QRect[4];

    QTimer *timer = new QTimer(this);
    timer->start(100);
    connect(timer, &QTimer::timeout, this, &PrivacyWidget::handlerTimeout);
    connect(HttpDownload::getInstance(), &HttpDownload::signalImage, this, &PrivacyWidget::handlerReceiveImage);

    reset();
}

PrivacyWidget::~PrivacyWidget()
{
    delete []rects;
}

void PrivacyWidget::reset()
{
    for(int i=0; i<4; i++) {
        rects[i] = QRect(0, 0, 0, 0);
    }
    currentIndex = 0;
}

void PrivacyWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);
    QPen pen;
    pen.setColor(Qt::green);
    p.setPen(pen);

    p.fillRect(rect(), Qt::black);

    p.drawPixmap(rect(), backgroundPixmap);

    for(int i=0; i<4; i++) {
        p.fillRect(rects[i], Qt::black);
        p.drawRect(rects[i]);
    }
}

void PrivacyWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) {
        startPos = event->pos();
    }
}

void PrivacyWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton && currentIndex != -1) {
        endPos = event->pos();
        rects[currentIndex].setTopLeft(QPoint(qMin(startPos.x(), endPos.x()), qMin(startPos.y(), endPos.y())));
        rects[currentIndex].setBottomRight(QPoint(qMax(startPos.x(), endPos.x()), qMax(startPos.y(), endPos.y())));
        update();
    }
}

void PrivacyWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    currentIndex++;
    if(currentIndex >= 4) {
        currentIndex = -1;
    }
}

void PrivacyWidget::handlerReceiveImage(QPixmap *pixmap)
{
    if(isVisible() && !pixmap->isNull()){
        backgroundPixmap = *pixmap;
        update();
        delete pixmap;
    }
}

void PrivacyWidget::handlerTimeout()
{
    if(isVisible()) {
        if(HttpDownload::getInstance()->isLeisure()){
            QMetaObject::invokeMethod(HttpDownload::getInstance(), "getImage", Qt::QueuedConnection);
        }
    }
}
