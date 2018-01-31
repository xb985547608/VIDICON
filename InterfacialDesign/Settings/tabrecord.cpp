#include "tabrecord.h"
#include <QGridLayout>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QSlider>
#include <QDebug>
#include <QModelIndex>
#include <QSpinBox>
#include <QDateEdit>
#include <QTimeEdit>
#include <QTimeEdit>
#include <QCheckBox>
#include "DrawSubWidget/timeregionwidget.h"
#include "parsexml.h"
#include <QMessageBox>

TabRecord::TabRecord(QWidget *parent) : QTabWidget(parent)
{
    initScheduleWidget();
    initSDStorageWidget();
    initSnapshotWidget();
//    initDestinationWidget();
//    initNASWidget();

    connect(VidiconProtocol::getInstance(), &VidiconProtocol::signalSendData, this, &TabRecord::handlerReceiveData);
    connect(this, &TabRecord::signalSetParameter, VidiconProtocol::getInstance(), &VidiconProtocol::handlerSetParameter);
    connect(this, &TabRecord::signalGetParameter, VidiconProtocol::getInstance(), &VidiconProtocol::handlerGetParameter);
    connect(this, &TabRecord::currentChanged, this, [this](){
        handlerSwitchTab(QModelIndex());
    });

    emit signalGetParameter(SCHEDULEPARAMETER);
    emit signalGetParameter(SNAPSHOTPARAMETER);
}

TabRecord::~TabRecord()
{
    qDebug("delete TabRecord");
}

