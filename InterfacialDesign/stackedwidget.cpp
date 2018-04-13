#include <QBoxLayout>

#include "stackedwidget.h"

StackedWidget::StackedWidget(QWidget *parent) :
    BasicWidget(parent)
{
    m_stackedWidget = new QStackedWidget(parent);

    if (layout() == NULL) {
        QHBoxLayout *row = new QHBoxLayout;
        row->setSpacing(0);
        row->setContentsMargins(0, 0, 0, 0);
        setLayout(row);
    }
    QLayout *row = layout();
    row->addWidget(m_stackedWidget);
}

void StackedWidget::setCurrentIndex(int index)
{
    if (m_stackedWidget->count() == 0)
        return;

    m_stackedWidget->setCurrentIndex(qBound(0, index, m_stackedWidget->count()));
}

int StackedWidget::currentIndex() const
{
    return m_stackedWidget->currentIndex();
}

void StackedWidget::addWidget(QWidget *widget)
{
    m_stackedWidget->addWidget(widget);
}
