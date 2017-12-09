#ifndef PREVIEWWIDGET_H
#define PREVIEWWIDGET_H

#include <QWidget>

namespace Ui{
class PreviewForm;
}

class PreviewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PreviewWidget(QWidget *parent = nullptr);
    ~PreviewWidget();

signals:

public slots:

private:
    Ui::PreviewForm *ui;

};

#endif // PREVIEWWIDGET_H
