#ifndef STATUSTIP_H
#define STATUSTIP_H

#include <QLabel>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

class StatusTip : public QLabel
{
    Q_OBJECT
private:
    explicit StatusTip(bool folowToHeadWidget, QWidget *parent = nullptr);
    static StatusTip *_instance;

public:
    static StatusTip *getInstance(QWidget *parent = NULL) {
        if (_instance == NULL) {
            _instance = new StatusTip(true, parent);
        }
        return _instance;
    }
    bool eventFilter(QObject *watched, QEvent *event) override;

protected:
    void showEvent(QShowEvent *event);

signals:

public slots:
    void showStatusTip(QString text);
private:
    int tipHeight;
    QPropertyAnimation *animation;
    QGraphicsOpacityEffect *effect;
};

#endif // STATUSTIP_H
