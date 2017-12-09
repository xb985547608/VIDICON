#ifndef PLAYBACKWIDGET_H
#define PLAYBACKWIDGET_H

#include <QWidget>

namespace Ui{
class PlaybackForm;
}

class PlaybackWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlaybackWidget(QWidget *parent = nullptr);
    ~PlaybackWidget();

signals:

public slots:

private:
    Ui::PlaybackForm *ui;
};

#endif // PLAYBACKWIDGET_H
