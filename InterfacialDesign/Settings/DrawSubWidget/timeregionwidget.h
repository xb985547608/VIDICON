#ifndef SQUAREWAVE_H
#define SQUAREWAVE_H

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

    VidiconProtocol::Plan **getPlans() { return plans; }
    QMap<int, int> getWeeksState() { return weekStateMap; }

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
    QMap<int, int> weekStateMap;
    VidiconProtocol::Plan **plans;
    QStringList wordList;

};

#endif // SQUAREWAVE_H
