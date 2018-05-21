#include "basewidget.h"
#include <QBoxLayout>

BaseWidget::BaseWidget(QWidget *parent) :
    QWidget(parent)
{
    connect(VidiconProtocol::getInstance(), &VidiconProtocol::signalReceiveData, this, &BaseWidget::handleReceiveData);
    connect(this, &BaseWidget::signalSetParameter, VidiconProtocol::getInstance(), &VidiconProtocol::handleSetParameter);
    connect(this, &BaseWidget::signalGetParameter, VidiconProtocol::getInstance(), &VidiconProtocol::handleGetParameter);
}

void BaseWidget::setAlignment(QWidget *parentWidget, QWidget *widget, Qt::Alignment alignment)
{
    if (parentWidget == NULL || widget == NULL)
        return;

    //去除原有的布局
    if (parentWidget->layout() != NULL) {
        delete parentWidget->layout();
    }

    if (!(alignment & Qt::AlignHorizontal_Mask))
        alignment |= Qt::AlignHCenter;
    else if (!(alignment & Qt::AlignVertical_Mask))
        alignment |= Qt::AlignVCenter;

    QHBoxLayout *row = new QHBoxLayout();
    if (alignment | Qt::AlignLeft) {
        row->addWidget(widget);
        row->addStretch();
    } else if (alignment | Qt::AlignHCenter) {
        row->addStretch();
        row->addWidget(widget);
        row->addStretch();
    } else if (alignment | Qt::AlignRight) {
        row->addStretch();
        row->addWidget(widget);
    }

    QVBoxLayout *column = new QVBoxLayout();
    if (alignment | Qt::AlignTop) {
        column->addLayout(row);
        column->addStretch();
    } else if (alignment | Qt::AlignVCenter) {
        column->addStretch();
        column->addLayout(row);
        column->addStretch();
    } else if (alignment | Qt::AlignBottom) {
        column->addStretch();
        column->addLayout(row);
    }

    parentWidget->setLayout(column);
}

void BaseWidget::setAlignment(QWidget *parentWidget, QLayout *layout, Qt::Alignment alignment)
{
    if (parentWidget == NULL || layout == NULL)
        return;

    //去除原有的布局
    if (parentWidget->layout() != NULL) {
        delete parentWidget->layout();
    }

    if (!(alignment & Qt::AlignHorizontal_Mask))
        alignment |= Qt::AlignHCenter;
    else if (!(alignment & Qt::AlignVertical_Mask))
        alignment |= Qt::AlignVCenter;

    QHBoxLayout *row = new QHBoxLayout();
    if (alignment & Qt::AlignLeft) {
        row->addLayout(layout);
        row->addStretch();
    } else if (alignment & Qt::AlignHCenter) {
        row->addStretch();
        row->addLayout(layout);
        row->addStretch();
    } else if (alignment & Qt::AlignRight) {
        row->addStretch();
        row->addLayout(layout);
    }

    QVBoxLayout *column = new QVBoxLayout();
    if (alignment & Qt::AlignTop) {
        column->addLayout(row);
        column->addStretch();
    } else if (alignment & Qt::AlignVCenter) {
        column->addStretch();
        column->addLayout(row);
        column->addStretch();
    } else if (alignment & Qt::AlignBottom) {
        column->addStretch();
        column->addLayout(row);
    }

    parentWidget->setLayout(column);
}
