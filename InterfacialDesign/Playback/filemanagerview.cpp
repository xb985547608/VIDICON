#include "filemanagerview.h"
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
#include <QCheckBox>
#include <QSortFilterProxyModel>
#include "Network/httpdownload.h"

FileView::FileView(QWidget *parent) : QTableView(parent)
{
    FileModel *model = new FileModel(this);
    setModel(model);
    FileViewHeaderView *header = new FileViewHeaderView(Qt::Horizontal, this);
    setHorizontalHeader(header);
    //设置自定义委托
    setItemDelegate(new FileViewDelegate(this));
    //列宽自适应缩放填充
    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //单行选中
    setSelectionBehavior(QAbstractItemView::SelectRows);
    //选择模式-->单选
    setSelectionMode(QTableView::SingleSelection);
    //表头不高亮
    horizontalHeader()->setHighlightSections(false);
    //隐藏列头
    verticalHeader()->setVisible(false);
    //显示网格
    setShowGrid(true);
    //外框风格-->无边框
    setFrameShape(QFrame::NoFrame);
    //预设每列宽度
    horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
    setColumnWidth(0, 40);
    setColumnWidth(2, 60);


    setStyleSheet("QTableView{selection-background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #37ecba, stop: 1 #72afd3);}\
                   QCheckBox::indicator:unchecked { border-image: url(:/images/unchecked.png)0 0 0 28;}\
                   QCheckBox::indicator:unchecked:hover { border-image: url(:/images/unchecked.png)0 14 0 14;}\
                   QCheckBox::indicator:unchecked:pressed { border-image: url(:/images/unchecked.png)0 28 0 0;}\
                   QCheckBox::indicator:checked { border-image: url(:/images/checked.png)0 0 0 28;}\
                   QCheckBox::indicator:checked:hover { border-image: url(:/images/checked.png)0 14 0 14;}\
                   QCheckBox::indicator:checked:pressed { border-image: url(:/images/checked.png)0 28 0 0;}\
                   QPushButton{border:0px;color:black;text-decoration:underline;font:bold arial,sans-serif;}\
                   QPushButton:hover{color:blue}\
                   QPushButton:pressed{color:red}");

    connect(header, &FileViewHeaderView::signalStateChange, model, &FileModel::handleStateChange);
    connect(model, &FileModel::signalStateChange, header, &FileViewHeaderView::handleStateChange);
    connect(HttpDownload::getInstance(), &HttpDownload::signalImage, this, &FileView::handleReceiveImage);
}

FileView::~FileView()
{
}

void FileView::setDataSource(const QStringList &list)
{
    static_cast<FileModel *>(model())->setDataSource(list);

    for(int i=0; i<btnList.length(); i++) {
        delete btnList.at(i);
    }
    btnList.clear();

    for(int i=0; i<list.length(); i++) {
        QPushButton *btn = new QPushButton("查看", this);
        btn->setObjectName(QString::number(i));
        connect(btn, &QPushButton::clicked, this, [btn, this](){
            int row = btn->objectName().toInt();
            QString path = QString("/record/%1").arg(dataSource().at(row).fileName);
            qDebug() << "#FileView# view remote photo --> " << path;
            if(path.right(3).compare("jpg") == 0)
                QMetaObject::invokeMethod(HttpDownload::getInstance(), "getImage", Q_ARG(QString, path));
        });
        setIndexWidget(model()->index(i, OPERATIONCOLUMN), btn);
        btnList.append(btn);
    }
}

const QList<FileModel::FileInfo> &FileView::dataSource()
{
    return static_cast<FileModel *>(model())->getDataSource();
}

void FileView::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton) {
        QModelIndex index = indexAt(event->pos());
        if(index.isValid() && index.column() == CHECKBOXCOLUMN) {
            model()->setData(index, true, PRESSEDROLE);
        }
    }
    QTableView::mousePressEvent(event);
}

void FileView::mouseReleaseEvent(QMouseEvent *event)
{
    QModelIndex index = indexAt(event->pos());
    if(index.isValid() && index.column() == CHECKBOXCOLUMN) {
        model()->setData(index, false, PRESSEDROLE);
    }
    QTableView::mouseReleaseEvent(event);
}

void FileView::handleReceiveImage(QPixmap *pixmap)
{
    if(isVisible() && !pixmap->isNull()){
        QDialog *d = new QDialog;
        QLabel *lbl = new QLabel(d);
        lbl->setPixmap(*pixmap);
        lbl->setScaledContents(true);
        d->setMinimumSize(400, 300);
        d->resize(400, 300);
        QVBoxLayout *layout = new QVBoxLayout(d);
        layout->addWidget(lbl);
        d->exec();
        d->deleteLater();
        delete pixmap;
    }
}

FileModel::FileModel(QObject *parent) : QAbstractTableModel(parent),
    column(3)
{
    headList << "" << "文件名" << "操作";
}

FileModel::~FileModel()
{

}

int FileModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return fileList.length();
}

int FileModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return column;
}