void TabRecord::initScheduleWidget()
{
    QStringList list;
    scheduleWidget = new QWidget(this);

    QCheckBox *cb1 = new QCheckBox("启用录像计划", scheduleWidget);
    scheduleMap.insert("Enable", cb1);

    TimeRegionWidget *region = new TimeRegionWidget(scheduleWidget);
    scheduleMap.insert("region", region);

    QCheckBox *cb2 = new QCheckBox("时间段1", scheduleWidget);
    cb2->setObjectName("Time Period 0");
    connect(cb2, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerTimeSelectState);
    connect(cb2, &QCheckBox::stateChanged, this, &TabRecord::handlerTimeSelect);
    scheduleMap.insert("Time Period 0", cb2);
    QTimeEdit *time1 = new QTimeEdit(scheduleWidget);
    time1->setEnabled(false);
    time1->setObjectName("Time Period 0 start");
    connect(time1, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handlerTimeChange);
    scheduleMap.insert("Time Period 0 start", time1);
    QLabel *lbl1 = new QLabel("-", scheduleWidget);
    QTimeEdit *time2 = new QTimeEdit(scheduleWidget);
    time2->setTime(QTime(23, 59, 59));
    time2->setEnabled(false);
    time2->setObjectName("Time Period 0 end");
    connect(time2, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handlerTimeChange);
    scheduleMap.insert("Time Period 0 end", time2);

    QCheckBox *cb3 = new QCheckBox("时间段2", scheduleWidget);
    cb3->setObjectName("Time Period 1");
    connect(cb3, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerTimeSelectState);
    connect(cb3, &QCheckBox::stateChanged, this, &TabRecord::handlerTimeSelect);
    scheduleMap.insert("Time Period 1", cb3);
    QTimeEdit *time3 = new QTimeEdit(scheduleWidget);
    time3->setEnabled(false);
    time3->setObjectName("Time Period 1 start");
    connect(time3, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handlerTimeChange);
    scheduleMap.insert("Time Period 1 start", time3);
    QLabel *lbl2 = new QLabel("-", scheduleWidget);
    QTimeEdit *time4 = new QTimeEdit(scheduleWidget);
    time4->setTime(QTime(23, 59, 59));
    time4->setEnabled(false);
    time4->setObjectName("Time Period 1 end");
    connect(time4, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handlerTimeChange);
    scheduleMap.insert("Time Period 1 end", time4);

    QCheckBox *cb4 = new QCheckBox("时间段3", scheduleWidget);
    cb4->setObjectName("Time Period 2");
    connect(cb4, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerTimeSelectState);
    connect(cb4, &QCheckBox::stateChanged, this, &TabRecord::handlerTimeSelect);
    scheduleMap.insert("Time Period 2", cb4);
    QTimeEdit *time5 = new QTimeEdit(scheduleWidget);
    time5->setEnabled(false);
    time5->setObjectName("Time Period 2 start");
    connect(time5, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handlerTimeChange);
    scheduleMap.insert("Time Period 2 start", time5);
    QLabel *lbl3 = new QLabel("-", scheduleWidget);
    QTimeEdit *time6 = new QTimeEdit(scheduleWidget);
    time6->setTime(QTime(23, 59, 59));
    time6->setEnabled(false);
    time6->setObjectName("Time Period 2 end");
    connect(time6, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handlerTimeChange);
    scheduleMap.insert("Time Period 2 end", time6);

    QCheckBox *cb5 = new QCheckBox("时间段4", scheduleWidget);
    cb5->setObjectName("Time Period 3");
    connect(cb5, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerTimeSelectState);
    connect(cb5, &QCheckBox::stateChanged, this, &TabRecord::handlerTimeSelect);
    scheduleMap.insert("Time Period 3", cb5);
    QTimeEdit *time7 = new QTimeEdit(scheduleWidget);
    time7->setEnabled(false);
    time7->setObjectName("Time Period 3 start");
    connect(time7, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handlerTimeChange);
    scheduleMap.insert("Time Period 3 start", time7);
    QLabel *lbl4 = new QLabel("-", scheduleWidget);
    QTimeEdit *time8 = new QTimeEdit(scheduleWidget);
    time8->setTime(QTime(23, 59, 59));
    time8->setEnabled(false);
    time8->setObjectName("Time Period 3 end");
    connect(time8, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handlerTimeChange);
    scheduleMap.insert("Time Period 3 end", time8);

    QPushButton *btn1 = new QPushButton("全选", scheduleWidget);
    connect(btn1, &QPushButton::clicked, this, &TabRecord::handlerSelectAllWeek);

    QCheckBox *cb6 = new QCheckBox("周天", scheduleWidget);
    cb6->setObjectName("week 0");
    connect(cb6, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerWeekSelectState);
    scheduleMap.insert("week 0", cb6);

    QCheckBox *cb7 = new QCheckBox("周一", scheduleWidget);
    cb7->setObjectName("week 1");
    connect(cb7, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerWeekSelectState);
    scheduleMap.insert("week 1", cb7);

    QCheckBox *cb8 = new QCheckBox("周二", scheduleWidget);
    cb8->setObjectName("week 2");
    connect(cb8, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerWeekSelectState);
    scheduleMap.insert("week 2", cb8);

    QCheckBox *cb9 = new QCheckBox("周三", scheduleWidget);
    cb9->setObjectName("week 3");
    connect(cb9, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerWeekSelectState);
    scheduleMap.insert("week 3", cb9);

    QCheckBox *cb10 = new QCheckBox("周四", scheduleWidget);
    cb10->setObjectName("week 4");
    connect(cb10, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerWeekSelectState);
    scheduleMap.insert("week 4", cb10);

    QCheckBox *cb11 = new QCheckBox("周五", scheduleWidget);
    cb11->setObjectName("week 5");
    connect(cb11, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerWeekSelectState);
    scheduleMap.insert("week 5", cb11);

    QCheckBox *cb12 = new QCheckBox("周六", scheduleWidget);
    cb12->setObjectName("week 6");
    connect(cb12, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerWeekSelectState);
    scheduleMap.insert("week 6", cb12);

    QPushButton *btn2 = new QPushButton("保存", scheduleWidget);
    connect(btn2, &QPushButton::clicked, this, &TabRecord::handlerPrepareData);

    QGridLayout *layout1 = new QGridLayout;
    layout1->addWidget(cb1,   0, 0, 1, 2);

    layout1->addWidget(region,  1, 0, 7, 6);

    QHBoxLayout *layout4 = new QHBoxLayout;
    layout4->addWidget(cb2);
    layout4->addWidget(time1);
    layout4->addWidget(lbl1 );
    layout4->addWidget(time2);
    layout1->addLayout(layout4, 8, 0, 1, 3);

    QHBoxLayout *layout5 = new QHBoxLayout;
    layout5->addWidget(cb3);
    layout5->addWidget(time3);
    layout5->addWidget(lbl2 );
    layout5->addWidget(time4);
    layout1->addLayout(layout5, 9, 0, 1, 3);

    QHBoxLayout *layout6 = new QHBoxLayout;
    layout6->addWidget(cb4);
    layout6->addWidget(time5);
    layout6->addWidget(lbl3 );
    layout6->addWidget(time6);
    layout1->addLayout(layout6, 10, 0, 1, 3);

    QHBoxLayout *layout7 = new QHBoxLayout;
    layout7->addWidget(cb5);
    layout7->addWidget(time7);
    layout7->addWidget(lbl4 );
    layout7->addWidget(time8);
    layout1->addLayout(layout7, 11, 0, 1, 3);

    layout1->addWidget(btn1,    12, 0, 1, 1);
    layout1->addWidget(cb6,     12, 1, 1, 1);
    layout1->addWidget(cb7,     12, 2, 1, 1);
    layout1->addWidget(cb8,     12, 3, 1, 1);
    layout1->addWidget(cb9,     12, 4, 1, 1);
    layout1->addWidget(cb10,     12, 5, 1, 1);
    layout1->addWidget(cb11,     12, 6, 1, 1);
    layout1->addWidget(cb12,     12, 7, 1, 1);

    layout1->addWidget(btn2,    13, 0, 1, 8, Qt::AlignCenter);

    QVBoxLayout *layout2 = new QVBoxLayout;
    layout2->addLayout(layout1);
    layout2->addStretch();

    QHBoxLayout *layout3 = new QHBoxLayout(scheduleWidget);
    layout3->addStretch(1);
    layout3->addLayout(layout2, 4);
    layout3->addStretch(1);

    addTab(scheduleWidget, "远程录像计划");
}

