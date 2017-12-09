#include "playbackwidget.h"
#include "ui_playbackform.h"

PlaybackWidget::PlaybackWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlaybackForm)
{
    ui->setupUi(this);
}

PlaybackWidget::~PlaybackWidget()
{

}
