#include "recordwidget.h"
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

RecordWidget::RecordWidget(QWidget *parent) :
    StackedWidget(parent)
{
    initScheduleWidget();
    initSDStorageWidget();
    initSnapshotWidget();
//    initDestinationWidget();
//    initNASWidget();
}

RecordWidget::~RecordWidget()
{
}

void RecordWidget::initScheduleWidget()
{
    QStringList list;
    m_scheduleWidget = new QWidget(this);

    QCheckBox *cb1 = new QCheckBox("启用录像计划", m_scheduleWidget);
    m_scheduleMap.insert("Enable", cb1);

    TimeRegionWidget *region = new TimeRegionWidget(m_scheduleWidget);
    m_scheduleMap.insert("region", region);

    QCheckBox *cb2 = new QCheckBox("时间段1", m_scheduleWidget);
    cb2->setObjectName("Time Period 0");
    connect(cb2, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleTimeSelectState);
    connect(cb2, &QCheckBox::stateChanged, this, &RecordWidget::handleTimeSelect);
    m_scheduleMap.insert("Time Period 0", cb2);
    QTimeEdit *time1 = new QTimeEdit(m_scheduleWidget);
    time1->setEnabled(false);
    time1->setObjectName("Time Period 0 start");
    connect(time1, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handleTimeChange);
    m_scheduleMap.insert("Time Period 0 start", time1);
    QLabel *lbl1 = new QLabel("-", m_scheduleWidget);
    QTimeEdit *time2 = new QTimeEdit(m_scheduleWidget);
    time2->setTime(QTime(23, 59, 59));
    time2->setEnabled(false);
    time2->setObjectName("Time Period 0 end");
    connect(time2, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handleTimeChange);
    m_scheduleMap.insert("Time Period 0 end", time2);

    QCheckBox *cb3 = new QCheckBox("时间段2", m_scheduleWidget);
    cb3->setObjectName("Time Period 1");
    connect(cb3, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleTimeSelectState);
    connect(cb3, &QCheckBox::stateChanged, this, &RecordWidget::handleTimeSelect);
    m_scheduleMap.insert("Time Period 1", cb3);
    QTimeEdit *time3 = new QTimeEdit(m_scheduleWidget);
    time3->setEnabled(false);
    time3->setObjectName("Time Period 1 start");
    connect(time3, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handleTimeChange);
    m_scheduleMap.insert("Time Period 1 start", time3);
    QLabel *lbl2 = new QLabel("-", m_scheduleWidget);
    QTimeEdit *time4 = new QTimeEdit(m_scheduleWidget);
    time4->setTime(QTime(23, 59, 59));
    time4->setEnabled(false);
    time4->setObjectName("Time Period 1 end");
    connect(time4, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handleTimeChange);
    m_scheduleMap.insert("Time Period 1 end", time4);

    QCheckBox *cb4 = new QCheckBox("时间段3", m_scheduleWidget);
    cb4->setObjectName("Time Period 2");
    connect(cb4, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleTimeSelectState);
    connect(cb4, &QCheckBox::stateChanged, this, &RecordWidget::handleTimeSelect);
    m_scheduleMap.insert("Time Period 2", cb4);
    QTimeEdit *time5 = new QTimeEdit(m_scheduleWidget);
    time5->setEnabled(false);
    time5->setObjectName("Time Period 2 start");
    connect(time5, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handleTimeChange);
    m_scheduleMap.insert("Time Period 2 start", time5);
    QLabel *lbl3 = new QLabel("-", m_scheduleWidget);
    QTimeEdit *time6 = new QTimeEdit(m_scheduleWidget);
    time6->setTime(QTime(23, 59, 59));
    time6->setEnabled(false);
    time6->setObjectName("Time Period 2 end");
    connect(time6, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handleTimeChange);
    m_scheduleMap.insert("Time Period 2 end", time6);

    QCheckBox *cb5 = new QCheckBox("时间段4", m_scheduleWidget);
    cb5->setObjectName("Time Period 3");
    connect(cb5, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleTimeSelectState);
    connect(cb5, &QCheckBox::stateChanged, this, &RecordWidget::handleTimeSelect);
    m_scheduleMap.insert("Time Period 3", cb5);
    QTimeEdit *time7 = new QTimeEdit(m_scheduleWidget);
    time7->setEnabled(false);
    time7->setObjectName("Time Period 3 start");
    connect(time7, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handleTimeChange);
    m_scheduleMap.insert("Time Period 3 start", time7);
    QLabel *lbl4 = new QLabel("-", m_scheduleWidget);
    QTimeEdit *time8 = new QTimeEdit(m_scheduleWidget);
    time8->setTime(QTime(23, 59, 59));
    time8->setEnabled(false);
    time8->setObjectName("Time Period 3 end");
    connect(time8, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handleTimeChange);
    m_scheduleMap.insert("Time Period 3 end", time8);

    QPushButton *btn1 = new QPushButton("全选", m_scheduleWidget);
    connect(btn1, &QPushButton::clicked, this, &RecordWidget::handleSelectAllWeek);

    QCheckBox *cb6 = new QCheckBox("周天", m_scheduleWidget);
    cb6->setObjectName("week 0");
    connect(cb6, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleWeekSelectState);
    m_scheduleMap.insert("week 0", cb6);

    QCheckBox *cb7 = new QCheckBox("周一", m_scheduleWidget);
    cb7->setObjectName("week 1");
    connect(cb7, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleWeekSelectState);
    m_scheduleMap.insert("week 1", cb7);

    QCheckBox *cb8 = new QCheckBox("周二", m_scheduleWidget);
    cb8->setObjectName("week 2");
    connect(cb8, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleWeekSelectState);
    m_scheduleMap.insert("week 2", cb8);

    QCheckBox *cb9 = new QCheckBox("周三", m_scheduleWidget);
    cb9->setObjectName("week 3");
    connect(cb9, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleWeekSelectState);
    m_scheduleMap.insert("week 3", cb9);

    QCheckBox *cb10 = new QCheckBox("周四", m_scheduleWidget);
    cb10->setObjectName("week 4");
    connect(cb10, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleWeekSelectState);
    m_scheduleMap.insert("week 4", cb10);

    QCheckBox *cb11 = new QCheckBox("周五", m_scheduleWidget);
    cb11->setObjectName("week 5");
    connect(cb11, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleWeekSelectState);
    m_scheduleMap.insert("week 5", cb11);

    QCheckBox *cb12 = new QCheckBox("周六", m_scheduleWidget);
    cb12->setObjectName("week 6");
    connect(cb12, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleWeekSelectState);
    m_scheduleMap.insert("week 6", cb12);

    QPushButton *btn2 = new QPushButton("保存", m_scheduleWidget);
    connect(btn2, &QPushButton::clicked, this, &RecordWidget::handlePrepareData);

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

    setAlignment(m_scheduleWidget, layout1, Qt::AlignTop | Qt::AlignHCenter);
    addWidget(m_scheduleWidget);
}

