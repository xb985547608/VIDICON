#include "previewwidget.h"
#include "ui_previewform.h"

PreviewWidget::PreviewWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PreviewForm)
{
    ui->setupUi(this);
}

PreviewWidget::~PreviewWidget()
{

}
