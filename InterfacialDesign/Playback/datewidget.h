#ifndef DATEWIDGET_H
#define DATEWIDGET_H

#include <QWidget>
#include <QDate>
#include <QMap>
#include <QLabel>
#include <QDateEdit>
#include <QComboBox>
class DateWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DateWidget(QWidget *parent = nullptr);

protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void enterEvent(QEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);

signals:
    void signalSetParameter(int type, void *param, QString SessionID = "R00001");
    void signalGetParameter(int type, int StreamType = 0, int Channel = 0, QString SessionID = "R00001");
    void signalDateChange(QDate date);

public slots:
    void handlerReceiveData(int type, QByteArray data);
    void handlerDateChangle(const QDate &date);

private:
    QMap<int, QLabel *> lblDateMap;
    QDateEdit *dateEdit;
    QMap<int, int> MonthMap;
//    QComboBox *typeSelect;
};

#endif // DATEWIDGET_H
