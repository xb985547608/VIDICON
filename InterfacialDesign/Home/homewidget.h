#ifndef HOMEWIDGET_H
#define HOMEWIDGET_H

#include <QWidget>

namespace Ui {
class HomeForm;
}

class HomeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HomeWidget(QWidget *parent = nullptr);
    ~HomeWidget();

signals:
    void signalLogout();

public slots:
    void onWelcomeBtn();
    void onPreviewBtn();
    void onPlaybackBtn();
    void onPhotoBtn();
    void onSetupBtn();

protected:
    virtual void resizeEvent(QResizeEvent *event);

private:
    Ui::HomeForm *ui;
};

#endif // SWITCHWIDGET_H
