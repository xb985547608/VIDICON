#ifndef LISTVIEW_H
#define LISTVIEW_H

/**
 * @brief         一个简易的列表视图
 * @author        xiaobin <xiaobin@sunniwell.com>
 * @date          20180504
 */

#include <QListView>
#include <QAbstractListModel>

class ListModel;

class ListView : public QListView
{
    Q_OBJECT
public:
    explicit ListView(QWidget *parent = Q_NULLPTR);
    ~ListView();

    void addItems(const QStringList &items);
protected:
    void mousePressEvent(QMouseEvent *event) override;
};

class ListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ListModel(QObject *parent = 0);
    ~ListModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
private:
    QStringList m_items;
};

#endif // LISTVIEW_H