void TabRecord::initSDStorageWidget()
{
    QStringList list;
    SDStorageWidget = new QWidget(this);

    QRadioButton *rBtn1 = new QRadioButton("掉线自动录像", SDStorageWidget);
    SDStorageMap.insert("Offline Record", rBtn1);
    QRadioButton *rBtn2 = new QRadioButton("自动续传", SDStorageWidget);
    SDStorageMap.insert("Auto Upload", rBtn2);

    QLabel *lbl1 = new QLabel("SD总大小：", SDStorageWidget);
    QLineEdit *lineEdit1 = new QLineEdit(SDStorageWidget);
    SDStorageMap.insert("Total Space", lineEdit1);

    QLabel *lbl2 = new QLabel("SD已使用大小：", SDStorageWidget);
    QLineEdit *lineEdit2 = new QLineEdit(SDStorageWidget);
    SDStorageMap.insert("Used Space", lineEdit2);

    QLabel *lbl3 = new QLabel("SD未使用大小：", SDStorageWidget);
    QLineEdit *lineEdit3 = new QLineEdit(SDStorageWidget);
    SDStorageMap.insert("Available Space", lineEdit3);

    QPushButton *btn1 = new QPushButton("Refresh", SDStorageWidget);
    connect(btn1, &QPushButton::clicked, this, [this](){ emit signalGetParameter(SDCARDPARAMETER); });
    QPushButton *btn2 = new QPushButton("Format", SDStorageWidget);
    connect(btn2, &QPushButton::clicked, this, [this](){
        if(QMessageBox::question(this, "警告", "是否格式化SD卡") == QMessageBox::Yes) {
            emit signalSetParameter(FORMATSDCARD, NULL);
        }
    });

    QLabel *lbl4 = new QLabel("SD内存已满时：", SDStorageWidget);
    QComboBox *comboBox1 = new QComboBox(SDStorageWidget);
    list << "循环录像" << "停止录像";
    comboBox1->addItems(list);
    list.clear();
    SDStorageMap.insert("Overwrite", comboBox1);

    QLabel *lbl5 = new QLabel("录像码流：", SDStorageWidget);
    QComboBox *comboBox2 = new QComboBox(SDStorageWidget);
    list << "主码流" << "子码流";
    comboBox2->addItems(list);
    list.clear();
    SDStorageMap.insert("Stream", comboBox2);

    QLabel *lbl6 = new QLabel("录像类型：", SDStorageWidget);
    QComboBox *comboBox3 = new QComboBox(SDStorageWidget);
    list << "ivd" << "mp4";
    comboBox3->addItems(list);
    list.clear();
    SDStorageMap.insert("Record Type", comboBox1);

    QLabel *lbl7 = new QLabel("报警持续录像时间：", SDStorageWidget);
    QLineEdit *lineEdit4 = new QLineEdit(SDStorageWidget);
    SDStorageMap.insert("Record Time", lineEdit4);

    QPushButton *btn3 = new QPushButton("保存", SDStorageWidget);
    connect(btn3, &QPushButton::clicked, this, &TabRecord::handlerPrepareData);

    QGridLayout *layout1 = new QGridLayout;
    layout1->addWidget(rBtn1,     0, 0, 1, 2);
    layout1->addWidget(rBtn2,     0, 2, 1, 2);

    layout1->addWidget(lbl1,      1, 0, 1, 2);
    layout1->addWidget(lineEdit1, 1, 2, 1, 2);

    layout1->addWidget(lbl2,      2, 0, 1, 2);
    layout1->addWidget(lineEdit2, 2, 2, 1, 2);

    layout1->addWidget(lbl3,      3, 0, 1, 2);
    layout1->addWidget(lineEdit3, 3, 2, 1, 2);

    layout1->addWidget(btn1,      4, 2, 1, 1);
    layout1->addWidget(btn2,      4, 3, 1, 1);

    layout1->addWidget(lbl4,      5, 0, 1, 2);
    layout1->addWidget(comboBox1, 5, 2, 1, 2);

    layout1->addWidget(lbl5,      6, 0, 1, 2);
    layout1->addWidget(comboBox2, 6, 2, 1, 2);

    layout1->addWidget(lbl6,      7, 0, 1, 2);
    layout1->addWidget(comboBox3, 7, 2, 1, 2);

    layout1->addWidget(lbl7,      8, 0, 1, 2);
    layout1->addWidget(lineEdit4, 8, 2, 1, 2);

    layout1->addWidget(btn3,      9, 0, 1, 6, Qt::AlignCenter);

    QVBoxLayout *layout2 = new QVBoxLayout;
    layout2->addLayout(layout1);
    layout2->addStretch();

    QHBoxLayout *layout3 = new QHBoxLayout(SDStorageWidget);
    layout3->addStretch(1);
    layout3->addLayout(layout2, 4);
    layout3->addStretch(1);

    addTab(SDStorageWidget, "SD卡管理");
}

