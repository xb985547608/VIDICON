#ifndef DRAWOSDREGION_H
#define DRAWOSDREGION_H

#include <QWidget>
#include "Protocol/vidiconprotocol.h"
#include <QMap>

#define XSCALEMAX 1000
#define YSCALEMAX 1000

class OSDWidget : public QWidget
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

signals:

public slots:
    void handleReceiveImage(QPixmap *pixmap);
    void handleTimeout();

private:
    QList<OSDParameter> m_parameters;
    QPixmap m_backgroundPixmap;

    bool m_validPress;
    QPoint m_lastPos;
    int m_currentMoveIndex;
};

#endif // DRAWOSDREGION_H
