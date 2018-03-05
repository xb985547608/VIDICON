#include "statustip.h"
#include <QEvent>
#include <QDebug>

StatusTip *StatusTip::_instance = NULL;
StatusTip::StatusTip(bool folowToHeadWidget, QWidget *parent) :
    QLabel(parent),
    tipHeight(20)
{
    Q_ASSERT_X(parent, "", Q_FUNC_INFO);
    //追溯根界面
    if (folowToHeadWidget) {
        while (parent->parentWidget() != 0) {
            parent = parent->parentWidget();
        }
        setParent(parent);
    }
    parent->installEventFilter(this);
    setAlignment(Qt::AlignRight | Qt::AlignCenter);
    setStyleSheet("background-color:lightGray; color:black; border:1px solid darkGray; border-top-left-radius:5px");

    effect = new QGraphicsOpacityEffect(this);
    setGraphicsEffect(effect);

    animation = new QPropertyAnimation(effect, "opacity", this);
    animation->setDuration(3000);
    animation->setStartValue(1.0);
    animation->setEndValue(0.0);
    animation->setLoopCount(1);
    animation->setEasingCurve(QEasingCurve::InQuart);

    connect(animation, &QPropertyAnimation::finished, this, &StatusTip::hide);

    hide();
}

bool StatusTip::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::ChildAdded) {
        QChildEvent* childEvent = dynamic_cast<QChildEvent*>(event);
        if (childEvent->child() != this) {
            QWidget* parent = parentWidget();
            setParent(NULL);
            setParent(parent);
        }
    }

    if(event->type() == QEvent::Resize) {
        QWidget* parent = parentWidget();
        QRect rect(parent->width() / 2, parent->height() - tipHeight,
                   parent->width() / 2, tipHeight);
        setGeometry(rect);
    }
    return QWidget::eventFilter(watched, event);
}

void StatusTip::showEvent(QShowEvent *event)
{
    QWidget *parent = parentWidget();
    if (parent == NULL)
        return;

    QLabel::showEvent(event);
}

void StatusTip::showStatusTip(QString text)
{
    setText(text);

    animation->stop();
    animation->start();
    show();
}
