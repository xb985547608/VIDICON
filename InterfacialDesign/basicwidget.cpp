#include "basicwidget.h"
#include <QBoxLayout>

BasicWidget::BasicWidget(QWidget *parent) :
    QWidget(parent)
{
    connect(VidiconProtocol::getInstance(), &VidiconProtocol::signalReceiveData, this, &BasicWidget::handleReceiveData);
    connect(this, &BasicWidget::signalSetParameter, VidiconProtocol::getInstance(), &VidiconProtocol::handleSetParameter);
    connect(this, &BasicWidget::signalGetParameter, VidiconProtocol::getInstance(), &VidiconProtocol::handleGetParameter);
}

void BasicWidget::setAlignment(QWidget *parentWidget, QWidget *widget, Qt::Alignment alignment)
{
    if (parentWidget == NULL || widget == NULL)
        return;

    //去除原有的布局
    if (parentWidget->layout() != NULL) {
        delete parentWidget->layout();
    }

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

void BasicWidget::setAlignment(QWidget *parentWidget, QLayout *layout, Qt::Alignment alignment)
{
    if (parentWidget == NULL || layout == NULL)
        return;

    //去除原有的布局
    if (parentWidget->layout() != NULL) {
        delete parentWidget->layout();
    }

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
