#include "listview.h"
#include <QHeaderView>
#include <QMouseEvent>

ListView::ListView(QWidget *parent) :
    QListView(parent)
{
    setModel(new ListModel(this));
    //设置单行选中
    setSelectionBehavior(QAbstractItemView::SelectRows);
    //表格边框风格
    setFrameShape(QFrame::NoFrame);
    //设置鼠标跟踪
    setMouseTracking(true);
    //行交替颜色
    setAlternatingRowColors(true);    
    setStyleSheet("QListView::item:selected{  "
                    "background-color: #3399FF;    /*选中行颜色*/  "
                   "}");
}

ListView::~ListView()
{
}

void ListView::addItems(const QStringList &items)
{
    foreach (QString item, items) {
        int count = model()->rowCount();
        model()->insertRow(count);
        model()->setData(model()->index(count, 0), item);
    }
}

void ListView::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        QModelIndex index = indexAt(event->pos());
        if(index.isValid()){
            QListView::mousePressEvent(event);
        }
    }
}

ListModel::ListModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

ListModel::~ListModel()
{
}

int ListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_items.length();
}

int ListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant ListModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid()){
        return QVariant();
    }

    switch(role) {
        //提示信息显示的数据
        case Qt::ToolTipRole:
        //编辑时显示的数据
        case Qt::EditRole:
        //正常时显示的数据
        case Qt::DisplayRole:
            return m_items.at(index.row());
        //显示的字体样式
        case Qt::FontRole: {
                QFont font;
                font.setPixelSize(14);
                return font;
            }
        //文本对齐方式
        case Qt::TextAlignmentRole:
            return Qt::AlignCenter;
//        //前景色
//        case Qt::ForegroundRole:
//            return QColor(51, 103, 155);
//        //背景色
//        case Qt::BackgroundRole:
//            return QBrush(QColor(255, 255, 255));
        case Qt::SizeHintRole:
            return QSize(1, 30);
        default:
            break;
    }

    return QVariant();
}

bool ListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid()){
        return false;
    }

    switch(role){
        //提示信息显示的数据
        case Qt::ToolTipRole:
        //编辑时显示的数据
        case Qt::EditRole:
        //正常时显示的数据
        case Qt::DisplayRole: {
            m_items.replace(index.row(), value.toString());
            emit dataChanged(index, index);
            return true;
        }
        default:
            break;
    }
    return false;
}

bool ListModel::insertRows(int row, int count, const QModelIndex &parent)
{
    for (int i=0; i<count; i++) {
        m_items.insert(row + 0, "");
    }
    beginInsertRows(parent, row, row + count);
    endInsertRows();
    return true;
}
