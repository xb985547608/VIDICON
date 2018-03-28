#include "basicwidget.h"

BasicWidget::BasicWidget(QWidget *parent) :
    QWidget(parent)
{
    connect(VidiconProtocol::getInstance(), &VidiconProtocol::signalReceiveData, this, &BasicWidget::handleReceiveData);
    connect(this, &BasicWidget::signalSetParameter, VidiconProtocol::getInstance(), &VidiconProtocol::handleSetParameter);
    connect(this, &BasicWidget::signalGetParameter, VidiconProtocol::getInstance(), &VidiconProtocol::handleGetParameter);
}
