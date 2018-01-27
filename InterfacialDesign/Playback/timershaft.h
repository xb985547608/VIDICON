#ifndef TIMERSHAFT_H
#define TIMERSHAFT_H

#include <QWidget>
#include <QPainter>
#include "Protocol/vidiconprotocol.h"

#define GROOVEHEIGHT  10
#define TICKMAXHEIGHT    15
#define TICKMAXWIDTH    1
#define TRIANGLEWIDTH  10
#define TRIANGLEHEIGHT  8
#define ZOOMDELTA      0.3

class TimerShaft : public QWidget
{
    Q_OBJECT
public:
    explicit TimerShaft(QWidget *parent = nullptr);

signals:
    void signalSetParameter(int type, void *param, QString SessionID = "R00001");
    void signalGetParameter(int type, int StreamType = 0, int Channel = 0, QString SessionID = "R00001");

public slots:
    void drawTick(QPainter &p);
    void drawGroove(QPainter &p);
    void drawHighlight(QPainter &p);
    void drawTimeText(QPainter &p);
    void drawCurTimeTag(QPainter &p);
    void drawInfo(QPainter &p);
    void drawFloatingFrame(QPainter &p);
    void handlerReceiveData(int type, QByteArray data);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    bool event(QEvent *event) override;

private:
    qreal  stretchScale;    //缩放比例
    QPoint movePos;         //鼠标移动的位置
    bool   isMoving;        //鼠标移动的标志
    qreal  currentPlayPos;  //播放位置的百分比
    bool   isDragPlayPos;   //拖拽时间点标志
    qreal  leftPos;         //时间轴左边框位置

    int margin;                     //左右外边距间隙
    int width;                      //时间轴可视宽度
    int height;                     //时间轴高度
    qreal halfHourTickInterval;     //时间轴中30min所占宽度

    QMap<int, VidiconProtocol::TimeParameter> TimeParamMap;
};

#endif // TIMERSHAFT_H
