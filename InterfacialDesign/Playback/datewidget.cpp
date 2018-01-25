#include "datewidget.h"
#include <QPainter>
#include <QLabel>
#include <QGridLayout>
#include <QDebug>
#include <QMouseEvent>
#include <QApplication>
#include "Protocol/vidiconprotocol.h"
#include "parsexml.h"


DateWidget::DateWidget(QWidget *parent) : QWidget(parent)
{
    setMouseTracking(true);

    connect(this, &DateWidget::signalSetParameter, VidiconProtocol::getInstance(), &VidiconProtocol::handlerSetParameter);
    connect(VidiconProtocol::getInstance(), &VidiconProtocol::signalSendData, this, &DateWidget::handlerReceiveData);

    /*********************************界面布局*********************************/
    QGridLayout *layout1 = new QGridLayout(this);

//    QLabel *lbl = new QLabel("文件类型", this);
//    typeSelect = new QComboBox(this);
//    typeSelect->addItem("图片");
//    typeSelect->addItem("视频");
//    layout1->addWidget(lbl, 0, 0, 1, 3);
//    layout1->addWidget(typeSelect, 0, 3, 1, 4);

    dateEdit = new QDateEdit(this);
    dateEdit->setDisplayFormat("当前日期:yyyy-MM");
    connect(dateEdit, &QDateEdit::dateChanged, this, [this](QDate date){
        VidiconProtocol::BackUpQueryParameter *param = new VidiconProtocol::BackUpQueryParameter;
        param->Type = 6;
        param->Date = date;
        emit signalSetParameter(BACKQUERY, param);
    });
    layout1->addWidget(dateEdit, 1, 0, 1, 7, Qt::AlignCenter);
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
            lblDateMap.insert(i*7 + j, lbl);
            layout1->addWidget(lbl, i+3, j, 1, 1, Qt::AlignCenter);
        }
    }

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
    /*********************************界面布局*********************************/

    dateEdit->setDate(QDate::currentDate());
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
    QWidget *w = qApp->widgetAt(QCursor::pos());
    if(w->objectName().left(3) == "lbl") {
        QLabel *lbl = static_cast<QLabel *>(w);
        if(lbl->text().length()) {
            dateEdit->setDate(QDate(dateEdit->date().year(), dateEdit->date().month(), lbl->text().toInt()));
            handlerDateChangle(dateEdit->date());
        }
    }
}

void DateWidget::handlerReceiveData(int type, QByteArray data)
{
    switch(type) {
    case QUERYFILEMONTH: {
        VidiconProtocol::BackUpQueryParameter param;
        MonthMap.clear();
        param.Type = 6;
        param.MonthMap = &MonthMap;
        if(ParseXML::getInstance()->parseBackUpQueryParameter(&param, data)) {
            handlerDateChangle(dateEdit->date());
            qDebug() << "#DateWidget# handlerReceiveData, ParameterType:" << type << "parse data success...";
        }else {
            qDebug() << "#DateWidget# handlerReceiveData, ParameterType:" << type << "parse data error...";
        }
        break;
    }
    default:
        break;
    }
}

void DateWidget::handlerDateChangle(QDate date)
{
    int startIndex = QDate(date.year(), date.month(), 1).dayOfWeek();
    if(startIndex == 7) {
        startIndex = 0;
    }
    for(int i=0; i<startIndex; i++) {
        lblDateMap[i]->setText("");
        lblDateMap[i]->setStyleSheet("background-color:transparent; color: transparent");
    }
    for(int i=0; i<date.daysInMonth(); i++) {
        lblDateMap[startIndex + i]->setText(QString::number(i + 1));
        lblDateMap[startIndex + i]->setStyleSheet("QLabel{background-color:#0B282A; color: white} QLabel:hover{background-color:blue}");
    }
    for(int i=startIndex + date.daysInMonth(); i<42; i++) {
        lblDateMap[i]->setText("");
        lblDateMap[i]->setStyleSheet("background-color:transparent; color: transparent");
    }
    int currentDay = dateEdit->date().day() - 1;
    for(int i=0; i<MonthMap.size(); i++) {
        if(MonthMap[i] == 1) {
            if(currentDay == i) {
                lblDateMap[i + startIndex]->setStyleSheet("QLabel{background-color:green; color: white; border:4px solid #7BA8A7} QLabel:hover{background-color:blue}");
            }else {
                lblDateMap[i + startIndex]->setStyleSheet("QLabel{background-color:green; color: white;} QLabel:hover{background-color:blue}");
            }
        }else {
            if(currentDay == i) {
                lblDateMap[i + startIndex]->setStyleSheet("QLabel{background-color:#7BA8A7; color: white;} QLabel:hover{background-color:blue}");
            }
        }
    }
}