QVariant FileModel::data(const QModelIndex &index, int role) const
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
    case Qt::DisplayRole: {
        if(1 == index.column())
            return fileList.at(index.row()).fileName;
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
//    //前景色
//    case Qt::ForegroundRole: {
//        return QColor(0, 0, 0);
//    }
//    //背景色
//    case Qt::BackgroundRole: {
//        return QBrush(QColor(255, 255, 255));
//    }
    case Qt::CheckStateRole:{
        if (index.column() == CHECKBOXCOLUMN) {
            return fileList.at(index.row()).CheckState;
        }
        break;
    }
    case PRESSEDROLE: {
        if (index.column() == CHECKBOXCOLUMN) {
            return fileList.at(index.row()).bPressed;
        }
        break;
    }
    default:
        break;
    }

    return QVariant();
}

bool FileModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    switch (role) {
    //保存每个item的checkbox状态
    case Qt::CheckStateRole: {
        fileList[index.row()].CheckState = value.toInt();
        checkState();
        emit dataChanged(index, index);
        return true;
    }
    //显示按压效果
    case PRESSEDROLE: {
        if (index.column() == CHECKBOXCOLUMN) {
            fileList[index.row()].bPressed = value.toBool();
            emit dataChanged(index, index);
        }
        break;
    }
    default:
        return QAbstractTableModel::setData(index, value, role);
    }
    return false;
}

QVariant FileModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch(role){
    //显示的数据
    case Qt::DisplayRole:{
        if(Qt::Horizontal == orientation){
            if(0 <= section && section < headList.size()){
                return headList.at(section);
            }
        }
        break;
    }
    //显示的字体样式
    case Qt::FontRole:{
        QFont font;
        font.setPixelSize(16);
        font.setBold(true);
        return font;
    }
    //文本对齐方式
    case Qt::TextAlignmentRole:
        return Qt::AlignCenter;
    default:
        return QAbstractTableModel::headerData(section, orientation, role);
        break;
    }

    return QVariant();
}

Qt::ItemFlags FileModel::flags(const QModelIndex &index) const
{
    if(!index.isValid()) {
        return Qt::NoItemFlags;
    }

    Qt::ItemFlags flags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;

    return flags;
}

//根据头列表复选框状态的改变来确定列表所有item的状态
void FileModel::handleStateChange(int state)
{
    if(fileList.length() == 0)
        return;

    for(int i=0; i<fileList.length(); i++) {
        fileList[i].CheckState = state;
    }
    emit dataChanged(index(0, CHECKBOXCOLUMN), index(fileList.length(), CHECKBOXCOLUMN));
}

//设置显示的数据源
void FileModel::setDataSource(const QStringList &l)
{
    emit signalStateChange(Qt::Unchecked);
    fileList.clear();
    FileInfo info;
    info.CheckState = Qt::Unchecked;
    info.bPressed = false;
    foreach (QString fileName, l) {
        info.fileName = fileName;
        fileList.append(info);
    }
    beginResetModel();
    endResetModel();
}

//根据列表中item的状态来确定目前头列表复选框的状态
void FileModel::checkState()
{
    if(fileList.length() == 0) {
        emit signalStateChange(Qt::Unchecked);
        return;
    }

    int count = 0;
    foreach (FileInfo info, fileList) {
        if(info.CheckState == Qt::Checked) {
            count++;
        }
    }

    if(count == 0) {
        emit signalStateChange(Qt::Unchecked);
    }else if(count < fileList.length()) {
        emit signalStateChange(Qt::PartiallyChecked);
    }else if(count == fileList.length()) {
        emit signalStateChange(Qt::Checked);
    }
}

FileViewDelegate::FileViewDelegate(QObject *parent) : QItemDelegate(parent)
{

}

void FileViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == CHECKBOXCOLUMN){
        painter->save();
        FileView *view = static_cast<FileView *>(const_cast<QWidget *>(option.widget));
        //绘制背景
        drawBackground(painter, option, index);
        int data = index.model()->data(index, Qt::CheckStateRole).toInt();
        //绘制CheckBox
        QStyleOptionButton checkBoxStyle;
        checkBoxStyle.state = data == Qt::Checked ? QStyle::State_On : QStyle::State_Off;
        //按压
        if(index.model()->data(index, PRESSEDROLE).toBool())
            checkBoxStyle.state |= QStyle::State_Sunken;

        QRect rect = QRect(option.rect.x() + (option.rect.width() - 14) / 2,
                           option.rect.y() + (option.rect.height() - 15) / 2,
                           14, 15);
        checkBoxStyle.rect = rect;

        QCheckBox checkBox(view);
        view->style()->drawPrimitive(QStyle::PE_IndicatorCheckBox, &checkBoxStyle, painter, &checkBox);
        painter->restore();
    }else {
        QItemDelegate::paint(painter, option, index);
    }
}

