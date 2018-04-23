#include "downloadinfoview.h"
#include <QDebug>
#include <QHeaderView>
#include <QPainter>
#include <QProgressBar>
#include <QApplication>
#include <QContextMenuEvent>
#include <QMessageBox>

DownloadInfoView::DownloadInfoView(QWidget *parent) :
    QTableView(parent)
{
    QSortFilterProxyModel *proxy = new QSortFilterProxyModel(this);
    DownloadInfoModel *model = new DownloadInfoModel(proxy, this);
    proxy->setSourceModel(model);
    proxy->sort(1);
    setModel(proxy);

    //设置自定义委托
    setItemDelegate(new DownloadInfoDelegate(this));
    //列宽自适应缩放填充
    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //单行选中
    setSelectionBehavior(QAbstractItemView::SelectRows);
    //选择模式-->无法选择
    setSelectionMode(QTableView::NoSelection);
    //无法编辑
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    //表头不高亮
    horizontalHeader()->setHighlightSections(false);
    horizontalHeader()->setSortIndicatorShown(false);
    horizontalHeader()->setSectionsClickable(false);
    //隐藏列头
    verticalHeader()->setVisible(false);
    //显示网格
    setShowGrid(true);
    //外框风格-->无边框
    setFrameShape(QFrame::NoFrame);

    connect(this->model(), &QAbstractItemModel::dataChanged, this, &DownloadInfoView::reset);

    setStyleSheet("QProgressBar{\
                      border: 1px solid rgb(210, 225, 240);\
                      border-radius:6px;\
                      text-align: center;\
                      background: rgb(210, 225, 240);\
                  }\
                  QProgressBar::chunk {\
                      border: 1px solid rgb(0, 160, 230);\
                      border-radius:5px;\
                      background: rgb(0, 160, 230);\
                  }");

    createActions();
    reset();
}

void DownloadInfoView::addData(QString fileName, int state, int progress)
{
    QSortFilterProxyModel *proxy = static_cast<QSortFilterProxyModel *>(this->model());
    DownloadInfoModel *model = static_cast<DownloadInfoModel *>(proxy->sourceModel());
    model->addData(fileName, state, progress);
}

QVariant DownloadInfoView::data(const QModelIndex &index, int role)
{
    return model()->data(index, role);
}

QVariant DownloadInfoView::data(int row, int column, int role)
{
    return model()->data(model()->index(row, column), role);
}

bool DownloadInfoView::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return model()->setData(index, value, role);
}

void DownloadInfoView::setData(QString fileName, int state, int progress)
{
    if(model()->rowCount() == 0)
        return;
    for(int i=0; i<model()->rowCount(); i++) {
        if(model()->data(model()->index(i, 3)).toString().compare(fileName) == 0) {
            model()->setData(model()->index(i, 1), state);
            model()->setData(model()->index(i, 2), progress);
        }
    }
}

