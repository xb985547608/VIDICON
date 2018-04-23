#include "homewidget.h"
#include <QDebug>
#include <QContextMenuEvent>
#include <QMessageBox>
#include "ui/ui_homeform.h"
#include "lineedit.h"
#include "parsexml.h"
#include "Control/vlccontrol.h"
#include "Network/httpdownload.h"

HomeWidget::HomeWidget(QWidget *parent) :
    BasicWidget(parent),
    ui(new Ui::HomeForm)
{
    ui->setupUi(this);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setHighlightSections(false);
    ui->tableWidget->horizontalHeader()->setSectionsClickable(false);
    ui->tableWidget->installEventFilter(this);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Fixed);
    ui->tableWidget->setColumnWidth(0, 100);
    ui->tableWidget->setColumnWidth(2, 80);
    ui->tableWidget->setColumnWidth(3, 80);
    ui->tableWidget->setColumnWidth(4, 80);
    connect(ui->tableWidget, &QTableWidget::cellDoubleClicked, this, &HomeWidget::handleLogin);

    m_searchThread = new SearchDeviceThread(this);
    connect(m_searchThread, &SearchDeviceThread::signalDeviceInfo, this, &HomeWidget::hanldeDeviceInfo);
    connect(m_searchThread, &SearchDeviceThread::finished, this, [this](){
        QMessageBox::information(this, "提示", "搜索完毕");
    });

    m_selectIP = new SelectCurrentIpDialog(this);

    connect(ui->searchBtn, &QPushButton::clicked, this, &HomeWidget::onSearchBtnClicked);

    initLoginDialog();
    createActions();
    reset();

    m_cmDongle = new CMDongle(this);
}

HomeWidget::~HomeWidget()
{

}

void HomeWidget::reset()
{
    m_items.clear();
    for (int i=0; i<ui->tableWidget->rowCount(); i++)
        ui->tableWidget->removeRow(i);
    m_beforeLoginRow = -1;
    m_afterLoginRow = -1;
    isWaiting = false;

    ui->devNum->setText("0");
    ui->devIp->clear();
    ui->statusHint->setStyleSheet("border-image:url(:/images/h_offline.png)");
}

void HomeWidget::onSearchBtnClicked()
{
    if (!m_searchThread->isRunning()) {
        if (m_selectIP->exec() == QDialog::Accepted) {
            reset();
            m_searchThread->setSpecifiedIP(m_selectIP->getCurrentSelectIp());
            m_searchThread->start();
        }
    }
}

void HomeWidget::hanldeDeviceInfo(QVariant value)
{
    WholeDeviceInfo info = qvariant_cast<WholeDeviceInfo>(value);

    m_items.append(info);
    ui->devNum->setText(QString::number(m_items.count()));

    QTableWidgetItem *item1 = new QTableWidgetItem(QIcon(":/images/h_offline.png"), "未登录");
    QTableWidgetItem *item2 = new QTableWidgetItem(info.IPAddr);
    QTableWidgetItem *item3 = new QTableWidgetItem(QString::number(info.HTTPPort));
    QTableWidgetItem *item4 = new QTableWidgetItem(QString::number(info.ONVIFPort));
    QTableWidgetItem *item5 = new QTableWidgetItem(QString::number(info.RTSPPort));
    QTableWidgetItem *item6 = new QTableWidgetItem(info.MacAddr);
    QTableWidgetItem *item7 = new QTableWidgetItem(info.SerialNumber);
    QTableWidgetItem *item8 = new QTableWidgetItem(info.SoftwareVersion);

    item1->setTextAlignment(Qt::AlignCenter);
    item2->setTextAlignment(Qt::AlignCenter);
    item3->setTextAlignment(Qt::AlignCenter);
    item4->setTextAlignment(Qt::AlignCenter);
    item5->setTextAlignment(Qt::AlignCenter);
    item6->setTextAlignment(Qt::AlignCenter);
    item7->setTextAlignment(Qt::AlignCenter);
    item8->setTextAlignment(Qt::AlignCenter);

    int rowCount = ui->tableWidget->rowCount();

    ui->tableWidget->insertRow(rowCount);
    ui->tableWidget->setItem(rowCount, 0, item1);
    ui->tableWidget->setItem(rowCount, 1, item2);
    ui->tableWidget->setItem(rowCount, 2, item3);
    ui->tableWidget->setItem(rowCount, 3, item4);
    ui->tableWidget->setItem(rowCount, 4, item5);
    ui->tableWidget->setItem(rowCount, 5, item6);
    ui->tableWidget->setItem(rowCount, 6, item7);
    ui->tableWidget->setItem(rowCount, 7, item8);
}

