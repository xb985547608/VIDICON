#include "timershaft.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QTime>
#include <QFont>
#include <QPolygon>
#include "parsexml.h"

TimerShaft::TimerShaft(int htmlid, QWidget *parent) : BasicWidget(parent),
    m_stretchScale(1.0),
    m_isPress(false),
    m_isMoving(false),    
    m_currentPlayPos(0.1),
    m_isDragPlayPos(false),
    m_leftPos(0.0),
    m_date(QDate::currentDate()),
    m_htmlid(htmlid)
{
    setMouseTracking(true);
}

void TimerShaft::drawTick(QPainter &p)
{
    p.save();

    //绘制刻度
    p.setPen(QPen(QBrush(Qt::black), TICKMAXWIDTH));
    QPointF startPos(m_leftPos + m_margin, (m_height - GROOVEHEIGHT) / 2 - 1);
    for(int i=0; i<=24; i++) {
        //整点
        qreal x = startPos.x() + i * m_halfHourTickInterval * 2;
        if((x >= m_margin) && (x <= (m_margin + m_width + 2))){
            p.setPen(QPen());
            p.drawLine(QPointF(x,  startPos.y() - TICKMAXHEIGHT),
                       QPointF(x,  startPos.y()));
        }
        //15min
        x += m_halfHourTickInterval * 0.5;
        if((m_stretchScale >= 10.0) && (x >= m_margin) && (x <= (m_margin + m_width))){
            p.drawLine(QPointF(x,  startPos.y() - TICKMAXHEIGHT / 2),
                       QPointF(x,  startPos.y()));
        }
        //30min
        x += m_halfHourTickInterval * 0.5;
        if((m_stretchScale >= 4.0) && (x >= m_margin) && (x <= (m_margin + m_width))){
            p.drawLine(QPointF(x,  startPos.y() - TICKMAXHEIGHT / 2),
                       QPointF(x,  startPos.y()));
        }
        //45min
        x += m_halfHourTickInterval * 0.5;
        if((m_stretchScale >= 10.0) && (x >= m_margin) && (x <= (m_margin + m_width))){
            p.drawLine(QPointF(x,  startPos.y() - TICKMAXHEIGHT / 2),
                       QPointF(x,  startPos.y()));
        }
//        x += halfHourTickInterval * 0.5;
//        if(x > (margin + width)) {
//            break;
//        }
    }

    p.restore();
}

void TimerShaft::drawGroove(QPainter &p)
{
    p.save();

    //绘制groove
    p.setPen(QPen(Qt::NoPen));
    p.setBrush(QColor("#aab3b6"));
    QRect rect(m_margin, (m_height - GROOVEHEIGHT) / 2, m_width, GROOVEHEIGHT);
    p.drawRect(rect);

    p.restore();
}

void TimerShaft::drawHighlight(QPainter &p)
{
    p.save();
    p.setPen(Qt::NoPen);
    p.setBrush(QBrush(Qt::green));

    for (int i=0; i<m_timeParamMap.count(); i++) {
        int leftSec = qAbs(m_timeParamMap[i].StarTime.secsTo(QTime(0, 0, 0)));
        qreal left = leftSec / HALFHOURSEC * m_halfHourTickInterval;
        left += leftSec % HALFHOURSEC * m_halfHourTickInterval / HALFHOURSEC + m_margin;

        int rightSec = qAbs(m_timeParamMap[i].EndTime.secsTo(QTime(0, 0, 0)));
        qreal right = rightSec / HALFHOURSEC * m_halfHourTickInterval;
        right += rightSec % HALFHOURSEC * m_halfHourTickInterval / HALFHOURSEC + m_margin;

        left -= qAbs(m_leftPos);
        right -= qAbs(m_leftPos);

        left = qMax((int)left, m_margin);
        right = qMin((int)right, m_width + m_margin);

        if(left >= right)
            continue;

        QRectF rect(left, (m_height - GROOVEHEIGHT) / 2, right - left, GROOVEHEIGHT);
        p.drawRect(rect);
    }

    p.restore();
}

void TimerShaft::drawTimeText(QPainter &p)
{
    p.save();

    QPointF startPos(m_leftPos + m_margin, (m_height - GROOVEHEIGHT) / 2 - 1);
    //绘制时间点
    for (int i=0; i<=24; i++) {
        //整点
        qreal x = startPos.x() + i * m_halfHourTickInterval * 2 - m_margin;
        if ((x >= 0) && (x <= (m_width + 2))) {
            QString timeStr = QString("%1:00").arg(i, 2, 10, QChar('0'));
            p.drawText(QPointF(x, startPos.y() - TICKMAXHEIGHT - 3), timeStr);
        }
        //15min
        x += m_halfHourTickInterval * 0.5;
        if ((m_stretchScale >= 10.0) && (x >= 0) && (x <= m_width)) {
            QString timeStr = QString("%1:15").arg(i, 2, 10, QChar('0'));
            p.drawText(QPointF(x, startPos.y() - TICKMAXHEIGHT / 2 - 3), timeStr);
        }
        //30min
        x += m_halfHourTickInterval * 0.5;
        if ((m_stretchScale >= 4.0) && (x >= 0) && (x <= m_width)) {
            QString timeStr = QString("%1:30").arg(i, 2, 10, QChar('0'));
            p.drawText(QPointF(x, startPos.y() - TICKMAXHEIGHT / 2 - 3), timeStr);
        }
        //45min
        x += m_halfHourTickInterval * 0.5;
        if ((m_stretchScale >= 10.0) && (x >= 0) && (x <= m_width)) {
            QString timeStr = QString("%1:45").arg(i, 2, 10, QChar('0'));
            p.drawText(QPointF(x, startPos.y() - TICKMAXHEIGHT / 2 - 3), timeStr);
        }
//        x += halfHourTickInterval * 0.5;
//        if ((int)x > width) {
//            break;
//        }
    }

    p.restore();
}

