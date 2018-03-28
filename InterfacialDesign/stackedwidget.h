#ifndef STACKEDWIDGET_H
#define STACKEDWIDGET_H

#include <QStackedWidget>
#include "basicwidget.h"
#include "lineedit.h"

class StackedWidget : public BasicWidget
{
    Q_OBJECT
public:
    explicit StackedWidget(QWidget *parent = nullptr);

    void setCurrentIndex(int index);
    int currentIndex() const;

    void addWidget(QWidget *widget);

public slots:
    virtual void handlePrepareData() = 0;
    virtual void setCurrentIndex(const QModelIndex &index) = 0;

private:
    QStackedWidget *m_stackedWidget;
};

#endif // STACKEDWIDGET_H
