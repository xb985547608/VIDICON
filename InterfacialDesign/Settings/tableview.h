#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QTableView>
#include <QItemDelegate>
#include <QDialog>
#include <QMap>

class TableModel;

class TableView : public QTableView
{
    Q_OBJECT
public:
    TableView(QWidget *parent = Q_NULLPTR);
    ~TableView();

    void setDataSource(const QList<QStringList> &l);
    void addData(const QStringList &data);
    void popupModifyInfoWidget();

protected:
    virtual void mousePressEvent(QMouseEvent *event);
public slots:
    void handlerModifyInfo();
    void handlerDelUserInfo();
private:
    QDialog *modifyInfoWidget;
    QMap<QString, QWidget *> modifyInfoMap;
};

class TableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit TableModel(QObject *parent = 0);
    ~TableModel();

    void setDataSource(const QList<QStringList> &l);
    void addData(const QStringList &data);
    const QList<QStringList> &getDataSource() { return list; }
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
    QList<QStringList> list;
    QStringList headList;
};


class TableViewDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    TableViewDelegate(QObject *parent = 0);

    // painting
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model,
                     const QStyleOptionViewItem &option, const QModelIndex &index) Q_DECL_OVERRIDE;

    QMap<QModelIndex, QStyleOptionButton *> btnOptions;
};

#endif // TABLEVIEW_H
