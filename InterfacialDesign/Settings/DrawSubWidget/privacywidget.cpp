#include "privacywidget.h"
#include <QTimer>
#include "Network/httpdownload.h"
#include <QPainter>
#include <QFont>
#include <QMouseEvent>
#include <QDebug>

PrivacyWidget::PrivacyWidget(QWidget *parent) :
    ImageBaseWidget(parent)
{
    m_rects = new QRect[4];
    reset();
}

PrivacyWidget::~PrivacyWidget()
{
    delete []m_rects;
}

void PrivacyWidget::reset()
{
    for(int i=0; i<4; i++) {
        m_rects[i] = QRect(0, 0, 0, 0);
    }
    m_currentIndex = 0;
}

void PrivacyWidget::paintEvent(QPaintEvent *event)
{
    ImageBaseWidget::paintEvent(event);
    QPainter p(this);
    QPen pen;
    pen.setColor(Qt::green);
    p.setPen(pen);

    for(int i=0; i<4; i++) {
        p.fillRect(m_rects[i], Qt::black);
        p.drawRect(m_rects[i]);
    }
}

void PrivacyWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) {
        m_startPos = event->pos();
    }
}

void PrivacyWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton && m_currentIndex != -1) {
        m_endPos = event->pos();
        m_rects[m_currentIndex].setTopLeft(QPoint(qMin(m_startPos.x(), m_endPos.x()), qMin(m_startPos.y(), m_endPos.y())));
        m_rects[m_currentIndex].setBottomRight(QPoint(qMax(m_startPos.x(), m_endPos.x()), qMax(m_startPos.y(), m_endPos.y())));
        update();
    }
}

void PrivacyWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_currentIndex++;
    if(m_currentIndex >= 4) {
        m_currentIndex = -1;
    }
}

void PrivacyWidget::fullScreen()
{
    reset();
    m_rects[m_currentIndex].setTopLeft(QPoint(0, 0));
    m_rects[m_currentIndex].setBottomRight(QPoint(width(), height()));
    update();
}

void PrivacyWidget::clearScreen()
{
    reset();
    m_rects[m_currentIndex].setTopLeft(QPoint(0, 0));
    m_rects[m_currentIndex].setBottomRight(QPoint(0, 0));
    update();
}
