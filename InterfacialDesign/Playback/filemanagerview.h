#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QTableView>
#include <QItemDelegate>
#include <QDialog>
#include <QMap>
#include <QHeaderView>

#define CHECKBOXCOLUMN 0
#define OPERATIONCOLUMN 2
#define PRESSEDROLE   Qt::UserRole + 1
#define MOVINGROLE   Qt::UserRole + 2
#define RESETMOVINGALLROLE   Qt::UserRole + 3

class FileView;

class FileModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    typedef struct FileInfo{
        int CheckState;
        QString fileName;
        bool fileState;
        bool bPressed;
    }FileInfo;

    explicit FileModel(QObject *parent = 0);
    ~FileModel();

    void setDataSource(const QStringList &l);
    const QList<FileInfo> &getDataSource() { return fileList; }
    void checkState();

protected:
    //获取行数
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    //获取列数
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    //获取指定角色的数据数据
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
signals:
    void signalStateChange(int state);
public slots:
    void handlerStateChange(int state);

private:
    int column;

    QList<FileInfo> fileList;
    QStringList headList;
};

class FileView : public QTableView
{
    Q_OBJECT
public:
    explicit FileView(QWidget *parent = Q_NULLPTR);
    ~FileView();

    void setDataSource(const QStringList &list);
    const QList<FileModel::FileInfo> &dataSource();

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

public slots:
    void handlerReceiveImage(QPixmap *pixmap);

private:
    QList<QPushButton *> btnList;
};

class FileViewDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit FileViewDelegate(QObject *parent = 0);

protected:
    // painting
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model,
                     const QStyleOptionViewItem &option, const QModelIndex &index) Q_DECL_OVERRIDE;
    void drawBackground(QPainter *painter,
                           const QStyleOptionViewItem &option,
                           const QModelIndex &index) const;
};
//主要实现表头复选框功能
class FileViewHeaderView : public QHeaderView
{
    Q_OBJECT
public:
    FileViewHeaderView(Qt::Orientation orientation, QWidget *parent);

protected:
    virtual void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const;
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual bool event(QEvent *event);
signals:
    void signalStateChange(int state);
public slots:
    void handlerStateChange(int state);
private:
    bool bPressed;
    bool bMoving;
    bool bTristate;
    bool bChecked;
};

#endif // TABLEVIEW_H
