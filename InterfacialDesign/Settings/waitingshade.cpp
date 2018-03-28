#include "waitingshade.h"
#include <QPixmap>
#include <QEvent>
#include <QPaintEvent>
#include <QChildEvent>
#include <QPainter>
#include <QLabel>
#include <QVBoxLayout>
#include <QMovie>

WaitingShade *WaitingShade::s_instance = NULL;
WaitingShade::WaitingShade(bool folowToHeadWidget, QWidget *parent) :
    QWidget(parent),
    m_isUpdateSelf(false)
{
    Q_ASSERT_X(parent, "", Q_FUNC_INFO);

    if (folowToHeadWidget) {
        while (parent->parentWidget() != 0) {
            parent = parent->parentWidget();
        }
        setParent(parent);
    }

    parent->installEventFilter(this);

    QLabel* lbTitle = new QLabel(tr("请耐心等待，正在拼命上传参数......"));
    lbTitle->setStyleSheet("font-size: 28px; font-weight: bold; color: white");
    QLabel* lbProgress = new QLabel;
    QMovie* progressMovie = new QMovie(":/images/loader.gif");
    lbProgress->setMovie(progressMovie);
    progressMovie->start();

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addStretch();
    layout->addWidget(lbTitle, 0, Qt::AlignCenter);
    layout->addWidget(lbProgress, 0, Qt::AlignCenter);
    layout->addStretch();

    setVisible(false);
}

void WaitingShade::showEvent(QShowEvent *event)
{
    updateSelf();
    QWidget::showEvent(event);
}

void WaitingShade::paintEvent(QPaintEvent *event)
{
    QPainter p;

    p.begin(this);
    p.drawPixmap(0, 0, width(), height(), m_parentWidgetPixmap);
    p.setBrush(QBrush(QColor(0, 0, 0, 220)));
    p.drawRect(0, 0, width(), height());
    p.end();

    QWidget::paintEvent(event);
}

bool WaitingShade::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::ChildAdded) {
        QChildEvent* childEvent = dynamic_cast<QChildEvent*>(event);
        //保持对象处于最顶层
        if (childEvent->child() != this) {
            QWidget* parent = parentWidget();
            setParent(0);
            setParent(parent);
        }
    }

    if(isVisible() && event->type() == QEvent::Resize) {
        updateSelf();
    }
    return QWidget::eventFilter(watched, event);
}

void WaitingShade::updateSelf()
{
    if (!m_isUpdateSelf) {
        m_isUpdateSelf = true;
        {
            hide();
            resize(parentWidget()->size());
            m_parentWidgetPixmap = grabParentWidgetPixmap();
            show();
        }
        m_isUpdateSelf = false;
    }
}

QPixmap WaitingShade::grabParentWidgetPixmap() const
{
    QPixmap parentWidgetPixmap;

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    parentWidgetPixmap = parentWidget()->grab();
#else
    parentWidgetPixmap = QPixmap::grabWidget(parentWidget());
#endif

    return parentWidgetPixmap;
}
