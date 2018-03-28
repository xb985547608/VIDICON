#ifndef DATEWIDGET_H
#define DATEWIDGET_H

#include <QWidget>
#include <QDate>
#include <QMap>
#include <QLabel>
#include <QDateEdit>
#include <QComboBox>
#include "basicwidget.h"

class DateWidget : public BasicWidget
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
    void signalDateChange(QDate date);

public slots:
    void refresh(QDate date = QDate());

    void handleReceiveData(VidiconProtocol::Type type, QByteArray data);

    void handleDateChangle(const QDate &date);

private:
    QMap<int, QLabel *> m_lblDateMap;
    QDateEdit *m_dateEdit;
    QMap<int, int> m_MonthMap;
//    QComboBox *typeSelect;
};

#endif // DATEWIDGET_H
