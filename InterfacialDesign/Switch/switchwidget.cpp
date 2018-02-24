#include "switchwidget.h"
#include "ui/ui_switchform.h"
#include <QDebug>

SwitchWidget::SwitchWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SwitchForm)
{
    ui->setupUi(this);

    connect(ui->btnWelcome, SIGNAL(clicked()), this, SLOT(onWelcomeBtn()));
    connect(ui->btnPreview, SIGNAL(clicked()), this, SLOT(onPreviewBtn()));
    connect(ui->btnPlayback,SIGNAL(clicked()), this, SLOT(onPlaybackBtn()));
    connect(ui->btnPhoto,   SIGNAL(clicked()), this, SLOT(onPhotoBtn()));
    connect(ui->btnSettings,SIGNAL(clicked()), this, SLOT(onSettingsBtn()));
    connect(ui->btnLogout,  SIGNAL(clicked()), this, SLOT(onLogoutBtn()));

    ui->btnWelcome->setVisible(false);
    ui->btnPhoto->setVisible(false);
}

SwitchWidget::~SwitchWidget()
{

}

void SwitchWidget::refreshProperty(QWidget *widget, const char *name, const QVariant &value)
{
    widget->setProperty(name, value);
    widget->style()->unpolish(widget);
    widget->style()->polish(widget);
}

void SwitchWidget::resetMenuProperty()
{
    refreshProperty(ui->btnWelcome, "choice", "false");
    refreshProperty(ui->btnPreview, "choice", "false");
    refreshProperty(ui->btnPlayback,"choice", "false");
    refreshProperty(ui->btnPhoto,   "choice", "false");
    refreshProperty(ui->btnSettings,"choice", "false");
}

void SwitchWidget::setSettingsBtnVisible(bool visible)
{
    ui->btnSettings->setVisible(visible);
}

void SwitchWidget::switchWidgetHandler(SwitchWidget::SwitchState form)
{
    switch(form){
        case SwitchWidget::Home:
            resetMenuProperty();
            refreshProperty(ui->btnWelcome,         "choice", "true");
            emit signalSwitchState(SwitchWidget::Home);
            break;
        case SwitchWidget::Preview:
            resetMenuProperty();
            refreshProperty(ui->btnPreview,         "choice", "true");
            emit signalSwitchState(SwitchWidget::Preview);
            break;
        case SwitchWidget::Playback:
            resetMenuProperty();
            refreshProperty(ui->btnPlayback,        "choice", "true");
            emit signalSwitchState(SwitchWidget::Playback);
            break;
        case SwitchWidget::Photo:
            resetMenuProperty();
            refreshProperty(ui->btnPhoto,           "choice", "true");
            emit signalSwitchState(SwitchWidget::Photo);
            break;
        case SwitchWidget::Settings:
            resetMenuProperty();
            refreshProperty(ui->btnSettings,        "choice", "true");
            emit signalSwitchState(SwitchWidget::Settings);
            break;
        default:
            break;
    }
}

void SwitchWidget::onWelcomeBtn()
{

    switchWidgetHandler(SwitchWidget::Home);
}

void SwitchWidget::onPreviewBtn()
{

    switchWidgetHandler(SwitchWidget::Preview);
}

void SwitchWidget::onPlaybackBtn()
{

    switchWidgetHandler(SwitchWidget::Playback);
}

void SwitchWidget::onPhotoBtn()
{

    switchWidgetHandler(SwitchWidget::Photo);
}

void SwitchWidget::onSettingsBtn()
{

    switchWidgetHandler(SwitchWidget::Settings);
}

void SwitchWidget::onLogoutBtn()
{
    emit signalLogout();
}