void TabRecord::initSnapshotWidget()
{
    snapshotWidget = new QWidget(this);

    QCheckBox *cb1 = new QCheckBox("启用抓拍设置", snapshotWidget);
    snapshotMap.insert("Enable", cb1);

    QLabel *lbl1 = new QLabel("抓拍时间间隔(秒)(1-600):", snapshotWidget);
    QLineEdit *lineEdit1 = new QLineEdit(snapshotWidget);
    snapshotMap.insert("Interval", lineEdit1);

    TimeRegionWidget *region = new TimeRegionWidget(snapshotWidget);
    snapshotMap.insert("region", region);

    QCheckBox *cb2 = new QCheckBox("时间段1", snapshotWidget);
    cb2->setObjectName("Time Period 0");
    connect(cb2, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerTimeSelectState);
    connect(cb2, &QCheckBox::stateChanged, this, &TabRecord::handlerTimeSelect);
    snapshotMap.insert("Time Period 0", cb2);
    QTimeEdit *time1 = new QTimeEdit(snapshotWidget);
    time1->setEnabled(false);
    time1->setObjectName("Time Period 0 start");
    connect(time1, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handlerTimeChange);
    snapshotMap.insert("Time Period 0 start", time1);
    QLabel *lbl2 = new QLabel("-", snapshotWidget);
    QTimeEdit *time2 = new QTimeEdit(snapshotWidget);
    time2->setTime(QTime(23, 59, 59));
    time2->setEnabled(false);
    time2->setObjectName("Time Period 0 end");
    connect(time2, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handlerTimeChange);
    snapshotMap.insert("Time Period 0 end", time2);

    QCheckBox *cb3 = new QCheckBox("时间段2", snapshotWidget);
    cb3->setObjectName("Time Period 1");
    connect(cb3, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerTimeSelectState);
    connect(cb3, &QCheckBox::stateChanged, this, &TabRecord::handlerTimeSelect);
    snapshotMap.insert("Time Period 1", cb3);
    QTimeEdit *time3 = new QTimeEdit(snapshotWidget);
    time3->setEnabled(false);
    time3->setObjectName("Time Period 1 start");
    connect(time3, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handlerTimeChange);
    snapshotMap.insert("Time Period 1 start", time3);
    QLabel *lbl3 = new QLabel("-", snapshotWidget);
    QTimeEdit *time4 = new QTimeEdit(snapshotWidget);
    time4->setTime(QTime(23, 59, 59));
    time4->setEnabled(false);
    time4->setObjectName("Time Period 1 end");
    connect(time4, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handlerTimeChange);
    snapshotMap.insert("Time Period 1 end", time4);

    QCheckBox *cb4 = new QCheckBox("时间段3", snapshotWidget);
    cb4->setObjectName("Time Period 2");
    connect(cb4, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerTimeSelectState);
    connect(cb4, &QCheckBox::stateChanged, this, &TabRecord::handlerTimeSelect);
    snapshotMap.insert("Time Period 2", cb4);
    QTimeEdit *time5 = new QTimeEdit(snapshotWidget);
    time5->setEnabled(false);
    time5->setObjectName("Time Period 2 start");
    connect(time5, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handlerTimeChange);
    snapshotMap.insert("Time Period 2 start", time5);
    QLabel *lbl4 = new QLabel("-", snapshotWidget);
    QTimeEdit *time6 = new QTimeEdit(snapshotWidget);
    time6->setTime(QTime(23, 59, 59));
    time6->setEnabled(false);
    time6->setObjectName("Time Period 2 end");
    connect(time6, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handlerTimeChange);
    snapshotMap.insert("Time Period 2 end", time6);

    QCheckBox *cb5 = new QCheckBox("时间段4", snapshotWidget);
    cb5->setObjectName("Time Period 3");
    connect(cb5, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerTimeSelectState);
    connect(cb5, &QCheckBox::stateChanged, this, &TabRecord::handlerTimeSelect);
    snapshotMap.insert("Time Period 3", cb5);
    QTimeEdit *time7 = new QTimeEdit(snapshotWidget);
    time7->setEnabled(false);
    time7->setObjectName("Time Period 3 start");
    connect(time7, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handlerTimeChange);
    snapshotMap.insert("Time Period 3 start", time7);
    QLabel *lbl5 = new QLabel("-", snapshotWidget);
    QTimeEdit *time8 = new QTimeEdit(snapshotWidget);
    time8->setTime(QTime(23, 59, 59));
    time8->setEnabled(false);
    time8->setObjectName("Time Period 3 end");
    connect(time8, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handlerTimeChange);
    snapshotMap.insert("Time Period 3 end", time8);

    QPushButton *btn1 = new QPushButton("全选", snapshotWidget);
    connect(btn1, &QPushButton::clicked, this, &TabRecord::handlerSelectAllWeek);

    QCheckBox *cb6 = new QCheckBox("周天", snapshotWidget);
    cb6->setObjectName("week 0");
    connect(cb6, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerWeekSelectState);
    snapshotMap.insert("week 0", cb6);

    QCheckBox *cb7 = new QCheckBox("周一", snapshotWidget);
    cb7->setObjectName("week 1");
    connect(cb7, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerWeekSelectState);
    snapshotMap.insert("week 1", cb7);

    QCheckBox *cb8 = new QCheckBox("周二", snapshotWidget);
    cb8->setObjectName("week 2");
    connect(cb8, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerWeekSelectState);
    snapshotMap.insert("week 2", cb8);

    QCheckBox *cb9 = new QCheckBox("周三", snapshotWidget);
    cb9->setObjectName("week 3");
    connect(cb9, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerWeekSelectState);
    snapshotMap.insert("week 3", cb9);

    QCheckBox *cb10 = new QCheckBox("周四", snapshotWidget);
    cb10->setObjectName("week 4");
    connect(cb10, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerWeekSelectState);
    snapshotMap.insert("week 4", cb10);

    QCheckBox *cb11 = new QCheckBox("周五", snapshotWidget);
    cb11->setObjectName("week 5");
    connect(cb11, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerWeekSelectState);
    snapshotMap.insert("week 5", cb11);

    QCheckBox *cb12 = new QCheckBox("周六", snapshotWidget);
    cb12->setObjectName("week 6");
    connect(cb12, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerWeekSelectState);
    snapshotMap.insert("week 6", cb12);

    QPushButton *btn2 = new QPushButton("保存", snapshotWidget);
    connect(btn2, &QPushButton::clicked, this, &TabRecord::handlerPrepareData);

    QGridLayout *layout1 = new QGridLayout;
    layout1->addWidget(cb1,       0, 0, 1, 2);

    layout1->addWidget(lbl1,      1, 0, 1, 2);
    layout1->addWidget(lineEdit1, 1, 2, 1, 2);

    layout1->addWidget(region,    2, 0, 7, 6);

    QHBoxLayout *layout4 = new QHBoxLayout;
    layout4->addWidget(cb2);
    layout4->addWidget(time1);
    layout4->addWidget(lbl2 );
    layout4->addWidget(time2);
    layout1->addLayout(layout4, 9, 0, 1, 3);

    QHBoxLayout *layout5 = new QHBoxLayout;
    layout5->addWidget(cb3);
    layout5->addWidget(time3);
    layout5->addWidget(lbl3 );
    layout5->addWidget(time4);
    layout1->addLayout(layout5, 10, 0, 1, 3);

    QHBoxLayout *layout6 = new QHBoxLayout;
    layout6->addWidget(cb4);
    layout6->addWidget(time5);
    layout6->addWidget(lbl4 );
    layout6->addWidget(time6);
    layout1->addLayout(layout6, 11, 0, 1, 3);

    QHBoxLayout *layout7 = new QHBoxLayout;
    layout7->addWidget(cb5);
    layout7->addWidget(time7);
    layout7->addWidget(lbl5 );
    layout7->addWidget(time8);
    layout1->addLayout(layout7, 12, 0, 1, 3);

    layout1->addWidget(btn1,    13, 0, 1, 1);
    layout1->addWidget(cb6,     13, 1, 1, 1);
    layout1->addWidget(cb7,     13, 2, 1, 1);
    layout1->addWidget(cb8,     13, 3, 1, 1);
    layout1->addWidget(cb9,     13, 4, 1, 1);
    layout1->addWidget(cb10,    13, 5, 1, 1);
    layout1->addWidget(cb11,    13, 6, 1, 1);
    layout1->addWidget(cb12,    13, 7, 1, 1);

    layout1->addWidget(btn2,    14, 0, 1, 8, Qt::AlignCenter);

    QVBoxLayout *layout2 = new QVBoxLayout;
    layout2->addLayout(layout1);
    layout2->addStretch();

    QHBoxLayout *layout3 = new QHBoxLayout(snapshotWidget);
    layout3->addStretch(1);
    layout3->addLayout(layout2, 4);
    layout3->addStretch(1);

    addTab(snapshotWidget, "抓拍设置");
}

