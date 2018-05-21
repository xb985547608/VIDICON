#ifndef DRAWMOTIONREGION_H
#define DRAWMOTIONREGION_H

/**
 * @brief         显示区域上移动侦测范围的呈现与变更
 * @author        xiaobin <xiaobin@sunniwell.com>
 * @date          20180504
 */

#include <QMap>
#include "imagebasewidget.h"

#define REGIONROW 18
#define REGIONCOLUMN 22

class MotionWidget : public ImageBaseWidget
{
    Q_OBJECT
public:
    explicit MotionWidget(QWidget *parent = nullptr);
    char **getMotionRegionMap() { return m_motionRegionMap; }

protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
signals:

public slots:
    void handleFullScreen();
    void handleCleanScreen();

private:
    char **m_motionRegionMap;

    bool m_validPressLeft;
    bool m_validPressRight;
    QPoint m_pressPos;
};

#endif // DRAWMOTIONREGION_H