bool HomeWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->tableWidget && event->type() == QEvent::ContextMenu) {
        QContextMenuEvent *menuEvent = dynamic_cast<QContextMenuEvent *>(event);
        if (menuEvent != NULL) {
            QPoint pos = menuEvent->pos();
            pos.setY(pos.y() - ui->tableWidget->horizontalHeader()->height());
            m_beforeLoginRow = ui->tableWidget->indexAt(pos).row();

            if (m_beforeLoginRow >= 0 && m_beforeLoginRow < ui->tableWidget->rowCount()) {
                m_logoutAction->setEnabled(m_beforeLoginRow == m_afterLoginRow);
                m_loginAction->setEnabled(m_beforeLoginRow != m_afterLoginRow);
                m_menu->exec(QCursor::pos());
            }
        }
    }
    return QWidget::eventFilter(obj, event);
}

void HomeWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
}

void HomeWidget::handleReceiveData(VidiconProtocol::Type type, QByteArray data)
{
    bool isOK = false;

    switch(type) {
        case VidiconProtocol::LOGIN: {
            ResponseStatus reply;
            isOK = ParseXML::getInstance()->parseResponseStatus(&reply, data);
            if (isOK) {
                if (reply.StatusString.compare("OK", Qt::CaseInsensitive) == 0) {
                    if (m_afterLoginRow != -1) {
                        ui->tableWidget->item(m_afterLoginRow, 0)->setIcon(QIcon(":/images/h_offline.png"));
                        ui->tableWidget->item(m_afterLoginRow, 0)->setText("未登录");
                    }

                    ui->tableWidget->item(m_beforeLoginRow, 0)->setIcon(QIcon(":/images/h_online.png"));
                    ui->tableWidget->item(m_beforeLoginRow, 0)->setText("已登录");
                    m_afterLoginRow = m_beforeLoginRow;

                    ui->devIp->setText(m_items.at(m_afterLoginRow).IPAddr);
                    ui->statusHint->setStyleSheet("border-image:url(:/images/h_online.png)");

                    QMessageBox::information(this, "提示", "登录成功");
                    qDebug() << "#HomeWidget# login device success";
                } else {
                    QMessageBox::information(this, "提示", "登录失败");
                    qDebug() << "#HomeWidget# login device erroe, please try again";
                }
            }
            if (m_afterLoginRow != -1) {
                QString host = m_items.at(m_afterLoginRow).IPAddr;
                QString httpPort = QString::number(m_items.at(m_afterLoginRow).HTTPPort);
                QString rtspPort = QString::number(m_items.at(m_afterLoginRow).RTSPPort);

                QString train = static_cast<LineEdit *>(m_loginMap["train"])->text();
                QString coach = static_cast<LineEdit *>(m_loginMap["coach"])->text();
                QString seat  = static_cast<LineEdit *>(m_loginMap["seat"])->text();
                QString user = QString("CRH_%1_%2_%3")
                        .arg(train)
                        .arg(coach)
                        .arg(seat);

                VidiconProtocol::getInstance()->setHost(host);
                VidiconProtocol::getInstance()->setPort(httpPort);

                VlcControl::getInstance()->setHost(host);
                VlcControl::getInstance()->setPort(rtspPort);
                VlcControl::getInstance()->setUser(user);
                VlcControl::getInstance()->setPasswd(user);

                HttpDownload::getInstance()->setHost(host);
                HttpDownload::getInstance()->setPort(httpPort);
            }

            static_cast<LineEdit *>(m_loginMap["train"])->clear();
            static_cast<LineEdit *>(m_loginMap["coach"])->clear();
            static_cast<LineEdit *>(m_loginMap["seat"])->clear();
            break;
        }
        default:
            return;
    }

    if (isOK)
        qDebug() << "#HomeWidget# handleReceiveData, ParameterType:" << type << "parse data success...";
    else
        qDebug() << "#HomeWidget# handleReceiveData, ParameterType:" << type << "parse data error...";
}

void HomeWidget::handleLogin(int row)
{    
    if (row >= 0 && row < ui->tableWidget->rowCount() &&
            (row != m_afterLoginRow)) {
        m_beforeLoginRow = row;

        if (m_afterLoginRow == m_beforeLoginRow)
            return;

        QString host = m_items.at(row).IPAddr;
        QString port = QString::number(m_items.at(row).HTTPPort);

        VidiconProtocol::getInstance()->setHost(host);
        VidiconProtocol::getInstance()->setPort(port);

        if (m_loginDialog->exec() == QDialog::Accepted) {
#if 0
            QString user = static_cast<LineEdit *>(m_loginMap.value("user"))->text();
            QString passwd = static_cast<LineEdit *>(m_loginMap.value("passwd"))->text();
#else
            QString train = static_cast<LineEdit *>(m_loginMap["train"])->text();
            QString coach = static_cast<LineEdit *>(m_loginMap["coach"])->text();
            QString seat  = static_cast<LineEdit *>(m_loginMap["seat"])->text();
            QString user = QString("CRH_%1_%2_%3")
                    .arg(train)
                    .arg(coach)
                    .arg(seat);
#endif
            if (user.length() != 16) {
                QMessageBox::warning(this, "警告", "用户名格式错误，请重新输入");
            } else {
                QMetaObject::invokeMethod(VidiconProtocol::getInstance(), "login",
                                          Q_ARG(QString, user),
                                          Q_ARG(QString, user));
            }
        }
    }
}