//创建菜单和菜单项
void DownloadInfoView::createActions()
{
    m_popMenu = new QMenu(this);

    m_pauseAction = new QAction(m_popMenu);
    m_pauseAction->setText("暂停下载");
    m_pauseAction->setIcon(QIcon(":/images/pause.png"));
    connect(m_pauseAction, &QAction::triggered, this, [this](){
        QModelIndex index = model()->index(m_pointToRow, 1);
        if(data(index).toInt() == Downloading || data(index).toInt() == Waiting) {
            QString file = data(model()->index(m_pointToRow, 3)).toString();
            emit signalCancelDownload(file);
            setData(model()->index(m_pointToRow, 1), Pause);
        }
    });

    m_cancelDownloadAction = new QAction(m_popMenu);
    m_cancelDownloadAction->setText("取消下载");
    m_cancelDownloadAction->setIcon(QIcon(":/images/cancel.png"));
    connect(m_cancelDownloadAction, &QAction::triggered, this, [this](){
        QModelIndex index = model()->index(m_pointToRow, 1);
        if(data(index).toInt() == Downloading) {
            QString file = data(model()->index(m_pointToRow, 3)).toString();
            emit signalCancelDownload(file);
        }
    });

    m_redownloadAction = new QAction(m_popMenu);
    m_redownloadAction->setText("重新下载");
    m_redownloadAction->setIcon(QIcon(":/images/download.png"));
    connect(m_redownloadAction, &QAction::triggered, this, [this](){
        QModelIndex index = model()->index(m_pointToRow, 1);
        if(data(index).toInt() != Downloading) {
            setData(index, Waiting);
        }
    });

    m_deleteAction = new QAction(m_popMenu);
    m_deleteAction->setText("删除");
    m_deleteAction->setIcon(QIcon(":/images/delete.png"));
    connect(m_deleteAction, &QAction::triggered, this, [this](){
        QModelIndex index = model()->index(m_pointToRow, 1);
        if(QMessageBox::warning(this, "警告", "是否删除该下载任务", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
            model()->removeRow(index.row());
        }
    });

    m_popMenu->addAction(m_pauseAction);
    m_popMenu->addAction(m_cancelDownloadAction);
    m_popMenu->addAction(m_redownloadAction);
    m_popMenu->addAction(m_deleteAction);
}

void DownloadInfoView::contextMenuEvent(QContextMenuEvent *event)
{
    QPoint pos = mapFromGlobal(QCursor::pos());
    pos.setY(pos.y() - horizontalHeader()->height());

    QModelIndex index = indexAt(pos);
    if(index.isValid()) {
        m_pointToRow = index.row();
        index = model()->index(m_pointToRow, 1);
        if(data(index).toInt() == Downloading || data(index).toInt() == Waiting) {
            m_pauseAction->setEnabled(true);
            m_cancelDownloadAction->setEnabled(true);
            m_pauseAction->setEnabled(m_pointToRow != 0);
        }else {
            m_pauseAction->setEnabled(false);
            m_cancelDownloadAction->setEnabled(false);
        }
        m_popMenu->exec(QCursor::pos());
        event->accept();
    }
}

void DownloadInfoView::reset()
{
    horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
    setColumnWidth(0, 40);
    setColumnWidth(1, 40);
    setColumnWidth(2, 200);
}

DownloadInfoDelegate::DownloadInfoDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
    m_downloadingPixmap = QPixmap(":/images/downloading.png");
    m_waitingPixmap = QPixmap(":/images/waiting.png");
    m_pausePixmap = QPixmap(":/images/pause.png");
    m_errorPixmap = QPixmap(":/images/error.png");
    m_finishedPixmap = QPixmap(":/images/finished.png");
}

void DownloadInfoDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);
    viewOption.displayAlignment = Qt::AlignCenter;
    //去虚线框
    if (option.state.testFlag(QStyle::State_HasFocus))
        viewOption.state = viewOption.state ^ QStyle::State_HasFocus;

    //绘制代表状态的图标
    if(index.column() == 1) {
        drawBackground(painter, viewOption, index);
        int margin = 5;
        QRect rect = viewOption.rect.adjusted(margin * 2, margin, -margin * 2, -margin);
        int status = index.model()->data(index, Qt::DisplayRole).toInt();
        painter->setRenderHint(QPainter::SmoothPixmapTransform);

        if(status == Downloading) {
            painter->drawPixmap(rect, m_downloadingPixmap);
        }else if(status == Waiting) {
            painter->drawPixmap(rect, m_waitingPixmap);
        }else if(status == Pause) {
            painter->drawPixmap(rect, m_pausePixmap);
        }else if(status == Error) {
            painter->drawPixmap(rect, m_errorPixmap);
        }else if(status == Finished) {
            painter->drawPixmap(rect, m_finishedPixmap);
        }
    //绘制进度条
    }else if(index.column() == 2) {
        drawBackground(painter, viewOption, index);

        int nProgress = index.model()->data(index, Qt::DisplayRole).toInt();
        int nLeft = 8;
        int nTop = 8;
        int nWidth = viewOption.rect.width() - 2 * nLeft;
        int nHeight = viewOption.rect.height() - 2 * nTop;

        // 设置进度条的风格
        QStyleOptionProgressBar progressBarOption;
        progressBarOption.initFrom(viewOption.widget);
        // 设置进度条显示的区域
        progressBarOption.rect = QRect(viewOption.rect.left() + nLeft, viewOption.rect.top() + nTop,  nWidth, nHeight);
        // 设置最小值
        progressBarOption.minimum = 0;
        // 设置最大值
        progressBarOption.maximum = 100;
        // 设置对齐方式
        progressBarOption.textAlignment = Qt::AlignCenter;
        // 设置进度
        progressBarOption.progress = nProgress;
        // 设置文本（百分比）
        progressBarOption.text = QString("%1%").arg(nProgress);
        // 设置文本可见
        progressBarOption.textVisible = true;

        QWidget *w = const_cast<QWidget *>(viewOption.widget);
        QProgressBar progressBar(w);
        //绘制进度条
        w->style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter, &progressBar);
    }else {
        QStyledItemDelegate::paint(painter, viewOption, index);
    }
}

