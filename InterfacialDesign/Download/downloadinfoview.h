#ifndef DOWNLOADINFOVIEW_H
#define DOWNLOADINFOVIEW_H

#include <QTableView>
#include <QStyledItemDelegate>
#include <QSortFilterProxyModel>
#include <QMenu>
#include <QAction>
#include "Network/httpdownload.h"

class DownloadInfoView : public QTableView
{
    Q_OBJECT
public:
    explicit DownloadInfoView(QWidget *parent = nullptr);
    void addData(QString fileName, int state = Waiting, int progress = 0);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole);
    QVariant data(int row, int column, int role = Qt::DisplayRole);
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::DisplayRole);
    void setData(QString fileName, int state, int progress = 0);

    void createActions();
signals:
    void signalCancelDownload(QString file);
public slots:
    void contextMenuEvent(QContextMenuEvent *event) override;
    void reset();
private:
    QMenu *m_popMenu;
    QAction *m_cancelDownloadAction;
    QAction *m_redownloadAction;
    QAction *m_deleteAction;
    QAction *m_pauseAction;
    int m_pointToRow;//弹出菜单所在的行
};

class DownloadInfoModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    typedef struct DownloadInfo{
        int state;
        int progress;
        QString fileName;
    }DownloadInfo;

    explicit DownloadInfoModel(QSortFilterProxyModel *m_proxy, QObject *parent = 0);
    ~DownloadInfoModel();
    void addData(QString fileName, int state = Waiting, int progress = 0);

protected:
    //获取行数
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    //获取列数
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    //获取指定角色的数据数据
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::DisplayRole);
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

private:
    int m_column;
    QList<DownloadInfo> m_items;
    QStringList m_headItems;
    QSortFilterProxyModel *m_proxy;
};

class DownloadInfoDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit DownloadInfoDelegate(QObject *parent = 0);

protected:
    // painting
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
    void drawBackground(QPainter *painter,
                           const QStyleOptionViewItem &option,
                           const QModelIndex &index) const;

private:
    QPixmap m_downloadingPixmap;
    QPixmap m_waitingPixmap;
    QPixmap m_pausePixmap;
    QPixmap m_errorPixmap;
    QPixmap m_finishedPixmap;
};

#endif // DOWNLOADINFOVIEW_H
