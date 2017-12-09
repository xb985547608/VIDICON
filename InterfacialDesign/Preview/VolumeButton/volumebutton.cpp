#include "volumebutton.h"
#include <QWidgetAction>
#include <QDebug>
#include <QBoxLayout>
#include <ui_volumeform.h>

VolumeButton::VolumeButton(QWidget *parent) :
    QPushButton(parent),
    ui(new Ui::VolumeForm)
{
    QWidget *popup = new QWidget(this);//生成我们学校的小部件（显示音量的窗口）
    ui->setupUi(popup);

    QWidgetAction *action = new QWidgetAction(this);//新建部件的动作（就是弹出的内容）
    action->setDefaultWidget(popup);//弹出那个窗口

    menu = new QMenu(this);//新建菜单（用于添加上面的那个动作↑）
    menu->addAction(action);//在这个菜单中增加一个内容（就是显示我们的那个滑动条）
    menu->setStyleSheet("border-image:url();background-color:#F7F7F7;border:0px;");

    connect(this, SIGNAL(clicked(bool)), this, SLOT(popupMenu()));
    connect(ui->btnVolumeAdd, SIGNAL(clicked(bool)), this, SLOT(volumeAdd()));
    connect(ui->btnVolumeSub, SIGNAL(clicked(bool)), this, SLOT(volumeSub()));
}

void VolumeButton::popupMenu()
{
    QPoint pos;
    int x = 0 + (width() - menu->sizeHint().width())/2;
    pos.setX(x);
    pos.setY(-menu->sizeHint().height() - 5);
    menu->exec(this->mapToGlobal(pos));
}

void VolumeButton::volumeAdd()
{
    int value = ui->sliderPreviewVolume->value() + 10;
    value = value > ui->sliderPreviewVolume->maximum() ? ui->sliderPreviewVolume->maximum() : value;
    ui->sliderPreviewVolume->setValue(value);
}

void VolumeButton::volumeSub()
{
    int value = ui->sliderPreviewVolume->value() - 10;
    value = value < ui->sliderPreviewVolume->minimum() ? ui->sliderPreviewVolume->minimum() : value;
    ui->sliderPreviewVolume->setValue(value);
}