void RecordWidget::initSDStorageWidget()
{
    QStringList list;
    m_SDStorageWidget = new QWidget(this);

    QRadioButton *rBtn1 = new QRadioButton("掉线自动录像", m_SDStorageWidget);
    m_SDStorageMap.insert("Offline Record", rBtn1);
    QRadioButton *rBtn2 = new QRadioButton("自动续传", m_SDStorageWidget);
    m_SDStorageMap.insert("Auto Upload", rBtn2);

    rBtn1->setVisible(false);
    rBtn2->setVisible(false);

    QLabel *lbl1 = new QLabel("SD总大小：", m_SDStorageWidget);
    LineEdit *lineEdit1 = new LineEdit(m_SDStorageWidget);
    lineEdit1->setReadOnly(true);
    m_SDStorageMap.insert("Total Space", lineEdit1);

    QLabel *lbl2 = new QLabel("SD已使用大小：", m_SDStorageWidget);
    LineEdit *lineEdit2 = new LineEdit(m_SDStorageWidget);
    lineEdit2->setReadOnly(true);
    m_SDStorageMap.insert("Used Space", lineEdit2);

    QLabel *lbl3 = new QLabel("SD未使用大小：", m_SDStorageWidget);
    LineEdit *lineEdit3 = new LineEdit(m_SDStorageWidget);
    lineEdit3->setReadOnly(true);
    m_SDStorageMap.insert("Available Space", lineEdit3);

    QPushButton *btn1 = new QPushButton("刷新", m_SDStorageWidget);
    connect(btn1, &QPushButton::clicked, this, [this](){ emit signalGetParameter(VidiconProtocol::SDCARD); });
    QPushButton *btn2 = new QPushButton("格式化", m_SDStorageWidget);
    connect(btn2, &QPushButton::clicked, this, [this](){
        if(QMessageBox::question(this, "警告", "是否格式化SD卡") == QMessageBox::Yes) {
            emit signalSetParameter(VidiconProtocol::FORMATSDCARD);
        }
    });

    QLabel *lbl4 = new QLabel("SD内存已满时：", m_SDStorageWidget);
    QComboBox *comboBox1 = new QComboBox(m_SDStorageWidget);
    list << "循环录像" << "停止录像";
    comboBox1->addItems(list);
    list.clear();
    m_SDStorageMap.insert("Overwrite", comboBox1);

    QLabel *lbl5 = new QLabel("录像码流：", m_SDStorageWidget);
    QComboBox *comboBox2 = new QComboBox(m_SDStorageWidget);
    list << "主码流" << "子码流";
    comboBox2->addItems(list);
    list.clear();
    m_SDStorageMap.insert("Stream", comboBox2);

    QLabel *lbl6 = new QLabel("录像类型：", m_SDStorageWidget);
    QComboBox *comboBox3 = new QComboBox(m_SDStorageWidget);
    list << "ivd" << "mp4";
    comboBox3->addItems(list);
    list.clear();
    m_SDStorageMap.insert("Record Type", comboBox1);

    QLabel *lbl7 = new QLabel("录像时间(100-600秒)：", m_SDStorageWidget);
    LineEdit *lineEdit4 = new LineEdit(m_SDStorageWidget);
    lineEdit4->setValidator(new QIntValidator(100, 600, m_SDStorageWidget));
    m_SDStorageMap.insert("Record Time", lineEdit4);

    QPushButton *btn3 = new QPushButton("保存", m_SDStorageWidget);
    connect(btn3, &QPushButton::clicked, this, &RecordWidget::handlePrepareData);

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

    setAlignment(m_SDStorageWidget, layout1, Qt::AlignTop | Qt::AlignHCenter);
    addWidget(m_SDStorageWidget);
}

