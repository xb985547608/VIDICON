#ifndef PLAYBACKWIDGET_H
#define PLAYBACKWIDGET_H

/**
 * @brief         布局回放界面
 * @author        xiaobin <xiaobin@sunniwell.com>
 * @date          20180504
 */

#include <QWidget>
#include "datewidget.h"
#include "filemanagerdialog.h"
#include <QSound>
#include "basewidget.h"
#include "timershaft.h"

namespace Ui{
class PlaybackForm;
}

class PlaybackWidget : public BaseWidget
{
    Q_OBJECT
public:
    explicit PlaybackWidget(QWidget *parent = nullptr);
    ~PlaybackWidget();

    void refreshPolish(QWidget *w);

private:
    void setStateValue(int value);

signals:
    void signalVlcControl(int type, int subtype = -1, WId id = 0);
    void signalAddDownloadTask(QStringList files);

public slots:
    void handleReceiveData(VidiconProtocol::Type type, QByteArray data);

    void refresh();
    void onPlayBtnClicked();
    void checkPlayState();
    void onSlowForwardBtnClicked();
    void onFastPlayBtnClicked();
    void onSnapshotBtnClicked();
    void onVolumeBtnClicked();
    void handleTimeout();

private:
    Ui::PlaybackForm *ui;
    DateWidget *m_dateWidget;
    TimerShaft *m_timerShaft;
    FileManagerDialog *m_fileDialog;
    int m_htmlid;
    //-2 *4  -1 *2  0  1 *2  2 *4
    //    慢放--> 正常 -->快放
    int m_stateValue;
    bool m_isPlaying;
};

#endif // PLAYBACKWIDGET_H
