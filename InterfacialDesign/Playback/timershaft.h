#ifndef TIMERSHAFT_H
#define TIMERSHAFT_H

/**
 * @brief         展示时间轴，通过点击来播放视频
 * @author        xiaobin <xiaobin@sunniwell.com>
 * @date          20180504
 */

#include <QWidget>
#include <QPainter>
#include "Protocol/vidiconprotocol.h"
#include "basewidget.h"

#define GROOVEHEIGHT  10
#define TICKMAXHEIGHT    15
#define TICKMAXWIDTH    1
#define TRIANGLEWIDTH  10
#define TRIANGLEHEIGHT  8
#define ZOOMDELTA      0.5
#define HALFHOURSEC    1800
#define ONEDAYSEC    86399
#define DIFFVALUE      5

class TimerShaft : public BaseWidget
{
    Q_OBJECT
public:
    explicit TimerShaft(int m_htmlid, QWidget *parent = nullptr);

signals:

public slots:
    void drawTick(QPainter &p);
    void drawGroove(QPainter &p);
    void drawHighlight(QPainter &p);
    void drawTimeText(QPainter &p);
    void drawCurTimeTag(QPainter &p);
    void drawInfo(QPainter &p);
    void drawFloatingFrame(QPainter &p);
    void handleReceiveData(VidiconProtocol::Type type, QByteArray data);
    void hanlderDateChange(QDate m_date);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    bool event(QEvent *event) override;

private:
    void checkStartPlayTime(QPoint pos);

private:
    qreal  m_stretchScale;    //缩放比例
    QPoint m_movePos;         //鼠标移动的位置
    bool   m_isPress;
    bool   m_isMoving;        //显示浮动时间点的标志
    qreal  m_currentPlayPos;  //播放位置的百分比
    bool   m_isDragPlayPos;   //拖拽时间点标志
    qreal  m_leftPos;         //时间线左上角的x值

    int m_margin;                     //时间线左右外边距间隙==>时间线的水平偏移量
    int m_width;                      //时间线可见部分的宽度
    int m_height;                     //时间线可见部分的高度
    qreal m_halfHourTickInterval;     //时间线中30min所占宽度

    QMap<int, TimeParameter> m_timeParamMap;
    QDate m_date;
    int m_htmlid;
    bool m_isPlaying;
};

#endif // TIMERSHAFT_H
