#include "userinfoview.h"
#include <QHeaderView>
#include <QMouseEvent>
#include <QDebug>
#include <QStyleOptionButton>
#include <QPushButton>
#include <QPainter>
#include <QApplication>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QGridLayout>
#include <QMessageBox>

UserInfoView::UserInfoView(QWidget *parent) : QTableView(parent)
{
    setModel(new TableModel(this));
//    setItemDelegate(new TableViewDelegate(this));
    //无法选中
    setSelectionMode(QAbstractItemView::NoSelection);
    //隐藏列头
    verticalHeader()->setVisible(false);
    //隐藏行头
    horizontalHeader()->setVisible(false);
    //自动拉伸最后一列
    horizontalHeader()->setStretchLastSection(false);
    //不显示网格
    setShowGrid(true);
    //表格边框风格
    setFrameShape(QFrame::NoFrame);
    //设置鼠标跟踪
    setMouseTracking(true);
    //行交替颜色
    setAlternatingRowColors(true);
    setStyleSheet("QTableView::item:selected{  \
                    background-color: #3399FF;    /*选中行颜色*/  \
                   }");

    setSpan(0, 3, 1, 2);
}

UserInfoView::~UserInfoView()
{
    qDebug("delete Tableview");
}

void UserInfoView::setDataSource(const QList<QStringList> &l)
{
    static_cast<TableModel *>(model())->setDataSource(l);
}

void UserInfoView::addData(const QStringList &data)
{
    static_cast<TableModel *>(model())->addData(data);

    const QList<QStringList> list = static_cast<TableModel *>(model())->getDataSource();
    QPushButton *btn1 = new QPushButton("Modify", this);
    btn1->setObjectName(data.at(0));
    connect(btn1, &QPushButton::clicked, this, &UserInfoView::handleModifyInfo);
    setIndexWidget(model()->index(list.length(), 3), btn1);
    QPushButton *btn2 = new QPushButton("DelUser", this);
    btn2->setObjectName(data.at(0));
    connect(btn2, &QPushButton::clicked, this, &UserInfoView::handleDelUserInfo);
    setIndexWidget(model()->index(list.length(), 4), btn2);
}

void UserInfoView::popupModifyInfoWidget()
{
    bool isOK = false;
    int index = sender()->objectName().toInt(&isOK);
    if(!isOK)
        return;
    QStringList data = static_cast<TableModel *>(model())->getDataSource().at(index);
    QStringList list;
    QDialog *modifyInfoWidget = new QDialog(this);
    modifyInfoWidget->setFixedSize(400, 150);

    QLabel *lbl1 = new QLabel("User：", modifyInfoWidget);
    QLineEdit *lineEdit1 = new QLineEdit(modifyInfoWidget);
    lineEdit1->setText(data.at(1));
    modifyInfoMap.insert("User", lineEdit1);

    QLabel *lbl2 = new QLabel("Group：", modifyInfoWidget);
    QComboBox *comboBox1 = new QComboBox(modifyInfoWidget);
    list << "Supervisor" << "Manager" << "User";

    comboBox1->addItems(list);
    comboBox1->setCurrentText(data.at(2));
    list.clear();
    modifyInfoMap.insert("Group", comboBox1);

    QLabel *lbl3 = new QLabel("Password：", modifyInfoWidget);
    QLineEdit *lineEdit2 = new QLineEdit(modifyInfoWidget);
    modifyInfoMap.insert("Password", lineEdit2);

    QLabel *lbl4 = new QLabel("Confirm：", modifyInfoWidget);
    QLineEdit *lineEdit3 = new QLineEdit(modifyInfoWidget);
    modifyInfoMap.insert("Confirm", lineEdit3);

    QPushButton *btn = new QPushButton("Save", modifyInfoWidget);
    btn->setFixedWidth(50);

    QGridLayout *layout1 = new QGridLayout;
    layout1->addWidget(lbl1,      0, 0, 1, 1);
    layout1->addWidget(lineEdit1, 0, 1, 1, 2);

    layout1->addWidget(lbl2,      1, 0, 1, 1);
    layout1->addWidget(comboBox1, 1, 1, 1, 2);

    layout1->addWidget(lbl3,      2, 0, 1, 1);
    layout1->addWidget(lineEdit2, 2, 1, 1, 2);

    layout1->addWidget(lbl4,      3, 0, 1, 1);
    layout1->addWidget(lineEdit3, 3, 1, 1, 2);

    layout1->addWidget(btn,       4, 0, 1, 3, Qt::AlignCenter);

    QVBoxLayout *layout2 = new QVBoxLayout;
    layout2->addLayout(layout1);
    layout2->addStretch();

    QHBoxLayout *layout3 = new QHBoxLayout(modifyInfoWidget);
    layout3->addStretch(1);
    layout3->addLayout(layout2, 4);
    layout3->addStretch(1);

    modifyInfoWidget->exec();

    qDebug() << lineEdit1->text() << lineEdit2->text() << lineEdit3->text() << comboBox1->itemText(comboBox1->currentIndex());
    delete modifyInfoWidget;
}

