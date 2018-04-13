#include "osdwidget.h"
#include "parsexml.h"
#include <QTimer>
#include "Network/httpdownload.h"
#include <QPainter>
#include <QFont>
#include <QMouseEvent>
#include <QDebug>

OSDWidget::OSDWidget(QWidget *parent) : QWidget(parent) ,
    m_validPress(false)
{
    QFont f = font();
    f.setPixelSize(16);
    setFont(f);

    OSDParameter temp;
    for (int i=0; i<4; i++)
        m_parameters.append(temp);

    QTimer *timer = new QTimer(this);
    timer->start(100);
    connect(timer, &QTimer::timeout, this, &OSDWidget::handleTimeout);
    connect(HttpDownload::getInstance(), &HttpDownload::signalImage, this, &OSDWidget::handleReceiveImage);
}

OSDWidget::~OSDWidget()
{
}

void OSDWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);
    QPen pen;

    p.fillRect(rect(), Qt::black);
    p.drawPixmap(rect(), m_backgroundPixmap);

    for(int i=0; i<m_parameters.count(); i++) {
        QRect rect(m_parameters[i].x * size().width() / XSCALEMAX,
                   m_parameters[i].y * size().height() / YSCALEMAX,
                   size().width() / 2, fontMetrics().height() + 4);
        pen.setColor(Qt::yellow);
        p.setPen(pen);
        p.drawRect(rect);
        pen.setColor(Qt::red);
        p.setPen(pen);
        rect.adjust(4, 0, 0, -2);
        switch(m_parameters[i].OSDType) {
        case 0: {
            p.drawText(rect.bottomLeft() , m_parameters[i].OSDText);
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
            p.drawText(rect.bottomLeft() , m_parameters[i].OSDText);
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
        for(int i=0; i<m_parameters.count(); i++) {
            QRect rect(m_parameters[i].x * size().width() / XSCALEMAX,
                       m_parameters[i].y * size().height() / YSCALEMAX,
                       size().width() / 2, fontMetrics().height() + 4);
            if(rect.contains(event->pos())) {
                m_validPress = true;
                m_lastPos = event->pos() - rect.topLeft();
                m_currentMoveIndex = i;
            }
        }
    }
}

void OSDWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(m_validPress){
        QRect rect(m_parameters[m_currentMoveIndex].x * size().width() / XSCALEMAX,
                   m_parameters[m_currentMoveIndex].y * size().height() / YSCALEMAX,
                   size().width() / 2, fontMetrics().height() + 4);
        rect.moveTo(event->pos() - m_lastPos);
        m_parameters[m_currentMoveIndex].x = rect.x() * XSCALEMAX / size().width();
        m_parameters[m_currentMoveIndex].y = rect.y() * YSCALEMAX / size().height();

        update();
    }
}

void OSDWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_validPress = false;
}

void OSDWidget::handleReceiveImage(QPixmap *pixmap)
{
    if(isVisible() && !pixmap->isNull()){
        m_backgroundPixmap = *pixmap;
        update();
        delete pixmap;
    }
}

void OSDWidget::handleTimeout()
{
    if(isVisible()) {
        if(HttpDownload::getInstance()->isLeisure()){
            QMetaObject::invokeMethod(HttpDownload::getInstance(), "getImage", Qt::QueuedConnection);
        }
    }
}