void DownloadInfoDelegate::drawBackground(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (option.showDecorationSelected && (option.state & QStyle::State_Selected)) {
        QPalette::ColorGroup cg = (option.state & QStyle::State_Enabled) ? QPalette::Normal : QPalette::Disabled;
        if (cg == QPalette::Normal && !(option.state & QStyle::State_Active))
            cg = QPalette::Inactive;

        painter->fillRect(option.rect, option.palette.brush(cg, QPalette::Highlight));
    } else {
        QVariant value = index.data(Qt::BackgroundRole);
        if (value.canConvert<QBrush>()) {
            QPointF oldBO = painter->brushOrigin();
            painter->setBrushOrigin(option.rect.topLeft());
            painter->fillRect(option.rect, qvariant_cast<QBrush>(value));
            painter->setBrushOrigin(oldBO);
        }
    }
}

DownloadInfoModel::DownloadInfoModel(QSortFilterProxyModel *proxy, QObject *parent) :
    QAbstractTableModel(parent),
    m_column(4),
    m_proxy(proxy)
{
    m_items.clear();
    m_headItems << "序号" << "状态" << "进度" << "文件名";
}

DownloadInfoModel::~DownloadInfoModel()
{

}
//新增数据
void DownloadInfoModel::addData(QString fileName, int state, int progress)
{
    DownloadInfo info;
    info.state = state;
    info.progress = progress;
    info.fileName = fileName;
    m_items.append(info);

    int row = rowCount() - 1;
    beginInsertRows(QModelIndex(), row, row);
    insertRows(row, 1);
    endInsertRows();
    qDebug()<< rowCount();
}

int DownloadInfoModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_items.length();
}

int DownloadInfoModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_column;
}

QVariant DownloadInfoModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid()) {
        return QVariant();
    }

    switch(role){
    //提示信息显示的数据
    case Qt::ToolTipRole: {
        if(1 == index.column()) {
            switch(m_items.at(index.row()).state) {
            case Downloading:
                return "正在下载";
            case Waiting:
                return "等待下载";
            case Pause:
                return "暂停下载";
            case Error:
                return "下载出错";
            case Finished:
                return "下载完成";
            }
        }
        break;
    }
    //编辑时显示的数据
    case Qt::EditRole:
    //正常时显示的数据
    case Qt::DisplayRole: {
        if(0 == index.column())
            return m_proxy->mapFromSource(index).row()+1;
        else if(1 == index.column())
            return m_items.at(index.row()).state;
        else if(2 == index.column())
            return m_items.at(index.row()).progress;
        else if(3 == index.column())
            return m_items.at(index.row()).fileName;
        break;
    }
    //显示的字体样式
    case Qt::FontRole: {
        QFont font;
        font.setPixelSize(12);
        return font;
    }
    //文本对齐方式
    case Qt::TextAlignmentRole:
        return Qt::AlignCenter;
    default:
        break;
    }

    return QVariant();
}

bool DownloadInfoModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    switch (role) {
        case Qt::EditRole:
        case Qt::DisplayRole: {
            if(index.row() >= rowCount()) {
                DownloadInfo d;
                m_items.append(d);
            }
            if(1 == index.column())
                m_items[index.row()].state = value.toInt();
            else if(2 == index.column())
                m_items[index.row()].progress = value.toInt();
            else if(3 == index.column())
                m_items[index.row()].fileName = value.toString();
            emit dataChanged(index, index);
            return true;
        }
        default:
            return QAbstractTableModel::setData(index, value, role);
    }
    return false;
}

QVariant DownloadInfoModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch(role){
    //显示的数据
    case Qt::DisplayRole:{
        if(Qt::Horizontal == orientation){
            if(0 <= section && section < m_headItems.size()){
                return m_headItems.at(section);
            }
        }
        break;
    }
    //显示的字体样式
    case Qt::FontRole:{
        QFont font;
        font.setPixelSize(14);
        return font;
    }
    //文本对齐方式
    case Qt::TextAlignmentRole:
        return Qt::AlignCenter;
    default:
        return QAbstractTableModel::headerData(section, orientation, role);
    }

    return QVariant();
}

Qt::ItemFlags DownloadInfoModel::flags(const QModelIndex &index) const
{
    if(!index.isValid()) {
        return Qt::NoItemFlags;
    }

//    Qt::ItemFlags flags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index);
}

bool DownloadInfoModel::removeRows(int row, int count, const QModelIndex &parent)
{
    for(int i=row; i<row+count; i++) {
        m_items.removeAt(row);
    }
    beginRemoveRows(parent, row, row+count-1);
    endRemoveRows();
    return true;
}
