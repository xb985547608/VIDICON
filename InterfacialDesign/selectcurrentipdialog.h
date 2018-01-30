#ifndef SELECTCURRENTIPDIALOG_H
#define SELECTCURRENTIPDIALOG_H

#include <QDialog>
#include <QNetworkInterface>
#include <QHostAddress>

namespace Ui {
class SelectCurrentIpDialog;
}

class SelectCurrentIpDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SelectCurrentIpDialog(QWidget *parent = 0);
    ~SelectCurrentIpDialog();

    int getNetWorkInterfaceCount() {return allInterface.count();}
    QHostAddress getCurrentSelectIp();
    
private:
    Ui::SelectCurrentIpDialog *ui;
    QList<QHostAddress> allInterface;
};

#endif // SELECTCURRENTIPDIALOG_H
