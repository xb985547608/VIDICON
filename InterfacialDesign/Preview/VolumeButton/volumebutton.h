#ifndef VOLUMEBUTTON_H
#define VOLUMEBUTTON_H

#include <QPushButton>
#include <QSlider>
#include <QMenu>
#include <QLabel>

namespace Ui {
class VolumeForm;
}

class VolumeButton : public QPushButton
{
    Q_OBJECT
public:
    explicit VolumeButton(QWidget *parent = nullptr);
    ~VolumeButton(){}

signals:

public slots:
    void popupMenu();
    void volumeAdd();
    void volumeSub();
private:
    Ui::VolumeForm *ui;

    QSlider *slider;
    QMenu *menu;
    QLabel *label;
};

#endif // VOLUMEBUTTON_H
