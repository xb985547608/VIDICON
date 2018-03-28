#ifndef BASICWIDGET_H
#define BASICWIDGET_H

#include <QWidget>
#include "Protocol/vidiconprotocol.h"

class BasicWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BasicWidget(QWidget *parent = nullptr);

signals:
    void signalSetParameter(VidiconProtocol::Type type, void *param = NULL, QString SessionID = "R00001");
    void signalGetParameter(VidiconProtocol::Type type, void *param = NULL, QString SessionID = "R00001");

public slots:
    virtual void handleReceiveData(VidiconProtocol::Type type, QByteArray data) = 0;

};

#endif // BASICWIDGET_H