void UserInfoView::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        QModelIndex index = indexAt(event->pos());
        if(index.isValid()){
            QTableView::mousePressEvent(event);
        }
    }
}

void UserInfoView::handleModifyInfo()
{
    qDebug() << "Modify" << sender()->objectName();
    popupModifyInfoWidget();
}

void UserInfoView::handleDelUserInfo()
{
    qDebug() << "DelUser" << sender()->objectName();
    qDebug() << QMessageBox::question(this, "警告", QString("是否删除用户%1").arg(static_cast<TableModel *>(model())->getDataSource().at(sender()->objectName().toInt()).at(1)));
}

TableModel::TableModel(QObject *parent) : QAbstractTableModel(parent),
    column(5)
{
    headList << "No." << "User" << "Group" << "Operate";
}

TableModel::~TableModel()
{

}

int TableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return list.size() + 1;
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return column;
}

QVariant TableModel::data(const QModelIndex &index, int role) const
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
            {
                if(index.row() == 0){
                    return headList.at(index.column());
                }
                if(0 == index.column())
                    return list.at(index.row() - 1).at(0);
                else if(1 == index.column())
                    return list.at(index.row() - 1).at(1);
                else if(2 == index.column())
                    return list.at(index.row() - 1).at(2);
            }
            break;
        //显示的字体样式
        case Qt::FontRole:
            if(index.row() == 0){
                QFont font;
                font.setBold(true);
                font.setPixelSize(16);
                return font;
            }else{
                QFont font;
                font.setPixelSize(12);
                return font;
            }
            break;
        //文本对齐方式
        case Qt::TextAlignmentRole:
            return Qt::AlignCenter;
        //前景色
        case Qt::ForegroundRole:
            return QColor(0, 0, 0);
        //背景色
        case Qt::BackgroundRole:
            if(index.row() == 0){
                return QBrush(QColor(51, 103, 155));
            }else{
                return QBrush(QColor(255, 255, 255));
            }
        case Qt::SizeHintRole:
            return QSize(100, 100);
            break;
        default:
            break;
    }

    return QVariant();
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    if(!index.isValid()){
        return Qt::NoItemFlags;
    }

    Qt::ItemFlags flags = QAbstractTableModel::flags(index);

    if(index.row() != 0 && (index.column() == 3 || index.column() == 4))
        flags |= Qt::ItemIsEditable;

    return flags;
}

void TableModel::addData(const QStringList &data)
{
    list.append(data);
}

void TableModel::setDataSource(const QList<QStringList> &l)
{
    list = l;
}


TableViewDelegate::TableViewDelegate(QObject *parent):QItemDelegate(parent)
{

}

void TableViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(!index.isValid())
        return;

    if(index.row() != 0){
        QStyleOptionButton *btnOption = btnOptions.value(index, NULL);

        if(!btnOption){
            if(index.column() == 3){
                btnOption = new QStyleOptionButton;
                btnOption->state |= QStyle::State_Enabled;
                btnOption->text = QString("Modify");
                btnOption->rect = option.rect.adjusted(4, 2, -4, -2);

                (const_cast<TableViewDelegate *>(this))->btnOptions.insert(index, btnOption);
            }else if(index.column() == 4){
                btnOption = new QStyleOptionButton;
                btnOption->state |= QStyle::State_Enabled;
                btnOption->text = QString("DelUser");
                btnOption->rect = option.rect.adjusted(4, 2, -4, -2);

                (const_cast<TableViewDelegate *>(this))->btnOptions.insert(index, btnOption);
            }
        }

        if(btnOption){
            painter->save();
            btnOption->rect = option.rect.adjusted(4, 2, -4, -2);
            UserInfoView *view = static_cast<UserInfoView *>(const_cast<QWidget *>(option.widget));
            view->style()->drawControl(QStyle::CE_PushButton, btnOption, painter);
            painter->restore();
            return;
        }
    }
    QItemDelegate::paint(painter, option, index);
}

bool TableViewDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                                    const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent* e =(QMouseEvent*)event;

        if (option.rect.adjusted(4, 2, -4, -2).contains(e->x(), e->y()) && btnOptions.contains(index)) {
            btnOptions.value(index)->state |= QStyle::State_Sunken;
        }
    }
    if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent* e =(QMouseEvent*)event;

        if (option.rect.adjusted(4, 2, -4, -2).contains(e->x(), e->y()) && btnOptions.contains(index)) {
            btnOptions.value(index)->state &= (~QStyle::State_Sunken);
        }
    }
    return QItemDelegate::editorEvent(event, model, option, index);
}
