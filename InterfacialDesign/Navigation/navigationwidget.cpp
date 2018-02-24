#include "navigationwidget.h"
#include <QVBoxLayout>
#include <QIcon>
#include <QMouseEvent>
#include <QPainter>
#include <QLinearGradient>
#include <QDebug>

NavigationWidget::NavigationWidget(QWidget *parent, Qt::WindowFlags flags) :
    QWidget(parent),
    stacked(new QStackedWidget(this)),
    bar(new NavigationBar(this, this, flags))
{
    QVBoxLayout *vl = new QVBoxLayout(this);
    vl->setMargin(0);
    vl->setSpacing(0);
    vl->addWidget(bar, 0, Qt::AlignTop);
    vl->addWidget(stacked, 1);
}

NavigationWidget::~NavigationWidget()
{
}

int NavigationWidget::addTab(QWidget *widget, const QString &text)
{
    stacked->addWidget(widget);
    int index = bar->addTab(text);
    if(count() == 1) {
        setCurrentIndex(0);
    }
    return index;
}

int NavigationWidget::addTab(QWidget *widget, const QIcon &icon, const QString &text)
{
    stacked->addWidget(widget);
    int index = bar->addTab(icon, text);
    if(count() == 1) {
        setCurrentIndex(0);
    }
    return index;
}

int NavigationWidget::insertTab(int index, QWidget *widget, const QString &text)
{
    stacked->insertWidget(index, widget);
    return bar->insertTab(index, text);
}

int NavigationWidget::insertTab(int index, QWidget *widget, const QIcon &icon, const QString &text)
{
    stacked->insertWidget(index, widget);
    return bar->insertTab(index, icon, text);
}

void NavigationWidget::removeTab(int index)
{
    bar->removeTab(index);
    stacked->removeWidget(stacked->widget(index));
}

QString NavigationWidget::tabText(int index) const
{
    return bar->tabText(index);
}

void NavigationWidget::setTabText(int index, const QString &text)
{
    bar->setTabText(index, text);
}

QIcon NavigationWidget::tabIcon(int index) const
{
    return bar->tabIcon(index);
}

void NavigationWidget::setTabIcon(int index, const QIcon &icon)
{
    bar->setTabIcon(index, icon);
}

int NavigationWidget::horizontalAlignment() const
{
    return bar->horizontalAlignment();
}

void NavigationWidget::setHorizontalAlignment(Qt::AlignmentFlag alignment)
{
    bar->setHorizontalAlignment(alignment);
}

int NavigationWidget::count() const
{
    return bar->count();
}

int NavigationWidget::currentIndex() const
{
    return bar->currentIndex();
}

void NavigationWidget::setCurrentIndex(int index)
{
    int current = bar->currentIndex();
    bar->setCurrentIndex(index);
    if(current != bar->currentIndex()) {
        stacked->setCurrentIndex(index);
        emit currentChanged(index);
    }
}

NavigationBar::NavigationBar(NavigationWidget *q, QWidget *parent, Qt::WindowFlags flags) :
    QWidget(parent, flags),
    d(q),
    mCurrentIndex(-1),
    mAlignment(Qt::AlignCenter)
{
    setMouseTracking(true);
    setFixedHeight(40);

    bgPixmap = QPixmap(":/images/bg.png");
    linePixmap = QPixmap(":/images/line.png");
    clickedPixmap = QPixmap(":/images/clicked.png");
}

NavigationBar::~NavigationBar()
{
    qDeleteAll(mItemList);
}

int NavigationBar::addTab(const QString &text)
{
    return addTab(QIcon(), text);
}

int NavigationBar::addTab(const QIcon &icon, const QString &text)
{
    NavigationItem *item = new NavigationItem;
    item->icon = icon.pixmap(QSize(16,16));
    item->title = text;
    mItemList.append(item);
    update();
    return count() - 1;
}

int NavigationBar::insertTab(int index, const QString &text)
{
    return insertTab(index, QIcon(), text);
}

int NavigationBar::insertTab(int index, const QIcon &icon, const QString &text)
{
    NavigationItem *item = new NavigationItem;
    item->icon = icon.pixmap(QSize(16,16));
    item->title = text;
    mItemList.insert(index, item);
    update();
    return index;
}

void NavigationBar::removeTab(int index)
{
    delete mItemList.takeAt(index);
    update();
}

QString NavigationBar::tabText(int index) const
{
    return mItemList[index]->title;
}

void NavigationBar::setTabText(int index, const QString &text)
{
    mItemList[index]->title = text;
    update();
}

QIcon NavigationBar::tabIcon(int index) const
{
    return mItemList[index]->icon;
}

void NavigationBar::setTabIcon(int index, const QIcon &icon)
{
    mItemList[index]->icon = icon.pixmap(QSize(16, 16));
    update();
}

