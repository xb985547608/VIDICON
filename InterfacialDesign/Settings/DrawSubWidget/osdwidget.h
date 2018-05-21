#ifndef DRAWOSDREGION_H
#define DRAWOSDREGION_H

/**
 * @brief         显示区域上OSD的位置信息呈现与变更
 * @author        xiaobin <xiaobin@sunniwell.com>
 * @date          20180504
 */

#include "imagebasewidget.h"
#include "Protocol/vidiconprotocol.h"
#include <QMap>

#define XSCALEMAX 1000
#define YSCALEMAX 1000

class OSDWidget : public ImageBaseWidget
{
    Q_OBJECT
public:
    explicit OSDWidget(QWidget *parent = nullptr);
    ~OSDWidget();

    QList<OSDParameter> &getOSDParameters() {return m_parameters;}

protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

private:
    QList<OSDParameter> m_parameters;

    bool m_validPress;
    QPoint m_lastPos;
    int m_currentMoveIndex;
};

#endif // DRAWOSDREGION_H
