#ifndef STACKEDWIDGET_H
#define STACKEDWIDGET_H

/**
 * @brief         主要为设置界面提供的一个基础的带堆叠窗口功能的类
 *                当设置参数时显示遮罩
 * @author        xiaobin <xiaobin@sunniwell.com>
 * @date          20180504
 */

#include <QStackedWidget>
#include "basewidget.h"
#include "lineedit.h"

class StackedWidget : public BaseWidget
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
