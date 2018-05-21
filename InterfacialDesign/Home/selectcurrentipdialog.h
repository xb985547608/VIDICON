#ifndef SELECTCURRENTIPDIALOG_H
#define SELECTCURRENTIPDIALOG_H

/**
 * @brief         弹出对话框供其选择指定的IP
 * @author        xiaobin <xiaobin@sunniwell.com>
 * @date          20180504
 */

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

    int getNetWorkInterfaceCount() {return m_allInterface.count();}
    QHostAddress getCurrentSelectIp();
    
private:
    Ui::SelectCurrentIpDialog *m_ui;
    QList<QHostAddress> m_allInterface;
};

#endif // SELECTCURRENTIPDIALOG_H
