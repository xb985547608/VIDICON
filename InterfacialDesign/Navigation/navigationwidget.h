#ifndef NAVIGATIONWIDGET_H
#define NAVIGATIONWIDGET_H

#include <QWidget>
#include <QStackedWidget>

#define CONTENTMARGINLEFT   16
#define CONTENTMARGINRIGHT  16
#define CONTENTMARGIN       (CONTENTMARGINLEFT + CONTENTMARGINRIGHT)
#define ITEMMINSIZE         (40 + CONTENTMARGIN)
#define ITEMMAXSIZE         (120 + CONTENTMARGIN)
#define ITEMICONMARGIN      8

class NavigationItem;
class NavigationBar;

class NavigationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NavigationWidget(QWidget* parent = NULL, Qt::WindowFlags flags = 0);
    ~NavigationWidget();

    int addTab(QWidget* widget, const QString& text);
    int addTab(QWidget* widget, const QIcon& icon, const QString& text);

    int insertTab(int index, QWidget* widget, const QString& text);
    int insertTab(int index, QWidget* widget, const QIcon& icon, const QString& text);

    void removeTab(int index);

    QString tabText(int index) const;
    void setTabText(int index, const QString& text);

    QIcon tabIcon(int index) const;
    void setTabIcon(int index, const QIcon& icon);

    int horizontalAlignment() const;
    void setHorizontalAlignment(Qt::AlignmentFlag alignment);

    int count() const;
    int currentIndex() const;

public slots:
    void setCurrentIndex(int index);

signals:
    void currentChanged(int index);

private:
    QStackedWidget *stacked;
    NavigationBar *bar;
};

class NavigationBar : public QWidget
{
Q_OBJECT
public:
    explicit NavigationBar(NavigationWidget* q, QWidget* parent = NULL, Qt::WindowFlags flags = 0);
    ~NavigationBar();

    int addTab(const QString& text);
    int addTab(const QIcon& icon, const QString& text);

    int insertTab(int index, const QString& text);
    int insertTab(int index, const QIcon& icon, const QString& text);

    void removeTab(int index);

    QString tabText(int index) const;
    void setTabText(int index, const QString& text);

    QIcon tabIcon(int index) const;
    void setTabIcon(int index, const QIcon& icon);

    int horizontalAlignment() const;
    void setHorizontalAlignment(Qt::AlignmentFlag alignment);

    int count() const;
    int currentIndex() const;

public slots:
    void setCurrentIndex(int index);

protected:
    virtual void enterEvent(QEvent* event);
    virtual void leaveEvent(QEvent* event);
    virtual void paintEvent(QPaintEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);

private:
    void drawBackground(QPainter *p);
    void drawItem(const QRect& rect, QPainter *p, int index, int reserved);

    int indexByPosition(const QPoint& position);
    int navigationWidth();
    int navigationItemWidth(int index, int reserved);
protected:
    NavigationWidget *d;
private:
    int mCurrentIndex;
    QList<NavigationItem *> mItemList;
    Qt::AlignmentFlag mAlignment;

    QPixmap bgPixmap;
    QPixmap linePixmap;
    QPixmap clickedPixmap;
};

class NavigationItem
{
public:
    NavigationItem() :
        isClicked(false)
    {}

    QString title;
    bool isClicked;
    QRect lastRect;
    QPixmap icon;

    inline int width(const QFontMetrics &fontMetrics)
    { return fontMetrics.width(title); }
};

#endif // NAVIGATIONWIDGET_H