bool FileViewDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    QRect decorationRect = option.rect;

    QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
    if (event->type() == QEvent::MouseButtonRelease && decorationRect.contains(mouseEvent->pos()))
    {
        if (index.column() == CHECKBOXCOLUMN) {
            int data = model->data(index, Qt::CheckStateRole).toInt();
            model->setData(index, data == Qt::Unchecked ? Qt::Checked : Qt::Unchecked, Qt::CheckStateRole);
        }
    }

    return QItemDelegate::editorEvent(event, model, option, index);
}

void FileViewDelegate::drawBackground(QPainter *painter,
                                   const QStyleOptionViewItem &option,
                                   const QModelIndex &index) const
{
    if (option.showDecorationSelected && (option.state & QStyle::State_Selected)) {
        QPalette::ColorGroup cg = option.state & QStyle::State_Enabled
                                  ? QPalette::Normal : QPalette::Disabled;
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

FileViewHeaderView::FileViewHeaderView(Qt::Orientation orientation, QWidget *parent) : QHeaderView(orientation, parent),
    bPressed(false),
    bMoving(false),
    bTristate(false),
    bChecked(false)
{
    setHighlightSections(false);
    setMouseTracking(true);

//    setIndexWidget(model()->index(0, 0), new QCheckBox);

    // 响应鼠标
    setSectionsClickable(true);

    setStyleSheet("\
                    QCheckBox::indicator:unchecked { border-image: url(:/images/unchecked.png)0 0 0 28;}\
                    QCheckBox::indicator:unchecked:hover { border-image: url(:/images/unchecked.png)0 14 0 14;}\
                    QCheckBox::indicator:unchecked:pressed { border-image: url(:/images/unchecked.png)0 28 0 0;}\
                    QCheckBox::indicator:checked { border-image: url(:/images/checked.png)0 0 0 28;}\
                    QCheckBox::indicator:checked:hover { border-image: url(:/images/checked.png)0 14 0 14;}\
                    QCheckBox::indicator:checked:pressed { border-image: url(:/images/checked.png)0 28 0 0;}\
                    QCheckBox::indicator:indeterminate { border-image: url(:/images/tristate.png)0 0 0 28;}\
                    QCheckBox::indicator:indeterminate:hover { border-image: url(:/images/tristate.png)0 14 0 14;}\
                    QCheckBox::indicator:indeterminate:pressed { border-image: url(:/images/tristate.png)0 28 0 0;}");
}

void FileViewHeaderView::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
    painter->save();
    QHeaderView::paintSection(painter, rect, logicalIndex);
    painter->restore();

    if (logicalIndex == CHECKBOXCOLUMN)
    {
        painter->save();
        QStyleOptionButton option;
        option.initFrom(this);

        if (bPressed)
            option.state |= QStyle::State_Sunken;

        if (bTristate)
            option.state |= QStyle::State_NoChange;
        else
            option.state |= bChecked ? QStyle::State_On : QStyle::State_Off;

        if (testAttribute(Qt::WA_Hover) && underMouse()) {
            if (bMoving)
                option.state |= QStyle::State_MouseOver;
            else
                option.state &= ~QStyle::State_MouseOver;
        }
        QCheckBox checkBox(const_cast<FileViewHeaderView *>(this));
        option.iconSize = QSize(20, 20);
        option.rect = QRect(rect.x() + (rect.width() - 14) / 2,
                            rect.y() + (rect.height() - 15) / 2,
                            14, 15);
        style()->drawPrimitive(QStyle::PE_IndicatorCheckBox, &option, painter, &checkBox);
        painter->restore();
        //style()->drawItemPixmap(painter, rect, Qt::AlignCenter, QPixmap(":/images/checkBoxChecked"));
        //style()->drawControl(QStyle::CE_CheckBox, &option, painter, this);
    }
}

void FileViewHeaderView::mousePressEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && (logicalIndexAt(event->pos()) == CHECKBOXCOLUMN)) {
        bPressed = true;
        update();
    } else {
        QHeaderView::mousePressEvent(event);
    }
}

void FileViewHeaderView::mouseReleaseEvent(QMouseEvent *event)
{
    if (bPressed) {
        if (bTristate) {
            bChecked = true;
            bTristate = false;
        } else {
            bChecked = !bChecked;
        }
        update();
        Qt::CheckState state = bChecked ? Qt::Checked : Qt::Unchecked;
        emit signalStateChange(state);
    } else {
        QHeaderView::mouseReleaseEvent(event);
    }

    bPressed = false;
}

bool FileViewHeaderView::event(QEvent *event)
{
    if (event->type() == QEvent::Enter || event->type() == QEvent::Leave) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if (logicalIndexAt(mouseEvent->x()) == CHECKBOXCOLUMN) {
            bMoving = (event->type() == QEvent::Enter);
            update();
            return true;
        }
    }

    return QHeaderView::event(event);
}

void FileViewHeaderView::handleStateChange(int state)
{
    if (state == Qt::PartiallyChecked) {
        bTristate = true;
    } else {
        bTristate = false;
    }

    bChecked = (state != Qt::Unchecked);
    update();
}
