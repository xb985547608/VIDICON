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
    char **getMotionRegionMap() { return motionRegionMap; }

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
    char **motionRegionMap;

    bool validPressLeft;
    bool validPressRight;
    QPoint pressPos;
    QPixmap backgroundPixmap;
    QTimer *timer;
};

#endif // DRAWMOTIONREGION_H