void TimerShaft::drawCurTimeTag(QPainter &p)
{
    p.save();

    QRect rect(m_margin, (m_height - GROOVEHEIGHT) / 2, m_width, GROOVEHEIGHT);
    //绘制当前时间点标志
    p.setPen(QPen(Qt::NoPen));
    p.setBrush(Qt::red);
    int x = m_currentPlayPos * m_width + m_margin;
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
    int y = m_height - 10;
    QString strStatus = QString("Play Status: %1").arg(m_isPlaying ? "Playing" : "Stopped");
//    p.drawText(QPoint(margin, y), strStatus);

    //绘制提示信息
    QString strTag1("Alarm Record");
    QString strTag2("Timed Record");
    QString strTag3("Manual Record");
    int x = size().width() - (8 * fontMetrics().height() + fontMetrics().width(strTag1)
                                                     + fontMetrics().width(strTag2)
                                                     + fontMetrics().width(strTag3)) - m_margin;
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
    if (m_isMoving || m_isDragPlayPos) {
        QFont font = p.font();
        font.setPixelSize(16);
        font.setBold(true);
        p.setFont(font);

        //第一次矫正保证时间百分比在可视范围内
        if (m_movePos.x() < m_margin) {
            m_movePos.setX(m_margin);
        } else if (m_movePos.x() > (m_width + m_margin)) {
            m_movePos.setX(m_width + m_margin);
        }
        //绘制指示线
        p.setPen(QPen(Qt::black, 1, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));
        p.drawLine(QPoint(m_movePos.x(), (m_height + GROOVEHEIGHT) / 2), QPoint(m_movePos.x(), 0));
        //确定显示的字符串
        p.setPen(QPen(QBrush(Qt::blue), 2));
        int secs = (m_movePos.x() - m_margin + qAbs(m_leftPos)) * ONEDAYSEC / (m_width * m_stretchScale);
        QString timeStr = QTime(0, 0, 0).addSecs(secs).toString("HH:mm:ss");
        int strWidth = p.fontMetrics().width(timeStr);
        //第二次矫正保证浮动框在有效范围内显示
        if(m_movePos.x() > (size().width() - strWidth - m_margin)) {
            m_movePos.setX(size().width() - strWidth - m_margin);
        }

        p.drawText(QPoint(m_movePos.x(),  (m_height + GROOVEHEIGHT) / 2 + font.pixelSize() + 5), timeStr);
    }

    p.restore();
}

void TimerShaft::handleReceiveData(VidiconProtocol::Type type, QByteArray data)
{
    bool isOK = false;

    switch(type) {
    case VidiconProtocol::QUERYVIDEOTIMEDAY: {
        BackUpQueryParameter param;
        m_timeParamMap.clear();
        param.Type = 0;
        isOK = ParseXML::getInstance()->parseBackUpQueryParameter(&param, data);
        if (isOK) {
            m_timeParamMap = param.TimeParamMap;
            update();
        }
        break;
    }
    default:
        return;
    }

    if (isOK)
        qDebug() << "#TimerShaft# handleReceiveData, ParameterType:" << type << "parse data success...";
    else
        qDebug() << "#TimerShaft# handleReceiveData, ParameterType:" << type << "parse data error...";
}

void TimerShaft::hanlderDateChange(QDate date)
{
    this->m_date = date;
    qDebug() << "#TimerShaft# hanlderDateChange," << this->m_date;
}

void TimerShaft::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);
//    p.setRenderHint(QPainter::Antialiasing, true);

    m_margin = fontMetrics().width("00:00") / 2;
    m_width = size().width() - m_margin * 2;
    m_height = size().height();
    m_halfHourTickInterval = (qreal)m_width / 48 * m_stretchScale;

    drawTick(p);
    drawGroove(p);
    drawHighlight(p);
    drawTimeText(p);
//    drawCurTimeTag(p);
    drawInfo(p);
    drawFloatingFrame(p);
}

void TimerShaft::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    QRect rect(m_margin, 0, m_width, (m_height + GROOVEHEIGHT) / 2);
    if(event->buttons() & Qt::LeftButton && rect.contains(event->pos())) {
        m_isPress = true;
    }
}

