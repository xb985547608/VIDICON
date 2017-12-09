#include "settingswidget.h"
#include "ui_settingsform.h"

SettinsWidget::SettinsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsForm)
{
    ui->setupUi(this);
}

SettinsWidget::~SettinsWidget()
{

}
