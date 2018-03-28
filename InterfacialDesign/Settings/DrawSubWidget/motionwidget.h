#ifndef DRAWMOTIONREGION_H
#define DRAWMOTIONREGION_H

#include <QWidget>
#include <QMap>
#include <QTimer>

#define REGIONROW 18
#define REGIONCOLUMN 22

class MotionWidget : public QWidget
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
    void handleReceiveImage(QPixmap *pixmap);
    void handleTimeout();

private:
    char **m_motionRegionMap;

    bool m_validPressLeft;
    bool m_validPressRight;
    QPoint m_pressPos;
    QPixmap m_backgroundPixmap;
    QTimer *m_timer;
};

#endif // DRAWMOTIONREGION_H