void RecordWidget::initSnapshotWidget()
{
    m_snapshotWidget = new QWidget(this);

    QCheckBox *cb1 = new QCheckBox("启用抓拍设置", m_snapshotWidget);
    m_snapshotMap.insert("Enable", cb1);

    QLabel *lbl1 = new QLabel("抓拍时间间隔(秒)(1-600):", m_snapshotWidget);
    LineEdit *lineEdit1 = new LineEdit(m_snapshotWidget);
    lineEdit1->setValidator(new QIntValidator(1, 600, m_snapshotWidget));
    m_snapshotMap.insert("Interval", lineEdit1);

    TimeRegionWidget *region = new TimeRegionWidget(m_snapshotWidget);
    m_snapshotMap.insert("region", region);

    QCheckBox *cb2 = new QCheckBox("时间段1", m_snapshotWidget);
    cb2->setObjectName("Time Period 0");
    connect(cb2, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleTimeSelectState);
    connect(cb2, &QCheckBox::stateChanged, this, &RecordWidget::handleTimeSelect);
    m_snapshotMap.insert("Time Period 0", cb2);
    QTimeEdit *time1 = new QTimeEdit(m_snapshotWidget);
    time1->setEnabled(false);
    time1->setObjectName("Time Period 0 start");
    connect(time1, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handleTimeChange);
    m_snapshotMap.insert("Time Period 0 start", time1);
    QLabel *lbl2 = new QLabel("-", m_snapshotWidget);
    QTimeEdit *time2 = new QTimeEdit(m_snapshotWidget);
    time2->setTime(QTime(23, 59, 59));
    time2->setEnabled(false);
    time2->setObjectName("Time Period 0 end");
    connect(time2, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handleTimeChange);
    m_snapshotMap.insert("Time Period 0 end", time2);

    QCheckBox *cb3 = new QCheckBox("时间段2", m_snapshotWidget);
    cb3->setObjectName("Time Period 1");
    connect(cb3, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleTimeSelectState);
    connect(cb3, &QCheckBox::stateChanged, this, &RecordWidget::handleTimeSelect);
    m_snapshotMap.insert("Time Period 1", cb3);
    QTimeEdit *time3 = new QTimeEdit(m_snapshotWidget);
    time3->setEnabled(false);
    time3->setObjectName("Time Period 1 start");
    connect(time3, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handleTimeChange);
    m_snapshotMap.insert("Time Period 1 start", time3);
    QLabel *lbl3 = new QLabel("-", m_snapshotWidget);
    QTimeEdit *time4 = new QTimeEdit(m_snapshotWidget);
    time4->setTime(QTime(23, 59, 59));
    time4->setEnabled(false);
    time4->setObjectName("Time Period 1 end");
    connect(time4, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handleTimeChange);
    m_snapshotMap.insert("Time Period 1 end", time4);

    QCheckBox *cb4 = new QCheckBox("时间段3", m_snapshotWidget);
    cb4->setObjectName("Time Period 2");
    connect(cb4, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleTimeSelectState);
    connect(cb4, &QCheckBox::stateChanged, this, &RecordWidget::handleTimeSelect);
    m_snapshotMap.insert("Time Period 2", cb4);
    QTimeEdit *time5 = new QTimeEdit(m_snapshotWidget);
    time5->setEnabled(false);
    time5->setObjectName("Time Period 2 start");
    connect(time5, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handleTimeChange);
    m_snapshotMap.insert("Time Period 2 start", time5);
    QLabel *lbl4 = new QLabel("-", m_snapshotWidget);
    QTimeEdit *time6 = new QTimeEdit(m_snapshotWidget);
    time6->setTime(QTime(23, 59, 59));
    time6->setEnabled(false);
    time6->setObjectName("Time Period 2 end");
    connect(time6, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handleTimeChange);
    m_snapshotMap.insert("Time Period 2 end", time6);

    QCheckBox *cb5 = new QCheckBox("时间段4", m_snapshotWidget);
    cb5->setObjectName("Time Period 3");
    connect(cb5, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleTimeSelectState);
    connect(cb5, &QCheckBox::stateChanged, this, &RecordWidget::handleTimeSelect);
    m_snapshotMap.insert("Time Period 3", cb5);
    QTimeEdit *time7 = new QTimeEdit(m_snapshotWidget);
    time7->setEnabled(false);
    time7->setObjectName("Time Period 3 start");
    connect(time7, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handleTimeChange);
    m_snapshotMap.insert("Time Period 3 start", time7);
    QLabel *lbl5 = new QLabel("-", m_snapshotWidget);
    QTimeEdit *time8 = new QTimeEdit(m_snapshotWidget);
    time8->setTime(QTime(23, 59, 59));
    time8->setEnabled(false);
    time8->setObjectName("Time Period 3 end");
    connect(time8, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handleTimeChange);
    m_snapshotMap.insert("Time Period 3 end", time8);

    QPushButton *btn1 = new QPushButton("全选", m_snapshotWidget);
    connect(btn1, &QPushButton::clicked, this, &RecordWidget::handleSelectAllWeek);

    QCheckBox *cb6 = new QCheckBox("周天", m_snapshotWidget);
    cb6->setObjectName("week 0");
    connect(cb6, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleWeekSelectState);
    m_snapshotMap.insert("week 0", cb6);

    QCheckBox *cb7 = new QCheckBox("周一", m_snapshotWidget);
    cb7->setObjectName("week 1");
    connect(cb7, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleWeekSelectState);
    m_snapshotMap.insert("week 1", cb7);

    QCheckBox *cb8 = new QCheckBox("周二", m_snapshotWidget);
    cb8->setObjectName("week 2");
    connect(cb8, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleWeekSelectState);
    m_snapshotMap.insert("week 2", cb8);

    QCheckBox *cb9 = new QCheckBox("周三", m_snapshotWidget);
    cb9->setObjectName("week 3");
    connect(cb9, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleWeekSelectState);
    m_snapshotMap.insert("week 3", cb9);

    QCheckBox *cb10 = new QCheckBox("周四", m_snapshotWidget);
    cb10->setObjectName("week 4");
    connect(cb10, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleWeekSelectState);
    m_snapshotMap.insert("week 4", cb10);

    QCheckBox *cb11 = new QCheckBox("周五", m_snapshotWidget);
    cb11->setObjectName("week 5");
    connect(cb11, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleWeekSelectState);
    m_snapshotMap.insert("week 5", cb11);

    QCheckBox *cb12 = new QCheckBox("周六", m_snapshotWidget);
    cb12->setObjectName("week 6");
    connect(cb12, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleWeekSelectState);
    m_snapshotMap.insert("week 6", cb12);

    QPushButton *btn2 = new QPushButton("保存", m_snapshotWidget);
    connect(btn2, &QPushButton::clicked, this, &RecordWidget::handlePrepareData);

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

    setAlignment(m_snapshotWidget, layout1, Qt::AlignTop | Qt::AlignHCenter);
    addWidget(m_snapshotWidget);
}

