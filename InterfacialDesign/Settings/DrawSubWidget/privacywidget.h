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
    const QRect *getRects() { return m_rects; }

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
    QPixmap m_backgroundPixmap;
    QRect *m_rects;
    QPoint m_startPos;
    QPoint m_endPos;

    int m_currentIndex;
};

#endif // PRIVACYWIDGET_H
