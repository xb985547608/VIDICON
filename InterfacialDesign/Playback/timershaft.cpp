#include "timershaft.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QTime>
#include <QFont>
#include <QPolygon>
#include "parsexml.h"

TimerShaft::TimerShaft(QWidget *parent) : QWidget(parent),
    stretchScale(1.0),
    isMoving(false),    
    currentPlayPos(0.1),
    isDragPlayPos(false),
    leftPos(0.0)
{
    setMouseTracking(true);
    connect(this, &TimerShaft::signalSetParameter, VidiconProtocol::getInstance(), &VidiconProtocol::handlerSetParameter);
    connect(this, &TimerShaft::signalGetParameter, VidiconProtocol::getInstance(), &VidiconProtocol::handlerGetParameter);
    connect(VidiconProtocol::getInstance(), &VidiconProtocol::signalSendData, this, &TimerShaft::handlerReceiveData);
}

void TimerShaft::drawTick(QPainter &p)
{
    p.save();

    //绘制刻度
    p.setPen(QPen(QBrush(Qt::black), TICKMAXWIDTH));
    QPointF startPos(leftPos + margin, (height - GROOVEHEIGHT) / 2 - 1);
    for(int i=0; i<=24; i++) {
        //整点
        int x = startPos.x() + i * halfHourTickInterval * 2;
        if((x >= margin) && (x <= (margin + width))){
            p.setPen(QPen());
            p.drawLine(QPointF(x,  startPos.y() - TICKMAXHEIGHT),
                       QPointF(x,  startPos.y()));
        }
        //15min
        x += halfHourTickInterval * 0.5;
        if((stretchScale >= 10.0) && (x >= margin) && (x <= (margin + width))){
            p.drawLine(QPointF(x,  startPos.y() - TICKMAXHEIGHT / 2),
                       QPointF(x,  startPos.y()));
        }
        //30min
        x += halfHourTickInterval * 0.5;
        if((stretchScale >= 4.0) && (x >= margin) && (x <= (margin + width))){
            p.drawLine(QPointF(x,  startPos.y() - TICKMAXHEIGHT / 2),
                       QPointF(x,  startPos.y()));
        }
        //45min
        x += halfHourTickInterval * 0.5;
        if((stretchScale >= 10.0) && (x >= margin) && (x <= (margin + width))){
            p.drawLine(QPointF(x,  startPos.y() - TICKMAXHEIGHT / 2),
                       QPointF(x,  startPos.y()));
        }
        x += halfHourTickInterval * 0.5;
        if(x > (margin + width)) {
            break;
        }
    }

    p.restore();
}

void TimerShaft::drawGroove(QPainter &p)
{
    p.save();

    //绘制groove
    p.setPen(QPen(Qt::NoPen));
    p.setBrush(QBrush(Qt::gray));
    QRect rect(margin, (height - GROOVEHEIGHT) / 2, width, GROOVEHEIGHT);
    p.drawRect(rect);

    p.restore();
}

void TimerShaft::drawHighlight(QPainter &p)
{
    p.save();

    p.setBrush(QBrush(Qt::green));

    p.restore();
}

void TimerShaft::drawTimeText(QPainter &p)
{
    p.save();

    QPointF startPos(leftPos + margin, (height - GROOVEHEIGHT) / 2 - 1);
    //绘制时间点
    for(int i=0; i<=24; i++) {
        //整点
        int x = startPos.x() + i * halfHourTickInterval * 2 - margin;
        if((x >= 0) && (x <= width)) {
            QString timeStr = QString("%1:00").arg(i, 2, 10, QChar('0'));
            p.drawText(QPointF(x, startPos.y() - TICKMAXHEIGHT - 3), timeStr);
        }
        //15min
        x += halfHourTickInterval * 0.5;
        if((stretchScale >= 10.0) && (x >= 0) && (x <= width)) {
            QString timeStr = QString("%1:15").arg(i, 2, 10, QChar('0'));
            p.drawText(QPointF(x, startPos.y() - TICKMAXHEIGHT / 2 - 3), timeStr);
        }
        //30min
        x += halfHourTickInterval * 0.5;
        if((stretchScale >= 4.0) && (x >= 0) && (x <= width)) {
            QString timeStr = QString("%1:30").arg(i, 2, 10, QChar('0'));
            p.drawText(QPointF(x, startPos.y() - TICKMAXHEIGHT / 2 - 3), timeStr);
        }
        //45min
        x += halfHourTickInterval * 0.5;
        if((stretchScale >= 10.0) && (x >= 0) && (x <= width)) {
            QString timeStr = QString("%1:45").arg(i, 2, 10, QChar('0'));
            p.drawText(QPointF(x, startPos.y() - TICKMAXHEIGHT / 2 - 3), timeStr);
        }
        x += halfHourTickInterval * 0.5;
        if(x > width) {
            break;
        }
    }

    p.restore();
}