void TabRecord::initDestinationWidget()
{
    destinationWidget = new QWidget(this);

    QLabel *lbl1 = new QLabel("EventType", destinationWidget);
    QLabel *lbl2 = new QLabel("Scheduled", destinationWidget);
    QLabel *lbl3 = new QLabel("Motion", destinationWidget);
    QLabel *lbl4 = new QLabel("Alarm", destinationWidget);

    QLabel *lbl5 = new QLabel("SDCard", destinationWidget);
    QCheckBox *cb1 = new QCheckBox(destinationWidget);
    destinationMap.insert("SDCard Scheduled", cb1);
    QCheckBox *cb2 = new QCheckBox(destinationWidget);
    destinationMap.insert("SDCard Motion", cb2);
    QCheckBox *cb3 = new QCheckBox(destinationWidget);
    destinationMap.insert("SDCard Alarm", cb3);

    QLabel *lbl6 = new QLabel("FTP", destinationWidget);
    QCheckBox *cb4 = new QCheckBox(destinationWidget);
    destinationMap.insert("FTP Scheduled", cb4);
    QCheckBox *cb5 = new QCheckBox(destinationWidget);
    destinationMap.insert("FTP Motion", cb5);
    QCheckBox *cb6 = new QCheckBox(destinationWidget);
    destinationMap.insert("FTP Alarm", cb6);

    QLabel *lbl7 = new QLabel("NAS", destinationWidget);
    QCheckBox *cb7 = new QCheckBox(destinationWidget);
    destinationMap.insert("NAS Scheduled", cb7);
    QCheckBox *cb8 = new QCheckBox(destinationWidget);
    destinationMap.insert("NAS Motion", cb8);
    QCheckBox *cb9 = new QCheckBox(destinationWidget);
    destinationMap.insert("NAS Alarm", cb9);

    QPushButton *btn1 = new QPushButton("Save", destinationWidget);

    QGridLayout *layout1 = new QGridLayout;
    layout1->addWidget(lbl1,     0, 0, 1, 1, Qt::AlignCenter);
    layout1->addWidget(lbl2,     0, 1, 1, 1, Qt::AlignCenter);
    layout1->addWidget(lbl3,     0, 2, 1, 1, Qt::AlignCenter);
    layout1->addWidget(lbl4,     0, 3, 1, 1, Qt::AlignCenter);

    layout1->addWidget(lbl5,     1, 0, 1, 1, Qt::AlignCenter);
    layout1->addWidget(cb1,      1, 1, 1, 1, Qt::AlignCenter);
    layout1->addWidget(cb2,      1, 2, 1, 1, Qt::AlignCenter);
    layout1->addWidget(cb3,      1, 3, 1, 1, Qt::AlignCenter);

    layout1->addWidget(lbl6,     2, 0, 1, 1, Qt::AlignCenter);
    layout1->addWidget(cb4,      2, 1, 1, 1, Qt::AlignCenter);
    layout1->addWidget(cb5,      2, 2, 1, 1, Qt::AlignCenter);
    layout1->addWidget(cb6,      2, 3, 1, 1, Qt::AlignCenter);

    layout1->addWidget(lbl7,     3, 0, 1, 1, Qt::AlignCenter);
    layout1->addWidget(cb7,      3, 1, 1, 1, Qt::AlignCenter);
    layout1->addWidget(cb8,      3, 2, 1, 1, Qt::AlignCenter);
    layout1->addWidget(cb9,      3, 3, 1, 1, Qt::AlignCenter);

    layout1->addWidget(btn1,     4, 0, 1, 4, Qt::AlignCenter);

    QVBoxLayout *layout2 = new QVBoxLayout;
    layout2->addLayout(layout1);
    layout2->addStretch();

    QHBoxLayout *layout3 = new QHBoxLayout(destinationWidget);
    layout3->addStretch(1);
    layout3->addLayout(layout2, 4);
    layout3->addStretch(1);

    addTab(destinationWidget, "Destination");
}

