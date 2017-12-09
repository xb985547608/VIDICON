#ifndef SETUPWIDGET_H
#define SETUPWIDGET_H

#include <QWidget>

namespace Ui{
class SettingsForm;
}

class SettinsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SettinsWidget(QWidget *parent = nullptr);
    ~SettinsWidget();


signals:

public slots:

private:
    Ui::SettingsForm *ui;

};

#endif // SETUPWIDGET_H