void TimerShaft::drawCurTimeTag(QPainter &p)
{
    p.save();

    QRect rect(margin, (height - GROOVEHEIGHT) / 2, width, GROOVEHEIGHT);
    //绘制当前时间点标志
    p.setPen(QPen(Qt::NoPen));
    p.setBrush(Qt::red);
    int x = currentPlayPos * width + margin;
    QPolygon t;//绘制下三角
    t.append(QPoint(x, rect.bottom()));
    t.append(QPoint(x - TRIANGLEWIDTH / 2, rect.bottom() + TRIANGLEHEIGHT));
    t.append(QPoint(x + TRIANGLEWIDTH / 2, rect.bottom() + TRIANGLEHEIGHT));
    QPainterPath path;
    path.addPolygon(t);
    p.drawPath(path);

    p.restore();
}

void TimerShaft::drawInfo(QPainter &p)
{
    p.save();

    //绘制播放状态
    p.setPen(QPen(Qt::SolidLine));
    int y = height - 10;
    p.drawText(QPoint(margin, y), "Play Status:Stopped");

    //绘制提示信息
    QString strTag1("Alarm Record");
    QString strTag2("Timed Record");
    QString strTag3("Manual Record");
    int x = size().width() - (8 * fontMetrics().height() + fontMetrics().width(strTag1)
                                                     + fontMetrics().width(strTag2)
                                                     + fontMetrics().width(strTag3)) - margin;
    p.fillRect(x, y - fontMetrics().height(), fontMetrics().height(), fontMetrics().height(), Qt::red);
    x += fontMetrics().height() + fontMetrics().height();
    p.drawText(QPoint(x, y), strTag1);

    x += fontMetrics().width(strTag1) + fontMetrics().height();
    p.fillRect(x, y - fontMetrics().height(), fontMetrics().height(), fontMetrics().height(), Qt::blue);
    x += fontMetrics().height() + fontMetrics().height();
    p.drawText(QPoint(x, y), strTag2);

    x += fontMetrics().width(strTag2) + fontMetrics().height();
    p.fillRect(x, y - fontMetrics().height(), fontMetrics().height(), fontMetrics().height(), Qt::green);
    x += fontMetrics().height() + fontMetrics().height();
    p.drawText(QPoint(x, y), strTag3);

    p.restore();
}

void TimerShaft::drawFloatingFrame(QPainter &p)
{
    p.save();

    //绘制浮动框
    if(isMoving || isDragPlayPos) {
        QFont font = p.font();
        font.setPixelSize(16);
        font.setBold(true);
        p.setFont(font);
        p.setPen(QPen(QBrush(Qt::blue), 2));

        //第一次矫正保证时间百分比在可视范围内
        if(movePos.x() < margin) {
            movePos.setX(margin);
        }else if(movePos.x() > (width + margin)) {
            movePos.setX(width + margin);
        }
        //确定显示的字符串
        qreal percent = (qreal)(movePos.x() - margin + qAbs(leftPos)) / (width * stretchScale);
        QString timeStr = QTime(0, 0, 0).addSecs(86399 * percent).toString("HH:mm:ss");
        int strWidth = p.fontMetrics().width(timeStr);
        //第二次矫正保证浮动框在有效范围内显示
        if(movePos.x() > (size().width() - strWidth - margin)) {
            movePos.setX(size().width() - strWidth - margin);
        }

        p.drawText(QPoint(movePos.x(),  (height + GROOVEHEIGHT) / 2 + font.pixelSize() + 5), timeStr);
    }

    p.restore();
}

void TimerShaft::handlerReceiveData(int type, QByteArray data)
{
    switch(type) {
    case QUERYVIDEOTIMEDAY: {
        VidiconProtocol::BackUpQueryParameter param;
        TimeParamMap.clear();
        param.Type = 0;
        param.TimeParamMap = &TimeParamMap;
        if(ParseXML::getInstance()->parseBackUpQueryParameter(&param, data)) {
            for(int i=0; i<TimeParamMap.size(); i++) {
                qDebug() << TimeParamMap[i].StarTime;
            }
            qDebug() << "#TimerShaft# handlerReceiveData, ParameterType:" << type << "parse data success...";
        }else {
            qDebug() << "#TimerShaft# handlerReceiveData, ParameterType:" << type << "parse data error...";
        }
        break;
    }
    default:
        break;
    }
}