void RecordWidget::initDestinationWidget()
{
    m_destinationWidget = new QWidget(this);

    QLabel *lbl1 = new QLabel("EventType", m_destinationWidget);
    QLabel *lbl2 = new QLabel("Scheduled", m_destinationWidget);
    QLabel *lbl3 = new QLabel("Motion", m_destinationWidget);
    QLabel *lbl4 = new QLabel("Alarm", m_destinationWidget);

    QLabel *lbl5 = new QLabel("SDCard", m_destinationWidget);
    QCheckBox *cb1 = new QCheckBox(m_destinationWidget);
    m_destinationMap.insert("SDCard Scheduled", cb1);
    QCheckBox *cb2 = new QCheckBox(m_destinationWidget);
    m_destinationMap.insert("SDCard Motion", cb2);
    QCheckBox *cb3 = new QCheckBox(m_destinationWidget);
    m_destinationMap.insert("SDCard Alarm", cb3);

    QLabel *lbl6 = new QLabel("FTP", m_destinationWidget);
    QCheckBox *cb4 = new QCheckBox(m_destinationWidget);
    m_destinationMap.insert("FTP Scheduled", cb4);
    QCheckBox *cb5 = new QCheckBox(m_destinationWidget);
    m_destinationMap.insert("FTP Motion", cb5);
    QCheckBox *cb6 = new QCheckBox(m_destinationWidget);
    m_destinationMap.insert("FTP Alarm", cb6);

    QLabel *lbl7 = new QLabel("NAS", m_destinationWidget);
    QCheckBox *cb7 = new QCheckBox(m_destinationWidget);
    m_destinationMap.insert("NAS Scheduled", cb7);
    QCheckBox *cb8 = new QCheckBox(m_destinationWidget);
    m_destinationMap.insert("NAS Motion", cb8);
    QCheckBox *cb9 = new QCheckBox(m_destinationWidget);
    m_destinationMap.insert("NAS Alarm", cb9);

    QPushButton *btn1 = new QPushButton("Save", m_destinationWidget);

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

    setAlignment(m_destinationWidget, layout1, Qt::AlignTop | Qt::AlignHCenter);
    addWidget(m_destinationWidget);
}