void TabRecord::initNASWidget()
{
    NASWidget = new QWidget(this);

    QRadioButton *rBtn1 = new QRadioButton("Enable", NASWidget);
    NASMap.insert("Enable", rBtn1);

    QLabel *lbl1 = new QLabel("Server Address:", NASWidget);
    QLineEdit *lineEdit1 = new QLineEdit(NASWidget);
    NASMap.insert("Server Address", lineEdit1);

    QLabel *lbl2 = new QLabel("Remote Directory", NASWidget);
    QLineEdit *lineEdit2 = new QLineEdit(NASWidget);
    NASMap.insert("Remote Directory", lineEdit2);

    QPushButton *btn1 = new QPushButton("Save", NASWidget);

    QGridLayout *layout1 = new QGridLayout;
    layout1->addWidget(rBtn1,     0, 0, 1, 2);

    layout1->addWidget(lbl1,      1, 0, 1, 2);
    layout1->addWidget(lineEdit1, 1, 2, 1, 2);

    layout1->addWidget(lbl2,      2, 0, 1, 2);
    layout1->addWidget(lineEdit2, 2, 2, 1, 2);

    layout1->addWidget(btn1,      3, 0, 1, 4, Qt::AlignCenter);

    QVBoxLayout *layout2 = new QVBoxLayout;
    layout2->addLayout(layout1);
    layout2->addStretch();

    QHBoxLayout *layout3 = new QHBoxLayout(NASWidget);
    layout3->addStretch(1);
    layout3->addLayout(layout2, 4);
    layout3->addStretch(1);

    addTab(NASWidget, "NAS");
}

