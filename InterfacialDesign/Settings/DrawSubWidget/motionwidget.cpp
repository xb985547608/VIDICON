#include "motionwidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QApplication>
#include "Network/httpdownload.h"
#include <QMetaObject>

MotionWidget::MotionWidget(QWidget *parent) :
    ImageBaseWidget(parent)
{
    m_motionRegionMap = new char *[REGIONROW];
    for(int i=0; i<REGIONROW; i++) {
        m_motionRegionMap[i] = new char[REGIONCOLUMN];
    }
}

void MotionWidget::handleFullScreen()
{
    for(int i=0; i<REGIONROW; i++){
        for(int j=0; j<REGIONCOLUMN; j++){
            m_motionRegionMap[i][j] = 1;
        }
    }
}

void MotionWidget::handleCleanScreen()
{
    for(int i=0; i<REGIONROW; i++){
        for(int j=0; j<REGIONCOLUMN; j++){
            m_motionRegionMap[i][j] = 0;
        }
    }
}

void MotionWidget::paintEvent(QPaintEvent *event)
{
    ImageBaseWidget::paintEvent(event);

    QPainter p(this);
    QPen pen;

    float gridWidth = (float)size().width() / REGIONCOLUMN;
    float gridHeight = (float)size().height() / REGIONROW;

    for(int i=0; i<REGIONROW; i++){
        float xStart = 0;
        float yStart = 0 + i * gridHeight;
        for(int j=0; j<REGIONCOLUMN; j++){
            QRectF rect(xStart + j * gridWidth, yStart, gridWidth, gridHeight);
            if(m_motionRegionMap[i][j] == 1){
                pen.setColor(Qt::blue);
                p.setPen(pen);
                p.drawRect(rect);
            }else if(m_motionRegionMap[i][j] == 0){
//                pen.setColor(Qt::red);
//                p.setPen(pen);
//                if(motionRegionMap[i+1][j] == 0 && ((i+1) < REGIONROW)){
//                    p.drawLine(rect.bottomLeft(), rect.bottomRight());
//                }
//                if(motionRegionMap[i-1][j] == 0 && ((i-1) >= 0)){
//                    p.drawLine(rect.topLeft(), rect.topRight());
//                }
//                if(motionRegionMap[i][j+1] == 0 && ((j+1) < REGIONCOLUMN)){
//                    p.drawLine(rect.topRight(), rect.bottomRight());
//                }
//                if(motionRegionMap[i][j-1] == 0 && ((j-1) >= 0)){
//                    p.drawLine(rect.topLeft(), rect.bottomLeft());
//                }
            }
        }
    }
}

void MotionWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        m_validPressLeft = true;
    }else   if(event->button() == Qt::RightButton){
        m_validPressRight = true;
    }
    m_pressPos = event->pos();
}

void MotionWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(m_validPressLeft || m_validPressRight){
        int gridWidth = size().width() / REGIONCOLUMN;
        int gridHeight = size().height() / REGIONROW;
        int row = event->pos().y() / gridHeight;
        int column = event->pos().x() / gridWidth;

        if(row < 0 || row >= REGIONROW || column < 0 || column >= REGIONCOLUMN)
            return;

        if(m_validPressLeft){
            m_motionRegionMap[row][column] = 1;
            update();
        }else if(m_validPressRight){
            m_motionRegionMap[row][column] = 0;
            update();
        }

    }
}

void MotionWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        m_validPressLeft = false;
    }else   if(event->button() == Qt::RightButton){
        m_validPressRight = false;
    }
}
