#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>

class ImageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ImageWidget(QWidget *parent = nullptr);

protected:
    virtual void paintEvent(QPaintEvent *event);

signals:

public slots:
    void handlerReceiveImage(QPixmap pixmap);
    void handlerTimeout();
private:
    QPixmap backgroundPixmap;
};

#endif // IMAGEWIDGET_H
