#ifndef SWITCHWIDGET_H
#define SWITCHWIDGET_H

#include <QWidget>

namespace Ui {
class SwitchForm;
}

class SwitchWidget : public QWidget
{
    Q_OBJECT
public:
    enum SwitchState{
        Home,
        Preview,
        Playback,
        Photo,
        Settings
    };

    explicit SwitchWidget(QWidget *parent = nullptr);
    ~SwitchWidget();

    void refreshProperty(QWidget *widget, const char *name, const QVariant &value);
    void resetMenuProperty();
    void setSettingsBtnVisible(bool visible);
signals:
    void signalLogout();
    void signalSwitchState(SwitchWidget::SwitchState form);

public slots:
    void switchWidgetHandler(SwitchWidget::SwitchState form);
    void onWelcomeBtn();
    void onPreviewBtn();
    void onPlaybackBtn();
    void onPhotoBtn();
    void onSettingsBtn();
    void onLogoutBtn();

private:
    Ui::SwitchForm *ui;
};

#endif // SWITCHWIDGET_H