void TimerShaft::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        //处理当前时间点拖拽
        if (m_isDragPlayPos) {
            m_currentPlayPos = (qreal)(event->pos().x() - m_margin) / m_width;
            //矫正当前时间点的位置
            if (m_currentPlayPos < 0.0) {
                m_currentPlayPos = 0.0;
            } else if (m_currentPlayPos > 1.0) {
                m_currentPlayPos = 1.0;
            }
        //处理时间轴左右移动
        } else {
            m_leftPos = event->pos().x() - m_movePos.x() + m_leftPos;
            //矫正时间轴左边框的位置
            if (m_leftPos > 0) {
                m_leftPos = 0;
            } else if (m_leftPos < -(m_width * m_stretchScale - m_width)) {
                m_leftPos = -(m_width * m_stretchScale - m_width);
            }
        }
        m_isMoving = false;
    } else {
        //判断当前鼠标位置决定是否需要显示鼠标所在位置的时间点
        QRect rect1(m_margin, 0, m_width, (m_height + GROOVEHEIGHT) / 2);
        if (rect1.contains(event->pos())) {
            m_isMoving = true;
        } else {
            m_isMoving = false;
        }
    }

    m_movePos = event->pos();
    update();
}

void TimerShaft::mouseReleaseEvent(QMouseEvent *event)
{
    QRect rect(m_margin, 0, m_width, (m_height + GROOVEHEIGHT) / 2);
    if (m_isPress && rect.contains(event->pos())) {
        checkStartPlayTime(event->pos());
    }

    m_isDragPlayPos = false;
    m_isMoving = false;
    m_isPress = false;
    update();
}

void TimerShaft::wheelEvent(QWheelEvent *event)
{
    //以鼠标位置决定锚点所在
    int anchor = m_movePos.x() - m_margin;
    if (anchor < 0) {
        anchor = 0;
    } else if (anchor > m_width) {
        anchor = m_width;
    }
    //转换前锚点所在的百分比位置
    qreal anchorPercent = (qAbs(m_leftPos) + anchor) / (m_stretchScale * m_width);

    //处理拉伸因子的增减
    QPoint scrollAmount = event->angleDelta();
    m_stretchScale = scrollAmount.y() > 0 ? (m_stretchScale + ZOOMDELTA) : (m_stretchScale - ZOOMDELTA);

    //矫正拉伸因子和时间轴左边框位置
    if (m_stretchScale <= 1.0) {
        m_stretchScale = 1.0;
        m_leftPos = 0.0;
    } else {
        m_leftPos = anchor - m_stretchScale * m_width * anchorPercent;
        if (m_leftPos >= 0) {
            m_leftPos = 0;
        } else if (m_leftPos < -(m_width * m_stretchScale - m_width)) {
            m_leftPos = -(m_width * m_stretchScale - m_width);
        }
    }
    update();
}

bool TimerShaft::event(QEvent *event)
{
    if(event->type() == QEvent::Leave) {
        m_isMoving = false;
        m_isDragPlayPos = false;
        setCursor(Qt::ArrowCursor);
        update();
    }
    return QWidget::event(event);
}

//类似模糊匹配的功能，在边界点的边缘处自动对齐边界点
void TimerShaft::checkStartPlayTime(QPoint pos)
{
    int diffSecs = DIFFVALUE * ONEDAYSEC / m_stretchScale / m_width;
    int secs = (pos.x() - m_margin + qAbs(m_leftPos)) * ONEDAYSEC / (m_width * m_stretchScale);
    QTime posTime = QTime(0, 0, 0).addSecs(secs);
    QTime leftTime = QTime(0, 0, 0).addSecs(secs - diffSecs);
    QTime rightTime = QTime(0, 0, 0).addSecs(secs + diffSecs);

    TimeParameter temp;
    bool isOK = false;
    QTime playTime;
    for(int i=0; i<m_timeParamMap.count(); i++) {
        temp = m_timeParamMap.value(i);
        if(rightTime < temp.StarTime || leftTime > temp.EndTime) {
            continue;
        }else if(temp.StarTime > leftTime && temp.StarTime < rightTime) {
            playTime = temp.StarTime;
            isOK = true;
        }else if(temp.EndTime > leftTime && temp.EndTime < rightTime) {
            playTime = temp.EndTime;
            isOK = true;
        }else if(temp.EndTime > posTime && temp.StarTime < posTime) {
            playTime = posTime;
            isOK = true;
        }

        if(isOK) {
            qDebug() << "#checkStartPlayTime# play time" << playTime;
            m_currentPlayPos = (qreal)(pos.x() - m_margin) / m_width;
            StartPlayingParameter *param = new StartPlayingParameter;
            param->htmlid = m_htmlid;
            param->playing = 1;
            param->Time = QDateTime(m_date, playTime);
            emit signalSetParameter(VidiconProtocol::STARTPLAYING, param);
            break;
        }
        qDebug() << temp.StarTime << temp.EndTime;
    }

}
