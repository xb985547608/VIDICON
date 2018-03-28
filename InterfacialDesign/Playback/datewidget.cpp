#include "datewidget.h"
#include <QPainter>
#include <QLabel>
#include <QGridLayout>
#include <QDebug>
#include <QMouseEvent>
#include <QApplication>
#include "Protocol/vidiconprotocol.h"
#include "parsexml.h"
#include <QPushButton>

DateWidget::DateWidget(QWidget *parent) :
    BasicWidget(parent)
{
    setMouseTracking(true);

    /*********************************界面布局*********************************/
    QGridLayout *layout1 = new QGridLayout;

    m_dateEdit = new QDateEdit(this);
    m_dateEdit->setDate(QDate::currentDate());
    m_dateEdit->setDisplayFormat("当前日期:yyyy-MM-dd");
    connect(m_dateEdit, &QDateEdit::dateChanged, this, &DateWidget::refresh);
    layout1->addWidget(m_dateEdit, 1, 0, 1, 7, Qt::AlignCenter);
    QStringList list;
    list << "日" << "一" << "二" << "三" << "四" << "五" << "六";
    for(int i=0; i<7; i++) {
        QLabel *lbl = new QLabel(list.at(i), this);
        lbl->setAlignment(Qt::AlignCenter);
        lbl->setFixedSize(25, 25);
        layout1->addWidget(lbl, 2, i, 1, 1, Qt::AlignCenter);
    }
    for(int i=0; i<6; i++) {
        for(int j=0; j<7; j++) {
            QLabel *lbl = new QLabel(this);
            lbl->setText(QString::number(i*7 + j + 1));
            lbl->setAlignment(Qt::AlignCenter);
            lbl->setObjectName(QString("lbl%1").arg(i*7 + j));
            lbl->setFixedSize(25, 25);
            lbl->setMouseTracking(true);
            m_lblDateMap.insert(i*7 + j, lbl);
            layout1->addWidget(lbl, i+3, j, 1, 1, Qt::AlignCenter);
        }
    }

    //提示信息
    QLabel *lbl1 = new QLabel(this);
    lbl1->setStyleSheet("background-color:#7BA8A7");
    lbl1->setFixedSize(15, 15);
    QLabel *lbl2 = new QLabel("表示当前选中", this);
    layout1->addWidget(lbl1, 9, 0, 1, 1);
    layout1->addWidget(lbl2, 9, 1, 1, 6, Qt::AlignLeft);
    QLabel *lbl3 = new QLabel(this);
    lbl3->setStyleSheet("background-color:green");
    lbl3->setFixedSize(15, 15);
    QLabel *lbl4 = new QLabel("表示存在视频或图片", this);
    layout1->addWidget(lbl3, 10, 0, 1, 1);
    layout1->addWidget(lbl4, 10, 1, 1, 6, Qt::AlignLeft);
    layout1->setSpacing(2);
    layout1->setSizeConstraint(QLayout::SetFixedSize);

    //查询当前日期的图片和视频文件
    QPushButton *btn = new QPushButton(this);
    btn->setFixedSize(80, 25);
    btn->setStyleSheet("QPushButton{border-image:url(:images/query.png)0 80 0 0}"
                       "QPushButton:pressed{border-image:url(:images/query.png)0 0 0 80}");
    connect(btn, &QPushButton::clicked, this, [this]() {
        BackUpQueryParameter *param1 = new BackUpQueryParameter;
        param1->Date = m_dateEdit->date();
        param1->Type = 1;
        emit signalSetParameter(VidiconProtocol::BACKQUERY, param1);
        BackUpQueryParameter *param2 = new BackUpQueryParameter;
        param2->Date = m_dateEdit->date();
        param2->Type = 2;
        emit signalSetParameter(VidiconProtocol::BACKQUERY, param2);
    });

    QVBoxLayout *layout2 = new QVBoxLayout(this);
    layout2->addLayout(layout1, 0);
    layout2->addStretch();
    layout2->addWidget(btn, 0, Qt::AlignCenter);
    layout2->setContentsMargins(0, 0, 0, 0);
    /*********************************界面布局*********************************/

    m_MonthMap.clear();
    handleDateChangle(QDate::currentDate());
}

void DateWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
}

void DateWidget::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);

}

void DateWidget::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);
}

void DateWidget::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);

    //获取鼠标点击所在的控件对象
    QWidget *w = qApp->widgetAt(QCursor::pos());
    if(w->objectName().left(3) == "lbl") {
        QLabel *lbl = static_cast<QLabel *>(w);
        if(lbl->text().length()) {
            m_dateEdit->setDate(QDate(m_dateEdit->date().year(), m_dateEdit->date().month(), lbl->text().toInt()));
            handleDateChangle(m_dateEdit->date());
        }
    }
}

void DateWidget::refresh(QDate date)
{
    if (date.isNull())
        date = m_dateEdit->date();

    BackUpQueryParameter *param1 = new BackUpQueryParameter;
    param1->Type = 6;
    param1->Date = date;
    emit signalSetParameter(VidiconProtocol::BACKQUERY, param1);
    BackUpQueryParameter *param2 = new BackUpQueryParameter;
    param2->Type = 0;
    param2->Date = date;
    emit signalSetParameter(VidiconProtocol::BACKQUERY, param2);
    emit signalDateChange(date);
}

void DateWidget::handleReceiveData(VidiconProtocol::Type type, QByteArray data)
{
    bool isOK = false;

    switch(type) {
    case VidiconProtocol::QUERYFILEMONTH: {
        BackUpQueryParameter param;
        m_MonthMap.clear();
        param.Type = 6;
        param.MonthMap = &m_MonthMap;

        isOK = ParseXML::getInstance()->parseBackUpQueryParameter(&param, data);
        if (isOK) {
            handleDateChangle(m_dateEdit->date());
        }
        break;
    }
    default:
        return;
    }

    if (isOK)
        qDebug() << "#DateWidget# handleReceiveData, ParameterType:" << type << "parse data success...";
    else
        qDebug() << "#DateWidget# handleReceiveData, ParameterType:" << type << "parse data error...";
}

void DateWidget::handleDateChangle(const QDate &date)
{
    int startIndex = QDate(date.year(), date.month(), 1).dayOfWeek();
    if(startIndex == 7) {
        startIndex = 0;
    }
    //不在日期排列中的lbl隐藏掉
    for(int i=0; i<startIndex; i++) {
        m_lblDateMap[i]->setText("");
        m_lblDateMap[i]->setStyleSheet("background-color:transparent; color: transparent");
    }
    //将lbl按顺序填写日期
    for(int i=0; i<date.daysInMonth(); i++) {
        m_lblDateMap[startIndex + i]->setText(QString::number(i + 1));
        m_lblDateMap[startIndex + i]->setStyleSheet("QLabel{background-color:#0B282A; color: white} QLabel:hover{background-color:blue}");
    }
    //不在日期排列中的lbl隐藏掉
    for(int i=startIndex + date.daysInMonth(); i<42; i++) {
        m_lblDateMap[i]->setText("");
        m_lblDateMap[i]->setStyleSheet("background-color:transparent; color: transparent");
    }

    int currentDay = m_dateEdit->date().day() - 1;
    m_lblDateMap[currentDay + startIndex]->setStyleSheet("QLabel{background-color:#7BA8A7; color: white;} QLabel:hover{background-color:blue}");

    for(int i=0; i<m_MonthMap.size(); i++) {
        if(m_MonthMap[i] == 1) {
            if(currentDay == i) {
                m_lblDateMap[i + startIndex]->setStyleSheet("QLabel{background-color:green; color: white; border:4px solid #7BA8A7} QLabel:hover{background-color:blue}");
            }else {
                m_lblDateMap[i + startIndex]->setStyleSheet("QLabel{background-color:green; color: white;} QLabel:hover{background-color:blue}");
            }
        }
    }
    update();
}