void RecordWidget::initNASWidget()
{
    m_NASWidget = new QWidget(this);

    QRadioButton *rBtn1 = new QRadioButton("Enable", m_NASWidget);
    m_NASMap.insert("Enable", rBtn1);

    QLabel *lbl1 = new QLabel("Server Address:", m_NASWidget);
    LineEdit *lineEdit1 = new LineEdit(m_NASWidget);
    m_NASMap.insert("Server Address", lineEdit1);

    QLabel *lbl2 = new QLabel("Remote Directory", m_NASWidget);
    LineEdit *lineEdit2 = new LineEdit(m_NASWidget);
    m_NASMap.insert("Remote Directory", lineEdit2);

    QPushButton *btn1 = new QPushButton("Save", m_NASWidget);

    QGridLayout *layout1 = new QGridLayout;
    layout1->addWidget(rBtn1,     0, 0, 1, 2);

    layout1->addWidget(lbl1,      1, 0, 1, 2);
    layout1->addWidget(lineEdit1, 1, 2, 1, 2);

    layout1->addWidget(lbl2,      2, 0, 1, 2);
    layout1->addWidget(lineEdit2, 2, 2, 1, 2);

    layout1->addWidget(btn1,      3, 0, 1, 4, Qt::AlignCenter);

    setAlignment(m_NASWidget, layout1, Qt::AlignTop | Qt::AlignHCenter);
    addWidget(m_NASWidget);
}

