#ifndef WAITINGDIALOG_H
#define WAITINGDIALOG_H

#include <QDialog>

class WaitingDialog : public QDialog
{
    Q_OBJECT
public:
    explicit WaitingDialog(QWidget *parent = nullptr);

signals:

public slots:
};

#endif // WAITINGDIALOG_H