void TimerShaft::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);
//    p.setRenderHint(QPainter::Antialiasing, true);

    margin = fontMetrics().width("00:00") / 2;
    width = size().width() - margin * 2;
    height = size().height();
    halfHourTickInterval = (float)width / 48 * stretchScale;

    drawTick(p);
    drawGroove(p);
    drawHighlight(p);
    drawTimeText(p);
    drawCurTimeTag(p);
    drawInfo(p);
    drawFloatingFrame(p);
}

void TimerShaft::mousePressEvent(QMouseEvent *event)
{
    QRect rect(currentPlayPos * width + margin - TRIANGLEWIDTH / 2,
               (height + GROOVEHEIGHT) / 2,
               TRIANGLEWIDTH, TRIANGLEHEIGHT);

    //判断鼠标点击的地方是否符合拖拽行为
    if(rect.contains(event->pos())) {
        isDragPlayPos = true;
        currentPlayPos = (qreal)(event->pos().x() - margin) / width;
        setCursor(Qt::PointingHandCursor);
        update();
    }
}

void TimerShaft::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton) {
        //处理当前时间点拖拽
        if(isDragPlayPos) {
            currentPlayPos = (float)(event->pos().x() - margin) / width;
            //矫正当前时间点的位置
            if(currentPlayPos < 0.0) {
                currentPlayPos = 0.0;
            }else if(currentPlayPos > 1.0) {
                currentPlayPos = 1.0;
            }
        //处理时间轴左右移动
        }else {
            leftPos = event->pos().x() - movePos.x() + leftPos;
            //矫正时间轴左边框的位置
            if(leftPos > 0) {
                leftPos = 0;
            }else if(leftPos < -(width * stretchScale - width)) {
                leftPos = -(width * stretchScale - width);
            }
        }
        isMoving = false;
    }else {
        //判断当前鼠标位置决定是否需要显示鼠标所指时间点
        QRect rect1(margin, (height - GROOVEHEIGHT) / 2, width, GROOVEHEIGHT);
        if(rect1.contains(event->pos())) {
            isMoving = true;
        }else {
            isMoving = false;
        }
        //判断鼠标所在位置决定鼠标样式
        QRect rect2(currentPlayPos * width + margin - TRIANGLEWIDTH / 2,
                   (height + GROOVEHEIGHT) / 2,
                   TRIANGLEWIDTH, TRIANGLEHEIGHT);
        if(rect2.contains(event->pos())) {
            setCursor(Qt::PointingHandCursor);
        }else {
            setCursor(Qt::ArrowCursor);
        }
    }

    movePos = event->pos();
    update();
}

void TimerShaft::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    isDragPlayPos = false;
    isMoving = false;
    setCursor(Qt::ArrowCursor);
    update();
}

void TimerShaft::wheelEvent(QWheelEvent *event)
{
    //以鼠标位置决定锚点所在
    int anchor = movePos.x() - margin;
    if(anchor < 0) {
        anchor = 0;
    }else if(anchor > width) {
        anchor = width;
    }
    //转换前锚点所在的百分比位置
    qreal anchorPercent = (qAbs(leftPos) + anchor) / stretchScale / width;

    //处理拉伸因子的增减
    QPoint scrollAmount = event->angleDelta();
    stretchScale = scrollAmount.y() > 0 ? (stretchScale + ZOOMDELTA) : (stretchScale - ZOOMDELTA);

    //矫正拉伸因子和时间轴左边框位置
    if(stretchScale < 1.0) {
        stretchScale = 1.0;
        leftPos = 0.0;
    }else {
        leftPos = anchor - stretchScale * width * anchorPercent;
        if(leftPos > 0) {
            leftPos = 0;
        }else if(leftPos < -(width * stretchScale - width)) {
            leftPos = -(width * stretchScale - width);
        }
    }
    update();
}

bool TimerShaft::event(QEvent *event)
{
    if(event->type() == QEvent::Leave) {
        isMoving = false;
        isDragPlayPos = false;
        setCursor(Qt::ArrowCursor);
        update();
    }
    return QWidget::event(event);
}
