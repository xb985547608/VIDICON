#ifndef PRIVACYWIDGET_H
#define PRIVACYWIDGET_H

/**
 * @brief         显示区域上隐私区域的呈现与变更
 * @author        xiaobin <xiaobin@sunniwell.com>
 * @date          20180504
 */

#include "imagebasewidget.h"

class PrivacyWidget : public ImageBaseWidget
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
    void fullScreen();
    void clearScreen();

private:
    QRect *m_rects;
    QPoint m_startPos;
    QPoint m_endPos;

    int m_currentIndex;
};

#endif // PRIVACYWIDGET_H
