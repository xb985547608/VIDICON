#ifndef PHOTOWIDGET_H
#define PHOTOWIDGET_H

#include <QWidget>

namespace Ui {
class PhotoForm;
}

class PhotoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PhotoWidget(QWidget *parent = nullptr);
    ~PhotoWidget();

signals:

public slots:
private:
    Ui::PhotoForm *ui;
};

#endif // PHOTOWIDGET_H
