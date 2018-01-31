#include "timeregionwidget.h"
#include <QPainter>
#include <QDebug>

TimeRegionWidget::TimeRegionWidget(QWidget *parent) : QWidget(parent)
{
    wordList << "周天" << "周一" << "周二" << "周三" << "周四"
             << "周五" << "周六";

    plans = new VidiconProtocol::Plan *[7];
    for(int i=0; i<7; i++) {
        plans[i] = new VidiconProtocol::Plan[4];
        plans[i]->PlanTimeEnabled = 0;
        plans[i]->BeginTime = QTime(0, 0, 0);
        plans[i]->EndTime = QTime(0, 0, 0);
    }

    for(int i=0; i<7; i++) {
        weekStateMap[i] = Qt::Unchecked;
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
        if(weekStateMap.value(i) != Qt::Checked)
            continue;
        plans[i][index].BeginTime = startTime;
        plans[i][index].EndTime = endTime;
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
    for(int i=0; i<wordList.length(); i++){
        p.drawText(QPoint(wordWidth - fontMetrics().width(wordList.at(i)),
                          numHeight*2 + ROWCOUNT / wordList.length() * GRIDSIZE * i), wordList.at(i));
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
        if(plans[day][i].PlanTimeEnabled == 0)
            continue;
        startPos = ((float)plans[day][i].BeginTime.msecsTo(QTime(0, 0, 0)) / sumTimeMsec) * sumWidth;
        endPos = ((float)plans[day][i].EndTime.msecsTo(QTime(0, 0, 0)) / sumTimeMsec) * sumWidth;

        p->fillRect(QRect(sPoint.x() + startPos,
                          sPoint.y() + day * GRIDSIZE * ROWCOUNT / 7,
                          endPos - startPos,
                          GRIDSIZE), QBrush(Qt::green));
    }
}

void TimeRegionWidget::handlerTimeChange(QTime time)
{
    QString name = sender()->objectName();
    QStringList list = name.split(' ');
    int index = list.at(2).toInt();

    for(int i=0; i<7; i++) {
        if(weekStateMap.value(i) == Qt::Checked) {
            if(list.at(3) == "start") {
                if(plans[i][index].EndTime < time)
                    continue;
                plans[i][index].BeginTime = time;
            }else if(list.at(3) == "end") {
                if(plans[i][index].BeginTime > time)
                    continue;
                plans[i][index].EndTime = time;
            }
            plans[i][index].PlanTimeEnabled = 1;
        }
    }

    update();
}

void TimeRegionWidget::handlerWeekSelectState(int state)
{
    int index = sender()->objectName().right(1).toInt();

    weekStateMap[index] = state;

    update();
}

void TimeRegionWidget::handlerTimeSelectState(int state)
{
    int index = sender()->objectName().right(1).toInt();

    for(int i=0; i<7; i++) {
        if(weekStateMap[i] == Qt::Checked) {
            plans[i][index].PlanTimeEnabled = state == Qt::Checked ? 1 : 0;
        }
    }

    update();
}