void TabRecord::handlerSwitchTab(const QModelIndex &index)
{
    int type = index.row();
    if(sender() != this) {
        setCurrentIndex(index.row());
    }else {
        type = currentIndex();
    }

    switch(type){
    case 0: {
        emit signalGetParameter(SCHEDULEPARAMETER);
        break;
    }
    case 1: {
        emit signalGetParameter(SDCARDPARAMETER);
        emit signalGetParameter(SDSTORAGEPARAMETER);
        break;
    }
    case 2: {
        emit signalGetParameter(SNAPSHOTPARAMETER);
        break;
    }
    case 3: {
        emit signalGetParameter(DESTINATIONPARAMETER);
        break;
    }
    case 4: {
        emit signalGetParameter(NASPARAMETER);
        break;
    }
    default:
        break;
    }
}

void TabRecord::handlerTimeSelect(int state)
{
    TimeRegionWidget *region = NULL;
    QMap<QString, QWidget *> map;
    switch (currentIndex()) {
    case 0:
        region = static_cast<TimeRegionWidget *>(scheduleMap.value("region"));
        map = scheduleMap;
        break;
    case 2:
        region = static_cast<TimeRegionWidget *>(snapshotMap.value("region"));
        map = snapshotMap;
        break;
    default:
        return;
        break;
    }
    if(state == Qt::Checked){
        QTimeEdit *startTime = static_cast<QTimeEdit *>(map.value(QString("%1 start").arg(sender()->objectName())));
        QTimeEdit *endTime = static_cast<QTimeEdit *>(map.value(QString("%1 end").arg(sender()->objectName())));
        startTime->setEnabled(true);
        endTime->setEnabled(true);
        region->insertTimeRegion(sender()->objectName().right(1).toInt(), startTime->time(), endTime->time());
    }else{
        static_cast<QTimeEdit *>(map.value(QString("%1 start").arg(sender()->objectName())))->setEnabled(false);
        static_cast<QTimeEdit *>(map.value(QString("%1 end").arg(sender()->objectName())))->setEnabled(false);
    }
}

void TabRecord::handlerSelectAllWeek()
{
    QMap<QString, QWidget *> map;
    switch (currentIndex()) {
    case 0:
        map = scheduleMap;
        break;
        break;
    case 2:
        map = snapshotMap;
        break;
    default:
        return;
        break;
    }
    for(int i=0; i<7; i++){
        QCheckBox *cb = static_cast<QCheckBox *>(map.value(QString("week %1").arg(i)));
        cb->setCheckState(Qt::Checked);
    }
}

void TabRecord::handlerPrepareData()
{
    switch(currentIndex()) {
    case 0: {
        VidiconProtocol::RemoteRecordingPlan *param = new VidiconProtocol::RemoteRecordingPlan;
        param->Enabled = static_cast<QCheckBox *>(scheduleMap["Enable"])->checkState() ? 1 : 0;
        param->weeksStateMap = static_cast<TimeRegionWidget *>(scheduleMap["region"])->getWeeksState();
        param->Plans = static_cast<TimeRegionWidget *>(scheduleMap["region"])->getPlans();

        emit signalSetParameter(SCHEDULEPARAMETER, param);
        break;
    }
    case 1: {
        VidiconProtocol::SDStorageParameter *param = new VidiconProtocol::SDStorageParameter;
        param->OperType = static_cast<QComboBox *>(SDStorageMap["Overwrite"])->currentIndex();
        param->RecordSelect = static_cast<QComboBox *>(SDStorageMap["Stream"])->currentIndex();
        param->RecordMode = static_cast<QComboBox *>(SDStorageMap["Record Type"])->currentIndex();
        param->RecordTime = static_cast<QLineEdit *>(SDStorageMap["Record Time"])->text().toInt();

        emit signalSetParameter(SDSTORAGEPARAMETER, param);
        break;
    }
    case 2: {
        VidiconProtocol::SnapshotPlanParameter *param = new VidiconProtocol::SnapshotPlanParameter;
        param->Enabled = static_cast<QCheckBox *>(snapshotMap["Enable"])->checkState() ? 1 : 0;
        param->SnapIntervalTime = static_cast<QLineEdit *>(snapshotMap["Interval"])->text().toInt();
        param->weeksStateMap = static_cast<TimeRegionWidget *>(snapshotMap["region"])->getWeeksState();
        param->Plans = static_cast<TimeRegionWidget *>(snapshotMap["region"])->getPlans();

        emit signalSetParameter(SNAPSHOTPARAMETER, param);
    }
    default:
        break;
    }
}

