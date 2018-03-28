#include "timeregionwidget.h"
#include <QPainter>
#include <QDebug>

TimeRegionWidget::TimeRegionWidget(QWidget *parent) : QWidget(parent)
{
    m_wordList << "周天" << "周一" << "周二" << "周三" << "周四"
             << "周五" << "周六";

    m_plans = new PlanBasic *[7];
    for(int i=0; i<7; i++) {
        m_plans[i] = new PlanBasic[4];
        m_plans[i]->PlanTimeEnabled = 0;
        m_plans[i]->BeginTime = QTime(0, 0, 0);
        m_plans[i]->EndTime = QTime(0, 0, 0);
    }

    for(int i=0; i<7; i++) {
        m_weekStateMap[i] = Qt::Unchecked;
    }

    resize(550, 160);
    setFixedSize(550, 160);
}

void TimeRegionWidget::insertTimeRegion(int index, const QTime &startTime, const QTime &endTime)
{
    if(index < 0 || index > 3)
        return;

    if(startTime > endTime)
        return;
    for(int i=0; i<7; i++){
        if(m_weekStateMap.value(i) != Qt::Checked)
            continue;
        m_plans[i][index].BeginTime = startTime;
        m_plans[i][index].EndTime = endTime;
    }

    update();
}

void TimeRegionWidget::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QPainter p(this);
    QPen pen;

    pen.setColor(Qt::black);
    pen.setWidth(1);
    p.setPen(pen);
    int wordWidth = fontMetrics().width("Wednesday");
    int numHeight = fontMetrics().height();
    //绘制数字
    for(int i=0; i<=24; i++){
        p.drawText(QPoint(wordWidth + COLUMNCOUNT / 24 * GRIDSIZE * i,
                          numHeight), QString::number(i));
    }
    //绘制单词
    for(int i=0; i<m_wordList.length(); i++){
        p.drawText(QPoint(wordWidth - fontMetrics().width(m_wordList.at(i)),
                          numHeight*2 + ROWCOUNT / m_wordList.length() * GRIDSIZE * i), m_wordList.at(i));
    }
    wordWidth += 3;
    numHeight += 3;

    //绘制时间段区域
    for(int i=0; i<7; i++){
        drawTimeRegion(i, QPoint(wordWidth, numHeight), &p);
    }

    //绘制外框
    pen.setColor(Qt::black);
    pen.setWidth(BORDERLINEWIDTH);
    p.setPen(pen);
    int borderWidth = COLUMNCOUNT * GRIDSIZE;
    int borderHeight = ROWCOUNT * GRIDSIZE;
    p.drawRect(QRect(wordWidth, numHeight, borderWidth, borderHeight));

    //绘制网格线
    pen.setColor(Qt::black);
    pen.setWidth(GRIDLINEWIDTH);
    p.setPen(pen);
    int xStart = wordWidth;
    int yStart = numHeight;
    for(int offset=0; offset<borderWidth; offset+=GRIDSIZE){
        p.drawLine(QPoint(xStart + offset, yStart),
                   QPoint(xStart + offset, yStart + borderHeight));
    }
    for(int offset=0; offset<borderHeight; offset+=GRIDSIZE){
        p.drawLine(QPoint(xStart, yStart + offset),
                   QPoint(xStart + borderWidth, yStart + offset));
    }

}

void TimeRegionWidget::drawTimeRegion(int day, const QPoint &sPoint, QPainter *p)
{
    int startPos, endPos;
    int sumTimeMsec = QTime(23, 59, 59).msecsTo(QTime(0, 0, 0));
    int sumWidth = COLUMNCOUNT * GRIDSIZE;

    for(int i=0; i<4; i++){
        if(m_plans[day][i].PlanTimeEnabled == 0)
            continue;
        startPos = ((float)m_plans[day][i].BeginTime.msecsTo(QTime(0, 0, 0)) / sumTimeMsec) * sumWidth;
        endPos = ((float)m_plans[day][i].EndTime.msecsTo(QTime(0, 0, 0)) / sumTimeMsec) * sumWidth;

        p->fillRect(QRect(sPoint.x() + startPos,
                          sPoint.y() + day * GRIDSIZE * ROWCOUNT / 7,
                          endPos - startPos,
                          GRIDSIZE), QBrush(Qt::green));
    }
}

void TimeRegionWidget::handleTimeChange(QTime time)
{
    QString name = sender()->objectName();
    QStringList list = name.split(' ');
    int index = list.at(2).toInt();

    for(int i=0; i<7; i++) {
        if(m_weekStateMap.value(i) == Qt::Checked) {
            if(list.at(3) == "start") {
                if(m_plans[i][index].EndTime < time)
                    continue;
                m_plans[i][index].BeginTime = time;
            }else if(list.at(3) == "end") {
                if(m_plans[i][index].BeginTime > time)
                    continue;
                m_plans[i][index].EndTime = time;
            }
            m_plans[i][index].PlanTimeEnabled = 1;
        }
    }

    update();
}

void TimeRegionWidget::handleWeekSelectState(int state)
{
    int index = sender()->objectName().right(1).toInt();

    m_weekStateMap[index] = state;

    update();
}

void TimeRegionWidget::handleTimeSelectState(int state)
{
    int index = sender()->objectName().right(1).toInt();

    for(int i=0; i<7; i++) {
        if(m_weekStateMap[i] == Qt::Checked) {
            m_plans[i][index].PlanTimeEnabled = state == Qt::Checked ? 1 : 0;
        }
    }

    update();
}
