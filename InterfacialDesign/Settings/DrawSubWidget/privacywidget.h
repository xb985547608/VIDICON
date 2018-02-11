#ifndef PRIVACYWIDGET_H
#define PRIVACYWIDGET_H

#include <QWidget>

class PrivacyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PrivacyWidget(QWidget *parent = nullptr);
    ~PrivacyWidget();

    void reset();
    const QRect *getRects() { return rects; }

protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

signals:

public slots:
    void handlerReceiveImage(QPixmap *pixmap);
    void handlerTimeout();

private:
    QPixmap backgroundPixmap;
    QRect *rects;
    QPoint startPos;
    QPoint endPos;

    int currentIndex;
};

#endif // PRIVACYWIDGET_H