void TabRecord::handlerReceiveData(int type, QByteArray data)
{
    switch(type) {
    case SCHEDULEPARAMETER: {
        VidiconProtocol::RemoteRecordingPlan param;
        param.Plans = static_cast<TimeRegionWidget *>(scheduleMap["region"])->getPlans();
        if(ParseXML::getInstance()->parseScheduleParameter(&param, data)) {
            static_cast<QCheckBox *>(scheduleMap["Enable"])->setChecked(param.Enabled ? true : false);

            int week = QDate::currentDate().dayOfWeek() == 7 ? 0 : QDate::currentDate().dayOfWeek();
            static_cast<QCheckBox *>(scheduleMap[QString("week %1").arg(week)])->setCheckState(Qt::Checked);
            for(int i=0;i <4; i++) {
                static_cast<QCheckBox *>(scheduleMap[QString("Time Period %1").arg(i)])->setChecked(param.Plans[week][i].PlanTimeEnabled ? true : false);
                static_cast<QTimeEdit *>(scheduleMap[QString("Time Period %1 start").arg(i)])->setTime(param.Plans[week][i].BeginTime);
                static_cast<QTimeEdit *>(scheduleMap[QString("Time Period %1 end").arg(i)])->setTime(param.Plans[week][i].EndTime);
                static_cast<QTimeEdit *>(scheduleMap[QString("Time Period %1 start").arg(i)])->setEnabled(param.Plans[week][i].PlanTimeEnabled ? true : false);
                static_cast<QTimeEdit *>(scheduleMap[QString("Time Period %1 end").arg(i)])->setEnabled(param.Plans[week][i].PlanTimeEnabled ? true : false);
            }
            qDebug() << "#TabRecord# handlerReceiveData, ParameterType:" << type << "parse data success...";
        }else{
            qDebug() << "#TabRecord# handlerReceiveData, ParameterType:" << type << "parse data error...";
        }
        break;
    }
    case SNAPSHOTPARAMETER: {
        VidiconProtocol::SnapshotPlanParameter param;
        param.Plans = static_cast<TimeRegionWidget *>(snapshotMap["region"])->getPlans();
        if(ParseXML::getInstance()->parseSnapshotParameter(&param, data)) {
            static_cast<QCheckBox *>(snapshotMap["Enable"])->setChecked(param.Enabled ? true : false);
            static_cast<QLineEdit *>(snapshotMap["Interval"])->setText(QString::number(param.SnapIntervalTime));

            int week = QDate::currentDate().dayOfWeek() == 7 ? 0 : QDate::currentDate().dayOfWeek();
            static_cast<QCheckBox *>(snapshotMap[QString("week %1").arg(week)])->setCheckState(Qt::Checked);
            for(int i=0;i <4; i++) {
                static_cast<QCheckBox *>(snapshotMap[QString("Time Period %1").arg(i)])->setChecked(param.Plans[week][i].PlanTimeEnabled ? true : false);
                static_cast<QTimeEdit *>(snapshotMap[QString("Time Period %1 start").arg(i)])->setTime(param.Plans[week][i].BeginTime);
                static_cast<QTimeEdit *>(snapshotMap[QString("Time Period %1 end").arg(i)])->setTime(param.Plans[week][i].EndTime);
                static_cast<QTimeEdit *>(snapshotMap[QString("Time Period %1 start").arg(i)])->setEnabled(param.Plans[week][i].PlanTimeEnabled ? true : false);
                static_cast<QTimeEdit *>(snapshotMap[QString("Time Period %1 end").arg(i)])->setEnabled(param.Plans[week][i].PlanTimeEnabled ? true : false);
            }
            qDebug() << "#TabRecord# handlerReceiveData, ParameterType:" << type << "parse data success...";
        }else{
            qDebug() << "#TabRecord# handlerReceiveData, ParameterType:" << type << "parse data error...";
        }
        break;
    }
    case SDCARDPARAMETER: {
        VidiconProtocol::SDCardStatus param;
        if(ParseXML::getInstance()->parseSDCardStatusParameter(&param, data)) {
            static_cast<QLineEdit *>(SDStorageMap["Used Space"])->setText(QString::number(param.UsedKByte));
            static_cast<QLineEdit *>(SDStorageMap["Available Space"])->setText(QString::number(param.AvailableKByte));
            static_cast<QLineEdit *>(SDStorageMap["Total Space"])->setText(QString::number(param.TotoalSizeKByte));
            qDebug() << "#TabRecord# handlerReceiveData, ParameterType:" << type << "parse data success...";
        }else{
            qDebug() << "#TabRecord# handlerReceiveData, ParameterType:" << type << "parse data error...";
        }
        break;
    }
    case SDSTORAGEPARAMETER: {
        VidiconProtocol::SDStorageParameter param;
        if(ParseXML::getInstance()->parseSDStorageParameter(&param, data)) {
            static_cast<QComboBox *>(SDStorageMap["Overwrite"])->setCurrentIndex(param.OperType);
            static_cast<QComboBox *>(SDStorageMap["Stream"])->setCurrentIndex(param.RecordSelect);
            static_cast<QComboBox *>(SDStorageMap["Record Type"])->setCurrentIndex(param.RecordSelect);
            static_cast<QLineEdit *>(SDStorageMap["Record Time"])->setText(QString::number(param.RecordTime));
            qDebug() << "#TabRecord# handlerReceiveData, ParameterType:" << type << "parse data success...";
        }else{
            qDebug() << "#TabRecord# handlerReceiveData, ParameterType:" << type << "parse data error...";
        }
        break;
    }
    default:
        break;
    }
}
