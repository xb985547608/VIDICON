#ifndef TABLEVIEW_H
#define TABLEVIEW_H

/**
 * @brief         显示当前登录设备的所有的账号信息
 * @author        xiaobin <xiaobin@sunniwell.com>
 * @date          20180504
 */

#include <QTableView>
#include <QItemDelegate>
#include <QDialog>
#include <QMap>
#include "Protocol/vidiconprotocol.h"

class UserInfoModel;

class UserInfoView : public QTableView
{
    Q_OBJECT
public:
    explicit UserInfoView(QWidget *parent = Q_NULLPTR);
    ~UserInfoView();

    void setItems(const QList<UserConfigInfo> &l);

    void initModifyInfoWidget();
    void initAddUserInfoWidget();
signals:
    void signalSetParameter(VidiconProtocol::Type type, QVariant param = QVariant(), QString SessionID = "R00001");

protected:
    virtual void mousePressEvent(QMouseEvent *event);

public slots:
    void handleModifyInfo();
    void handleDelUserInfo();
    void handleAddUserInfo();

private:
    QDialog *m_modifyInfoWidget;
    QMap<QString, QWidget *> m_modifyInfoMap;

    QDialog *m_addUserInfoWidget;
    QMap<QString, QWidget *> m_addUserInfoMap;
};

class UserInfoModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit UserInfoModel(QObject *parent = 0);
    ~UserInfoModel();

    void setItems(const QList<UserConfigInfo> &l);

protected:
    //获取行数
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    //获取列数
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    //获取指定角色的数据数据
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
signals:

public slots:

private:
    int m_column;
    QList<UserConfigInfo> m_items;
    QStringList m_headItems;
};

#endif // TABLEVIEW_H