int NavigationBar::horizontalAlignment() const
{
    return mAlignment;
}

void NavigationBar::setHorizontalAlignment(Qt::AlignmentFlag alignment)
{
    mAlignment = alignment;
    update();
}

int NavigationBar::count() const
{
    return mItemList.count();
}

int NavigationBar::currentIndex() const
{
    return mCurrentIndex;
}

void NavigationBar::setCurrentIndex(int index)
{
    if (index != -1 && index != mCurrentIndex)
    {
       if (mCurrentIndex != -1)
          mItemList[mCurrentIndex]->isClicked = false;

       mItemList[index]->isClicked = true;
       mCurrentIndex = index;
       update();
    }
}

void NavigationBar::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);
    update();
}

void NavigationBar::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);
    update();
}

void NavigationBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);
    drawBackground(&p);

    int navWidth = navigationWidth();
    int navHeight = rect().height() - 2;

    int margin = 0;
    switch (mAlignment) {
    case Qt::AlignLeft:
       margin = 0;
       break;
    case Qt::AlignRight:
       margin = rect().width() - navWidth;
    default:
       margin = rect().width() / 2 - navWidth / 2;
       break;
    }

    if(count() > 0) {
        p.drawPixmap(QRect(margin, 1, 2, navHeight), linePixmap);
        margin += 2;
    }

    for(int i=0; i<mItemList.count(); i++) {
        int itemWidth = navigationItemWidth(i, margin);
        //画item
        drawItem(QRect(margin, 1, itemWidth, navHeight), &p, i, margin);
        margin += itemWidth;
        //画分割线
        p.drawPixmap(QRect(margin, 1, 2, navHeight), linePixmap);
        margin += 2;
    }
}

void NavigationBar::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);
    update();
}

void NavigationBar::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    if (event->button() != Qt::LeftButton)
       return;

    int index = indexByPosition(event->pos());
    d->setCurrentIndex(index);
}

void NavigationBar::drawBackground(QPainter *p)
{
    p->save();
    p->drawPixmap(rect(), bgPixmap);
    p->restore();
}

void NavigationBar::drawItem(const QRect &rect, QPainter *p, int index, int reserved)
{
    bool isClicked = mItemList[index]->isClicked;

    if(isClicked || !rect.contains(mapFromGlobal(QCursor::pos()))) {
        if(isClicked) {
            p->drawPixmap(rect, clickedPixmap);
        }
    }else {
        p->fillRect(rect, QBrush(QColor("#494D4E")));
    }

    QString title = mItemList[index]->title;

    QRect textTargetRect = rect;
    QRect iconTargetRect = rect;

    if(!mItemList[index]->icon.isNull()) {
        iconTargetRect.setX(iconTargetRect.x() + ITEMICONMARGIN / 2 + mItemList[index]->icon.width() / 2);
        iconTargetRect.setY(iconTargetRect.y() + rect.height() / 2 - mItemList[index]->icon.height() / 2);

        iconTargetRect.setWidth(mItemList[index]->icon.width());
        iconTargetRect.setHeight(mItemList[index]->icon.height());

        textTargetRect.setX(iconTargetRect.right() - ITEMICONMARGIN);

        p->drawPixmap(iconTargetRect, mItemList[index]->icon);
    }

    //判断是否需要省略文本
    if (textTargetRect.width() < navigationItemWidth(index, reserved))
       title = fontMetrics().elidedText(title, Qt::TextElideMode::ElideRight, textTargetRect.width() - CONTENTMARGIN);

    p->setPen(Qt::white);
    p->drawText(textTargetRect, title, QTextOption(Qt::AlignCenter));

    mItemList[index]->lastRect = rect;
}

int NavigationBar::indexByPosition(const QPoint &position)
{
    for (int i = 0; i < mItemList.count(); i++)
    {
       if (mItemList[i]->lastRect.contains(position))
          return i;
    }
    return -1;
}

//所有Item所占的位置大小
int NavigationBar::navigationWidth()
{
    int result = 0;
    for (int i = 0; i < mItemList.count(); i++)
    {
       result += navigationItemWidth(i, result);
    }

    return result;
}

int NavigationBar::navigationItemWidth(int index, int reserved)
{
    NavigationItem *item = mItemList[index];
    //文字所占宽度
    int result = item->width(fontMetrics());
    //剩余可用宽度大小
    int available = rect().width() - reserved;
    //图标所占宽度
    int iconWidth = 0;
    if (!item->icon.isNull())
       iconWidth = item->icon.width() + ITEMICONMARGIN;

    result = result + CONTENTMARGINLEFT + CONTENTMARGINRIGHT + iconWidth;
    result = qMax(ITEMMINSIZE, result);
    result = qMin(ITEMMAXSIZE, result);
    result = qMin(result, available);

    return result;
}
