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
    void handleReceiveData(VidiconProtocol::Type type, QByteArray data);
    void handleCurrentChange(int index);

private:
    HomeWidget *m_homeWidget;
    PreviewWidget *m_previewWidget;
    PlaybackWidget *m_playbackWidget;
    PhotoWidget *m_photoWidget;
    SettinsWidget *m_settinsWidget;
    NavigationWidget *m_navigationWidget;
    DownloadWidget *m_downloadWidget;
};

#endif // MAINWINDOW_H
