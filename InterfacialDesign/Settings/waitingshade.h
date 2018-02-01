#ifndef WAITINGSHADE_H
#define WAITINGSHADE_H

#include <QWidget>

class WaitingShade : public QWidget
{
    Q_OBJECT
public:
    static WaitingShade *getInstance(QWidget *parent = nullptr) {
        if(_instance == NULL) {
            _instance = new WaitingShade(true, parent);
        }
        return _instance;
    }

protected:
    void showEvent(QShowEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    explicit WaitingShade(bool folowToHeadWidget, QWidget *parent = nullptr);

    void updateSelf();
    QPixmap grabParentWidgetPixmap() const;

signals:

public slots:

private:
    QPixmap parentWidgetPixmap;
    bool isUpdateSelf;
    static WaitingShade *_instance;
};

#endif // WAITINGSHADE_H
