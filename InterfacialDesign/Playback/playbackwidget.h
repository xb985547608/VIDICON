#ifndef PLAYBACKWIDGET_H
#define PLAYBACKWIDGET_H

#include <QWidget>
#include "datewidget.h"
#include "filemanagerdialog.h"

namespace Ui{
class PlaybackForm;
}

class PlaybackWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlaybackWidget(QWidget *parent = nullptr);
    ~PlaybackWidget();
protected:
    void resizeEvent(QResizeEvent *event) override;

signals:
    void signalSetParameter(int type, void *param, QString SessionID = "R00001");
    void signalGetParameter(int type, int StreamType = 0, int Channel = 0, QString SessionID = "R00001");
    void signalVlcControl(int type, int subtype, WId id = 0);

public slots:
    void handlerWidgetSwitch();
    void handlerReceiveData(int type, QByteArray data);

private:
    Ui::PlaybackForm *ui;
    DateWidget *dateWidget;
    FileManagerDialog *fileDialog;
    int htmlid;
};

#endif // PLAYBACKWIDGET_H
