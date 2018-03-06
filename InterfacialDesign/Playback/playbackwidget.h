#ifndef PLAYBACKWIDGET_H
#define PLAYBACKWIDGET_H

#include <QWidget>
#include "datewidget.h"
#include "filemanagerdialog.h"
#include <QSound>

namespace Ui{
class PlaybackForm;
}

class PlaybackWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlaybackWidget(QWidget *parent = nullptr);
    ~PlaybackWidget();

    void refreshPolish(QWidget *w);

private:
    void setStateValue(int value);

protected:
    void resizeEvent(QResizeEvent *event) override;

signals:
    void signalSetParameter(int type, void *param, QString SessionID = "R00001");
    void signalGetParameter(int type, int StreamType = 0, int Channel = 0, QString SessionID = "R00001");
    void signalVlcControl(int type, int subtype = -1, WId id = 0);
    void signalAddDownloadTask(QStringList files);

public slots:
    void handleWidgetSwitch();
    void handleReceiveData(int type, QByteArray data);
    void onPlayBtnClicked();
    void onSlowForwardBtnClicked();
    void onFastPlayBtnClicked();
    void onSnapshotBtnClicked();
    void onVolumeBtnClicked();
    void handleTimeout();

private:
    Ui::PlaybackForm *ui;
    DateWidget *dateWidget;
    FileManagerDialog *fileDialog;
    int htmlid;
    //-2 *4  -1 *2  0  1 *2  2 *4
    //    慢放--> 正常 -->快放
    int stateValue;
    QSound *snapshotSoundEffect;
    bool isPlaying;
};

#endif // PLAYBACKWIDGET_H
