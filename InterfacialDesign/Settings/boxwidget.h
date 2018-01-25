#ifndef LISTVIEW_H
#define LISTVIEW_H

#include <QTableView>

class BoxModel;

class BoxView : public QTableView
{
    Q_OBJECT
public:
    BoxView(QWidget *parent = Q_NULLPTR);
    ~BoxView();

    void setData(const QStringList &list);
    BoxModel *getModel() const{ return model; }
protected:
    virtual void mousePressEvent(QMouseEvent *event);
public slots:
    void hanlderSwitchRow(int row);

private:
    BoxModel *model;
};

class BoxModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit BoxModel(QObject *parent = 0);
    ~BoxModel();

    //获取行数
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    //获取列数
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    //获取指定角色的数据数据
    virtual QVariant data(const QModelIndex &index, int role) const;

    void setDataSource(const QStringList &l);
    void setColumnCount(int c) { column = c; }

signals:

public slots:

private:
    int row;
    int column;
    QStringList list;
};

#endif // LISTVIEW_H
