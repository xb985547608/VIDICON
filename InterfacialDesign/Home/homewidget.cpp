#include "homewidget.h"
#include <QDebug>
#include "ui_homeform.h"

HomeWidget::HomeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomeForm)
{
    ui->setupUi(this);
}

HomeWidget::~HomeWidget()
{

}

void HomeWidget::onWelcomeBtn()
{

}

void HomeWidget::onPreviewBtn()
{

}

void HomeWidget::onPlaybackBtn()
{

}

void HomeWidget::onPhotoBtn()
{
}

void HomeWidget::onSetupBtn()
{
}

void HomeWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
}
