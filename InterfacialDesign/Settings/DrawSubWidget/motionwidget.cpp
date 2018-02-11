#include "motionwidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QApplication>
#include "Network/httpdownload.h"
#include <QMetaObject>

MotionWidget::MotionWidget(QWidget *parent) : QWidget(parent)
{
    motionRegionMap = new char *[REGIONROW];
    for(int i=0; i<REGIONROW; i++) {
        motionRegionMap[i] = new char[REGIONCOLUMN];
    }

    timer = new QTimer(this);
    timer->start(100);
    connect(timer, &QTimer::timeout, this, &MotionWidget::handlerTimeout);
    connect(HttpDownload::getInstance(), &HttpDownload::signalImage, this, &MotionWidget::handlerReceiveImage);
}

void MotionWidget::handlerFullScreen()
{
    for(int i=0; i<REGIONROW; i++){
        for(int j=0; j<REGIONCOLUMN; j++){
            motionRegionMap[i][j] = 1;
        }
    }
}

void MotionWidget::handlerCleanScreen()
{
    for(int i=0; i<REGIONROW; i++){
        for(int j=0; j<REGIONCOLUMN; j++){
            motionRegionMap[i][j] = 0;
        }
    }
}

void MotionWidget::handlerReceiveImage(QPixmap *pixmap)
{
    if(isVisible() && !pixmap->isNull()){
        backgroundPixmap = *pixmap;
        update();
        delete pixmap;
    }
}

void MotionWidget::handlerTimeout()
{
    if(isVisible()){
        if(HttpDownload::getInstance()->isLeisure()){
            QMetaObject::invokeMethod(HttpDownload::getInstance(), "getImage", Qt::QueuedConnection);
        }
    }
}

void MotionWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);
    QPen pen;

    float gridWidth = (float)size().width() / REGIONCOLUMN;
    float gridHeight = (float)size().height() / REGIONROW;

    p.fillRect(rect(), Qt::black);
    p.drawPixmap(rect(), backgroundPixmap);

    for(int i=0; i<REGIONROW; i++){
        float xStart = 0;
        float yStart = 0 + i * gridHeight;
        for(int j=0; j<REGIONCOLUMN; j++){
            QRectF rect(xStart + j * gridWidth, yStart, gridWidth, gridHeight);
            if(motionRegionMap[i][j] == 1){
                pen.setColor(Qt::blue);
                p.setPen(pen);
                p.drawRect(rect);
            }else if(motionRegionMap[i][j] == 0){
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
        validPressLeft = true;
    }else   if(event->button() == Qt::RightButton){
        validPressRight = true;
    }
    pressPos = event->pos();
}

void MotionWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(validPressLeft || validPressRight){
        int gridWidth = size().width() / REGIONCOLUMN;
        int gridHeight = size().height() / REGIONROW;
        int row = event->pos().y() / gridHeight;
        int column = event->pos().x() / gridWidth;

        if(row < 0 || row >= REGIONROW || column < 0 || column >= REGIONCOLUMN)
            return;

        if(validPressLeft){
            motionRegionMap[row][column] = 1;
            update();
        }else if(validPressRight){
            motionRegionMap[row][column] = 0;
            update();
        }

    }
}

void MotionWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        validPressLeft = false;
    }else   if(event->button() == Qt::RightButton){
        validPressRight = false;
    }
}
