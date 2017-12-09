#ifndef SWITCHBUTTON_H
#define SWITCHBUTTON_H

#include <QWidget>
#include <QPixmap>
#include <QTimer>

class SwitchButton : public QWidget
{
    Q_OBJECT
public:
    explicit SwitchButton(QWidget *parent = nullptr);
    ~SwitchButton(){}

    bool getState() const{ return isOpen; }
signals:

protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);

public slots:
    void slideHandle();
private:
    QPixmap *backgroundImg;
    QPixmap *addImg;
    QPixmap *subImg;
    QPixmap *handleImg;

    QTimer *timer;
    bool isOpen;
    bool isSlide;

    int slideLeft;
    int slideRight;
    int slidePos;

    int handleLeftLimit;
    int handleRightLimit;
};

#endif // SWITCHBUTTON_H
