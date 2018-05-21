#ifndef SQUAREWAVE_H
#define SQUAREWAVE_H

/**
 * @brief         将监控设备上的记录计划可视化呈现
 * @author        xiaobin <xiaobin@sunniwell.com>
 * @date          20180504
 */

#include <QWidget>
#include <QList>
#include <QTime>
#include "Protocol/vidiconprotocol.h"

#define ROWCOUNT   14
#define COLUMNCOUNT   48
#define GRIDSIZE  10
#define BORDERLINEWIDTH 3
#define GRIDLINEWIDTH  1

class TimeRegionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TimeRegionWidget(QWidget *parent = nullptr);

    PlanBasic **getPlans() { return m_plans; }
    QMap<int, int> getWeeksState() { return m_weekStateMap; }

    void insertTimeRegion(int index, const QTime &startTime, const QTime &endTime);

protected:
    virtual void paintEvent(QPaintEvent *e);

    void drawTimeRegion(int day, const QPoint &sPoint, QPainter *p);
signals:

public slots:
    void handleTimeChange(QTime time);
    void handleWeekSelectState(int state);
    void handleTimeSelectState(int state);
private:
    QMap<int, int> m_weekStateMap;
    PlanBasic **m_plans;
    QStringList m_wordList;

};

#endif // SQUAREWAVE_H