void HomeWidget::initLoginDialog()
{
    m_loginDialog = new QDialog(this);
    m_loginDialog->setFixedSize(200, 100);

#if 0
    QLabel *lbl1 = new QLabel("用户：", m_loginDialog);
    LineEdit *lineEdit1 = new LineEdit(m_loginDialog);
    m_loginMap.insert("user", lineEdit1);

    QLabel *lbl2 = new QLabel("密码：", m_loginDialog);
    LineEdit *lineEdit2 = new LineEdit(m_loginDialog);
    m_loginMap.insert("passwd", lineEdit2);

    QPushButton *btn = new QPushButton("登录", m_loginDialog);
    btn->setFixedWidth(50);
    connect(btn, &QPushButton::clicked, m_loginDialog, &QDialog::accept);

    QGridLayout *layout1 = new QGridLayout;
    layout1->addWidget(lbl1,      0, 0, 1, 1);
    layout1->addWidget(lineEdit1, 0, 1, 1, 2);

    layout1->addWidget(lbl2,      1, 0, 1, 1);
    layout1->addWidget(lineEdit2, 1, 1, 1, 2);

    layout1->addWidget(btn,       2, 0, 1, 3, Qt::AlignRight);

    QVBoxLayout *layout2 = new QVBoxLayout;
    layout2->addStretch();
    layout2->addLayout(layout1);
    layout2->addStretch();

    QHBoxLayout *layout3 = new QHBoxLayout(m_loginDialog);
    layout3->addStretch(1);
    layout3->addLayout(layout2);
    layout3->addStretch(1);
#else
    QLabel *lbl1 = new QLabel("车号：", m_loginDialog);
    LineEdit *lineEdit1 = new LineEdit(m_loginDialog);
    lineEdit1->setReadOnly(false);
    lineEdit1->setMaxLength(6);
    m_loginMap.insert("train", lineEdit1);

    QLabel *lbl2 = new QLabel("车厢号：", m_loginDialog);
    LineEdit *lineEdit2 = new LineEdit(m_loginDialog);
    lineEdit2->setReadOnly(false);
    lineEdit2->setMaxLength(2);
    m_loginMap.insert("coach", lineEdit2);

    QLabel *lbl3 = new QLabel("车位号：", m_loginDialog);
    LineEdit *lineEdit3 = new LineEdit(m_loginDialog);
    lineEdit3->setReadOnly(false);
    lineEdit2->setMaxLength(2);
    m_loginMap.insert("seat", lineEdit3);

    QPushButton *btn = new QPushButton("登录", m_loginDialog);
    btn->setFixedWidth(50);
    connect(btn, &QPushButton::clicked, m_loginDialog, &QDialog::accept);

    QGridLayout *layout1 = new QGridLayout;
    layout1->addWidget(lbl1,        0, 0, 1, 1);
    layout1->addWidget(lineEdit1,   0, 1, 1, 1);

    layout1->addWidget(lbl2,        1, 0, 1, 1);
    layout1->addWidget(lineEdit2,   1, 1, 1, 1);

    layout1->addWidget(lbl3,        2, 0, 1, 1);
    layout1->addWidget(lineEdit3,   2, 1, 1, 1);

    layout1->addWidget(btn,         3, 0, 1, 2, Qt::AlignCenter);

    setAlignment(m_loginDialog, layout1, Qt::AlignTop | Qt::AlignHCenter);
#endif
}

void HomeWidget::createActions()
{
    m_menu = new QMenu(this);

    m_loginAction = new QAction(m_menu);
    m_loginAction->setText("登录");
    m_loginAction->setIcon(QIcon(":/images/h_login.png"));
    connect(m_loginAction, &QAction::triggered, this, [this]() {
        handleLogin(m_beforeLoginRow);
    });

    m_logoutAction = new QAction(m_menu);
    m_logoutAction->setText("登出");
    m_logoutAction->setIcon(QIcon(":/images/h_logout.png"));
    connect(m_logoutAction, &QAction::triggered, this, [this](){
        ui->tableWidget->item(m_afterLoginRow, 0)->setIcon(QIcon(":/images/h_offline.png"));
        ui->tableWidget->item(m_afterLoginRow, 0)->setText("未登录");
        ui->devIp->clear();
        ui->statusHint->setStyleSheet("border-image:url(:/images/h_offline.png)");
        m_afterLoginRow = -1;
    });

    m_menu->addAction(m_loginAction);
    m_menu->addAction(m_logoutAction);
}
