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
    setModel(new UserInfoModel(this));
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
    setFrameShape(QFrame::Box);
    //设置鼠标跟踪
    setMouseTracking(true);
    //行交替颜色
    setAlternatingRowColors(true);
    setStyleSheet("QTableView::item:selected{  \
                    background-color: #3399FF;    /*选中行颜色*/  \
                   }");

    //合并第3、4列
    setSpan(0, 3, 1, 2);

    setColumnWidth(0, 120);
    setColumnWidth(1, 120);
    setColumnWidth(2, 120);
    setColumnWidth(3, 120);
    setColumnWidth(4, 120);
    setFixedWidth(610);

    initModifyInfoWidget();
    initAddUserInfoWidget();

    connect(this, &UserInfoView::signalSetParameter, VidiconProtocol::getInstance(), &VidiconProtocol::handleSetParameter);
}

UserInfoView::~UserInfoView()
{
    qDebug("delete Tableview");
}

void UserInfoView::setDataSource(const QList<VidiconProtocol::UserConfigInfo> &l)
{
    static_cast<UserInfoModel *>(model())->setDataSource(l);

    for (int i=0; i<l.size(); i++) {
        VidiconProtocol::UserConfigInfo info = l.at(i);

        QPushButton *btn1 = new QPushButton("Modify", this);
        btn1->setObjectName(QString::number(i + 1));
        connect(btn1, &QPushButton::clicked, this, &UserInfoView::handleModifyInfo);
        setIndexWidget(model()->index(i + 1, 3), btn1);

        QPushButton *btn2 = new QPushButton("DelUser", this);
        btn2->setObjectName(QString::number(i + 1));
        connect(btn2, &QPushButton::clicked, this, &UserInfoView::handleDelUserInfo);
        setIndexWidget(model()->index(i + 1, 4), btn2);
    }
}

void UserInfoView::initModifyInfoWidget()
{
    QStringList list;

    modifyInfoWidget = new QDialog(this);
    modifyInfoWidget->setFixedSize(400, 150);

    QLabel *lbl1 = new QLabel("用户名：", modifyInfoWidget);
    QLineEdit *lineEdit1 = new QLineEdit(modifyInfoWidget);
    lineEdit1->setReadOnly(true);
    modifyInfoMap.insert("User", lineEdit1);

    QLabel *lbl2 = new QLabel("权限：", modifyInfoWidget);
    QComboBox *comboBox1 = new QComboBox(modifyInfoWidget);
    list << "管理权限" << "维护权限" << "一般权限";
    comboBox1->addItems(list);
    modifyInfoMap.insert("Group", comboBox1);

    QLabel *lbl3 = new QLabel("输入密码：", modifyInfoWidget);
    QLineEdit *lineEdit2 = new QLineEdit(modifyInfoWidget);
    lineEdit2->setMaxLength(15);
    modifyInfoMap.insert("Password", lineEdit2);

    QLabel *lbl4 = new QLabel("确认密码：", modifyInfoWidget);
    QLineEdit *lineEdit3 = new QLineEdit(modifyInfoWidget);
    lineEdit3->setMaxLength(15);
    modifyInfoMap.insert("Confirm", lineEdit3);

    QPushButton *btn = new QPushButton("Save", modifyInfoWidget);
    btn->setFixedWidth(50);
    connect(btn, &QPushButton::clicked, this, [this](){
        QString passwd1 = static_cast<QLineEdit *>(modifyInfoMap["Password"])->text();
        QString passwd2 = static_cast<QLineEdit *>(modifyInfoMap["Confirm"])->text();

        if (passwd1 == passwd2) {
            VidiconProtocol::UserConfigInfo *info = new VidiconProtocol::UserConfigInfo;
            info->UserName = static_cast<QLineEdit *>(modifyInfoMap["User"])->text();
            info->PassWord = passwd1;
            info->Privilege = static_cast<QComboBox *>(modifyInfoMap["Group"])->currentIndex();
            emit signalSetParameter(USERCONFIG, info);
        }
    });

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
}