void RecordWidget::setCurrentIndex(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    switch(index.row()){
    case 0: {
        emit signalGetParameter(VidiconProtocol::SCHEDULE);
        break;
    }
    case 1: {
        emit signalGetParameter(VidiconProtocol::SDCARD);
        emit signalGetParameter(VidiconProtocol::SDSTORAGE);
        break;
    }
    case 2: {
        emit signalGetParameter(VidiconProtocol::SNAPSHOT);
        break;
    }
    case 3: {
        emit signalGetParameter(VidiconProtocol::DESTINATION);
        break;
    }
    case 4: {
        emit signalGetParameter(VidiconProtocol::NAS);
        break;
    }
    default:
        break;
    }

    StackedWidget::setCurrentIndex(index.row());
}

void RecordWidget::handlePrepareData()
{
    switch(currentIndex()) {
    case 0: {
        RemoteRecordingPlan param;
        param.Enabled = static_cast<QCheckBox *>(m_scheduleMap["Enable"])->checkState() ? 1 : 0;
        param.weeksStateMap = static_cast<TimeRegionWidget *>(m_scheduleMap["region"])->getWeeksState();
        param.Plans = static_cast<TimeRegionWidget *>(m_scheduleMap["region"])->getPlans();

        emit signalSetParameter(VidiconProtocol::SCHEDULE, QVariant::fromValue(param));
        break;
    }
    case 1: {
        SDStorageParameter param;
        param.OperType = static_cast<QComboBox *>(m_SDStorageMap["Overwrite"])->currentIndex();
        param.RecordSelect = static_cast<QComboBox *>(m_SDStorageMap["Stream"])->currentIndex();
        param.RecordMode = static_cast<QComboBox *>(m_SDStorageMap["Record Type"])->currentIndex();
        param.RecordTime = static_cast<LineEdit *>(m_SDStorageMap["Record Time"])->text().toInt();

        emit signalSetParameter(VidiconProtocol::SDSTORAGE, QVariant::fromValue(param));
        break;
    }
    case 2: {
        SnapshotPlanParameter param;
        param.Enabled = static_cast<QCheckBox *>(m_snapshotMap["Enable"])->checkState() ? 1 : 0;
        param.SnapIntervalTime = static_cast<LineEdit *>(m_snapshotMap["Interval"])->text().toInt();
        param.weeksStateMap = static_cast<TimeRegionWidget *>(m_snapshotMap["region"])->getWeeksState();
        param.Plans = static_cast<TimeRegionWidget *>(m_snapshotMap["region"])->getPlans();

        emit signalSetParameter(VidiconProtocol::SNAPSHOT, QVariant::fromValue(param));
    }
    default:
        break;
    }
}

