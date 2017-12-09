#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Login/loginwidget.h"
#include "Switch/switchwidget.h"
#include "Home/homewidget.h"
#include "Preview/previewwidget.h"
#include "Playback/playbackwidget.h"
#include "Photo/photowidget.h"
#include "Settings/settingswidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void loginHandler(LoginWidget::LoginState state);
    void logoutHandler();
    void switchStateHandler(SwitchWidget::SwitchState form);

protected:
    virtual void resizeEvent(QResizeEvent *event);

private:
    void resetWidgetState();

    Ui::MainWindow *ui;

    LoginWidget *loginWidget;
    SwitchWidget *switchWidget;
    HomeWidget *homeWidget;
    PreviewWidget *previewWidget;
    PlaybackWidget *playbackWidget;
    PhotoWidget *photoWidget;
    SettinsWidget *settinsWidget;
};

#endif // MAINWINDOW_H
