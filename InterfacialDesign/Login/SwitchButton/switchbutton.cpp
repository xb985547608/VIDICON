#include "switchbutton.h"
#include <QMouseEvent>
#include <QPainter>
#include <QRect>
#include <QDebug>

SwitchButton::SwitchButton(QWidget *parent) : QWidget(parent),
    isOpen(true), isSlide(false)
{
    backgroundImg = new QPixmap(":/images/login-SwitchBtn-Background.png");
    addImg = new QPixmap(":/images/login-SwitchBtn-Add.png");
    subImg = new QPixmap(":/images/login-SwitchBtn-Sub.png");
    handleImg = new QPixmap(":/images/login-SwitchBtn-Handle.png");

    slideLeft = (backgroundImg->width() - addImg->width())/2;
    slideRight = backgroundImg->width() - (backgroundImg->width() - addImg->width())/2;
    handleLeftLimit = 3;
    handleRightLimit = backgroundImg->width() - handleImg->width() - 3;

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(slideHandle()));


    setCursor(Qt::PointingHandCursor);
    resize(backgroundImg->size());
}

void SwitchButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);
    if(isSlide){
        p.drawPixmap(this->rect(), *backgroundImg);

        QRect rect1(this->rect().x() + (this->rect().width() - subImg->width())/2,
                    this->rect().y() + (this->rect().height() - subImg->height())/2,
                    subImg->width(), subImg->height());
        p.drawPixmap(rect1, *subImg);

        int width = (rect1.width() - slidePos) > 0 ? rect1.width() - slidePos : 0;
        QRect rect2(rect1.x() + rect1.width() - width,
                    this->rect().y() + (this->rect().height() - addImg->height())/2,
                    width, addImg->height());
        p.drawPixmap(rect2, addImg->copy(rect1.width() - width, 0, rect2.width(), rect2.height()));

        int handlePos = slidePos - handleImg->width()/2;
        handlePos = handlePos < handleLeftLimit ? handleLeftLimit : (handlePos > handleRightLimit ? handleRightLimit : handlePos);
        QRect rect3(handlePos,
                    this->rect().y() + (this->rect().height() - handleImg->height())/2 + 2,
                    handleImg->width(), handleImg->height());
        p.drawPixmap(rect3, *handleImg);

        return;
    }

    if(isOpen){
        p.drawPixmap(this->rect(), *backgroundImg);

        QRect rect1(this->rect().x() + (this->rect().width() - addImg->width())/2,
                    this->rect().y() + (this->rect().height() - addImg->height())/2,
                    addImg->width(), addImg->height());
        p.drawPixmap(rect1, *addImg);

        QRect rect2(handleLeftLimit,
                    this->rect().y() + (this->rect().height() - handleImg->height())/2 + 2,
                    handleImg->width(), handleImg->height());
        p.drawPixmap(rect2, *handleImg);
    }else{
        p.drawPixmap(this->rect(), *backgroundImg);

        QRect rect1(this->rect().x() + (this->rect().width() - subImg->width())/2,
                    this->rect().y() + (this->rect().height() - subImg->height())/2,
                    subImg->width(), subImg->height());
        p.drawPixmap(rect1, *subImg);

        QRect rect2(handleRightLimit,
                    this->rect().y() + (this->rect().height() - handleImg->height())/2 + 2,
                    handleImg->width(), handleImg->height());
        p.drawPixmap(rect2, *handleImg);
    }
}

void SwitchButton::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    if(event->type() == QMouseEvent::MouseButtonPress && (event->buttons() & Qt::LeftButton)){
        event->accept();
        if(isSlide)
            return;
        isSlide = true;
        isOpen = !isOpen;
        slidePos = isOpen ? slideRight : slideLeft;
        timer->start(10);
    }else{
        event->ignore();
    }
}

void SwitchButton::slideHandle()
{
    if(isOpen){
        slidePos -= 2;
        if(slidePos < slideLeft){
            timer->stop();
            isSlide = false;
        }
        update();
    }else{
        slidePos += 2;
        if(slidePos > slideRight){
            timer->stop();
            isSlide = false;
        }
        update();
    }
}
