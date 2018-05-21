#ifndef STATUSTIP_H
#define STATUSTIP_H

/**
 * @brief         用于提示一些客户须知的信息
 * @author        xiaobin <xiaobin@sunniwell.com>
 * @date          20180504
 */

#include <QLabel>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

class StatusTip : public QLabel
{
    Q_OBJECT
private:
    explicit StatusTip(bool folowToHeadWidget, QWidget *parent = nullptr);
    static StatusTip *s_instance;

public:
    static StatusTip *getInstance(QWidget *parent = NULL) {
        if (s_instance == NULL) {
            s_instance = new StatusTip(true, parent);
        }
        return s_instance;
    }
    bool eventFilter(QObject *watched, QEvent *event) override;

protected:
    void showEvent(QShowEvent *event);

signals:

public slots:
    void showStatusTip(QString text);
private:
    int m_tipHeight;
    QPropertyAnimation *m_animation;
    QGraphicsOpacityEffect *m_effect;
};

#endif // STATUSTIP_H