void RecordWidget::handleReceiveData(VidiconProtocol::Type type, QByteArray data)
{
    bool isOK = false;

    switch(type) {
    case VidiconProtocol::SCHEDULE: {
        RemoteRecordingPlan param;
        param.Plans = static_cast<TimeRegionWidget *>(m_scheduleMap["region"])->getPlans();

        isOK = ParseXML::getInstance()->parseScheduleParameter(&param, data);
        if (isOK) {
            static_cast<QCheckBox *>(m_scheduleMap["Enable"])->setChecked(param.Enabled ? true : false);

            int week = QDate::currentDate().dayOfWeek() == 7 ? 0 : QDate::currentDate().dayOfWeek();
            static_cast<QCheckBox *>(m_scheduleMap[QString("week %1").arg(week)])->setCheckState(Qt::Checked);
            for(int i=0;i <4; i++) {
                static_cast<QCheckBox *>(m_scheduleMap[QString("Time Period %1").arg(i)])->setChecked(param.Plans[week][i].PlanTimeEnabled ? true : false);
                static_cast<QTimeEdit *>(m_scheduleMap[QString("Time Period %1 start").arg(i)])->setTime(param.Plans[week][i].BeginTime);
                static_cast<QTimeEdit *>(m_scheduleMap[QString("Time Period %1 end").arg(i)])->setTime(param.Plans[week][i].EndTime);
                static_cast<QTimeEdit *>(m_scheduleMap[QString("Time Period %1 start").arg(i)])->setEnabled(param.Plans[week][i].PlanTimeEnabled ? true : false);
                static_cast<QTimeEdit *>(m_scheduleMap[QString("Time Period %1 end").arg(i)])->setEnabled(param.Plans[week][i].PlanTimeEnabled ? true : false);
            }
        }
        break;
    }
    case VidiconProtocol::SNAPSHOT: {
        SnapshotPlanParameter param;
        param.Plans = static_cast<TimeRegionWidget *>(m_snapshotMap["region"])->getPlans();

        isOK = ParseXML::getInstance()->parseSnapshotParameter(&param, data);
        if (isOK) {
            static_cast<QCheckBox *>(m_snapshotMap["Enable"])->setChecked(param.Enabled ? true : false);
            static_cast<LineEdit *>(m_snapshotMap["Interval"])->setText(QString::number(param.SnapIntervalTime));

            int week = QDate::currentDate().dayOfWeek() == 7 ? 0 : QDate::currentDate().dayOfWeek();
            static_cast<QCheckBox *>(m_snapshotMap[QString("week %1").arg(week)])->setCheckState(Qt::Checked);
            for(int i=0;i <4; i++) {
                static_cast<QCheckBox *>(m_snapshotMap[QString("Time Period %1").arg(i)])->setChecked(param.Plans[week][i].PlanTimeEnabled ? true : false);
                static_cast<QTimeEdit *>(m_snapshotMap[QString("Time Period %1 start").arg(i)])->setTime(param.Plans[week][i].BeginTime);
                static_cast<QTimeEdit *>(m_snapshotMap[QString("Time Period %1 end").arg(i)])->setTime(param.Plans[week][i].EndTime);
                static_cast<QTimeEdit *>(m_snapshotMap[QString("Time Period %1 start").arg(i)])->setEnabled(param.Plans[week][i].PlanTimeEnabled ? true : false);
                static_cast<QTimeEdit *>(m_snapshotMap[QString("Time Period %1 end").arg(i)])->setEnabled(param.Plans[week][i].PlanTimeEnabled ? true : false);
            }
        }
        break;
    }
    case VidiconProtocol::SDCARD: {
        SDCardStatus param;

        isOK = ParseXML::getInstance()->parseSDCardStatusParameter(&param, data);
        if (isOK) {
            static_cast<LineEdit *>(m_SDStorageMap["Used Space"])->setText(QString::number(param.UsedKByte));
            static_cast<LineEdit *>(m_SDStorageMap["Available Space"])->setText(QString::number(param.AvailableKByte));
            static_cast<LineEdit *>(m_SDStorageMap["Total Space"])->setText(QString::number(param.TotoalSizeKByte));
        }
        break;
    }
    case VidiconProtocol::SDSTORAGE: {
        SDStorageParameter param;

        isOK = ParseXML::getInstance()->parseSDStorageParameter(&param, data);
        if (isOK) {
            static_cast<QComboBox *>(m_SDStorageMap["Overwrite"])->setCurrentIndex(param.OperType);
            static_cast<QComboBox *>(m_SDStorageMap["Stream"])->setCurrentIndex(param.RecordSelect);
            static_cast<QComboBox *>(m_SDStorageMap["Record Type"])->setCurrentIndex(param.RecordSelect);
            static_cast<LineEdit *>(m_SDStorageMap["Record Time"])->setText(QString::number(param.RecordTime));
        }
        break;
    }
    default:
        return;
    }

    if (isOK)
        qDebug() << "#RecordWidget# handleReceiveData, ParameterType:" << type << "parse data success...";
    else
        qDebug() << "#RecordWidget# handleReceiveData, ParameterType:" << type << "parse data error...";
}

void RecordWidget::handleTimeSelect(int state)
{
    TimeRegionWidget *region = NULL;
    QMap<QString, QWidget *> map;
    switch (currentIndex()) {
    case 0:
        region = static_cast<TimeRegionWidget *>(m_scheduleMap.value("region"));
        map = m_scheduleMap;
        break;
    case 2:
        region = static_cast<TimeRegionWidget *>(m_snapshotMap.value("region"));
        map = m_snapshotMap;
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

void RecordWidget::handleSelectAllWeek()
{
    QMap<QString, QWidget *> map;
    switch (currentIndex()) {
    case 0:
        map = m_scheduleMap;
        break;
    case 2:
        map = m_snapshotMap;
        break;
    default:
        return;
    }
    for(int i=0; i<7; i++){
        QCheckBox *cb = static_cast<QCheckBox *>(map.value(QString("week %1").arg(i)));
        cb->setCheckState(Qt::Checked);
    }
}