void UserInfoView::initAddUserInfoWidget()
{
    QStringList list;

    addUserInfoWidget = new QDialog(this);
    addUserInfoWidget->setFixedSize(400, 150);

    QLabel *lbl1 = new QLabel("用户名：", addUserInfoWidget);
    QLineEdit *lineEdit1 = new QLineEdit(addUserInfoWidget);
    lineEdit1->setReadOnly(false);
    addUserInfoMap.insert("User", lineEdit1);

    QLabel *lbl2 = new QLabel("权限：", addUserInfoWidget);
    QComboBox *comboBox1 = new QComboBox(addUserInfoWidget);
    list << "管理权限" << "维护权限" << "一般权限";
    comboBox1->addItems(list);
    addUserInfoMap.insert("Group", comboBox1);

    QLabel *lbl3 = new QLabel("输入密码：", addUserInfoWidget);
    QLineEdit *lineEdit2 = new QLineEdit(addUserInfoWidget);
    lineEdit2->setMaxLength(15);
    addUserInfoMap.insert("Password", lineEdit2);

    QLabel *lbl4 = new QLabel("确认密码：", addUserInfoWidget);
    QLineEdit *lineEdit3 = new QLineEdit(addUserInfoWidget);
    lineEdit3->setMaxLength(15);
    addUserInfoMap.insert("Confirm", lineEdit3);

    QPushButton *btn = new QPushButton("Save", addUserInfoWidget);
    btn->setFixedWidth(50);
    connect(btn, &QPushButton::clicked, this, [this](){
        QString passwd1 = static_cast<QLineEdit *>(addUserInfoMap["Password"])->text();
        QString passwd2 = static_cast<QLineEdit *>(addUserInfoMap["Confirm"])->text();

        if (passwd1 == passwd2) {
            VidiconProtocol::UserConfigInfo *info = new VidiconProtocol::UserConfigInfo;
            info->UserName = static_cast<QLineEdit *>(addUserInfoMap["User"])->text();
            info->PassWord = passwd1;
            info->Privilege = static_cast<QComboBox *>(addUserInfoMap["Group"])->currentIndex();
            emit signalSetParameter(ADDUSER, info);
        }
    });

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

    QHBoxLayout *layout3 = new QHBoxLayout(addUserInfoWidget);
    layout3->addStretch(1);
    layout3->addLayout(layout2, 4);
    layout3->addStretch(1);
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
    int row = sender()->objectName().toInt();

    static_cast<QLineEdit *>(modifyInfoMap["User"])->setText(model()->data(model()->index(row, 1)).toString());
    static_cast<QComboBox *>(modifyInfoMap["Group"])->setCurrentIndex(model()->data(model()->index(row, 2)).toInt());

    static_cast<QLineEdit *>(modifyInfoMap["Password"])->clear();
    static_cast<QLineEdit *>(modifyInfoMap["Confirm"])->clear();

    modifyInfoWidget->exec();
}

void UserInfoView::handleDelUserInfo()
{
    int row = sender()->objectName().toInt();

    QString user = model()->data(model()->index(row, 1)).toString();
    if (QMessageBox::question(this, "删除用户！！", QString("是否删除用户名为：%1的账号信息").arg(user.toStdString().data()))
            == QMessageBox::Yes) {
        VidiconProtocol::UserConfigInfo *info = new VidiconProtocol::UserConfigInfo;
        info->UserName = user;
        emit signalSetParameter(DELETEUSER, info);
    }
}

void UserInfoView::handleAddUserInfo()
{
    static_cast<QLineEdit *>(addUserInfoMap["User"])->clear();
    static_cast<QComboBox *>(addUserInfoMap["Group"])->setCurrentIndex(0);
    static_cast<QLineEdit *>(addUserInfoMap["Password"])->clear();
    static_cast<QLineEdit *>(addUserInfoMap["Confirm"])->clear();

    addUserInfoWidget->exec();
}

UserInfoModel::UserInfoModel(QObject *parent) : QAbstractTableModel(parent),
    column(5)
{
    headList << "No." << "用户名" << "权限" << "操作";
}

UserInfoModel::~UserInfoModel()
{

}

int UserInfoModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return list.size() + 1;
}

int UserInfoModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return column;
}

QVariant UserInfoModel::data(const QModelIndex &index, int role) const
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
                    return index.row();
                else if(1 == index.column())
                    return list.at(index.row() - 1).UserName;
                else if(2 == index.column()) {
                    QStringList list;
                    list << "管理权限" << "维护权限" << "一般权限";
                    return list.at(this->list.at(index.row() - 1).Privilege);
                }
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

Qt::ItemFlags UserInfoModel::flags(const QModelIndex &index) const
{
    if(!index.isValid()){
        return Qt::NoItemFlags;
    }

    Qt::ItemFlags flags = QAbstractTableModel::flags(index);

    flags &= !Qt::ItemIsEditable;

    return flags;
}

void UserInfoModel::setDataSource(const QList<VidiconProtocol::UserConfigInfo> &l)
{
    list = l;
    beginResetModel();
    endResetModel();
}
