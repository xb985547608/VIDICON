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

    VidiconProtocol::OSDParameter *getOSDParameters() {return parameters;}

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
    VidiconProtocol::OSDParameter *parameters;
    QPixmap backgroundPixmap;

    bool validPress;
    QPoint diffValue;
    int currentMoveIndex;
};

#endif // DRAWOSDREGION_H
