#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Login/loginwidget.h"
#include "Home/homewidget.h"
#include "Preview/previewwidget.h"
#include "Playback/playbackwidget.h"
#include "Photo/photowidget.h"
#include "Settings/settingswidget.h"
#include "Navigation/navigationwidget.h"
#include "Download/downloadwidget.h"

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
    void handleReceiveData(int type, QByteArray data);

private:
    HomeWidget *homeWidget;
    PreviewWidget *previewWidget;
    PlaybackWidget *playbackWidget;
    PhotoWidget *photoWidget;
    SettinsWidget *settinsWidget;
    NavigationWidget *navigationWidget;
    DownloadWidget *downloadWidget;
};

#endif // MAINWINDOW_H
