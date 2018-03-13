#ifndef TABLEVIEW_H
#define TABLEVIEW_H

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

    void setDataSource(const QList<VidiconProtocol::UserConfigInfo> &l);

    void initModifyInfoWidget();
    void initAddUserInfoWidget();
signals:
    void signalSetParameter(int type, void *param = NULL, QString SessionID = "R00001");

protected:
    virtual void mousePressEvent(QMouseEvent *event);

public slots:
    void handleModifyInfo();
    void handleDelUserInfo();
    void handleAddUserInfo();

private:
    QDialog *modifyInfoWidget;
    QMap<QString, QWidget *> modifyInfoMap;

    QDialog *addUserInfoWidget;
    QMap<QString, QWidget *> addUserInfoMap;
};

class UserInfoModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit UserInfoModel(QObject *parent = 0);
    ~UserInfoModel();

    void setDataSource(const QList<VidiconProtocol::UserConfigInfo> &l);

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
    int column;
    QList<VidiconProtocol::UserConfigInfo> list;
    QStringList headList;
};

#endif // TABLEVIEW_H
