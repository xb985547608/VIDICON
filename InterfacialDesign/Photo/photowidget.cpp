#include "photowidget.h"
#include "ui_photoform.h"

PhotoWidget::PhotoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PhotoForm)
{
    ui->setupUi(this);
}

PhotoWidget::~PhotoWidget()
{

}
