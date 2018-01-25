#include "boxwidget.h"
#include <QHeaderView>
#include <QMouseEvent>

BoxView::BoxView(QWidget *parent) : QTableView(parent),
    model(new BoxModel(this))
{
    //设置单行选中
    setSelectionBehavior(QAbstractItemView::SelectRows);
    //隐藏列头
    verticalHeader()->setVisible(false);
    //隐藏行头
    horizontalHeader()->setVisible(false);
    horizontalHeader()->setStretchLastSection(true);
    //列表网格显示
    setShowGrid(false);
    //设置列表网格风格
    setGridStyle(Qt::NoPen);
    //表格边框风格
    setFrameShape(QFrame::NoFrame);
    //设置鼠标跟踪
    setMouseTracking(true);
    //行交替颜色
    setAlternatingRowColors(true);    
    setStyleSheet("QTableView::item:selected{  \
                    background-color: #3399FF;    /*选中行颜色*/  \
                   }");

}

BoxView::~BoxView()
{
    qDebug("delete BoxView");
}

void BoxView::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        QModelIndex index = indexAt(event->pos());
        if(index.isValid()){
            QTableView::mousePressEvent(event);
        }
    }
}

void BoxView::setData(const QStringList &list)
{
    model->setDataSource(list);
    setModel(model);
}

void BoxView::hanlderSwitchRow(int row)
{
    setCurrentIndex(model->index(row, 0));
}

BoxModel::BoxModel(QObject *parent) : QAbstractTableModel(parent),
    column(1)
{
}

BoxModel::~BoxModel()
{

}

int BoxModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return list.size();
}

int BoxModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return column;
}

QVariant BoxModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid()){
        return QVariant();
    }

    switch(role){
        //提示信息显示的数据
        case Qt::ToolTipRole:
        //编辑时显示的数据
        case Qt::EditRole:
        //正常时显示的数据
        case Qt::DisplayRole:
            if(index.row() < list.size()){
                if(0 == index.column())
                    return list.at(index.row());

                if(1 == index.column())
                    return list.at(index.row());

                if(2 == index.column())
                    return list.at(index.row());
            }
            break;
        //显示的字体样式
        case Qt::FontRole:
            {
                QFont font;
                font.setPixelSize(12);
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
        default:
            break;
    }

    return QVariant();
}

void BoxModel::setDataSource(const QStringList &l)
{
    list = l;
}

