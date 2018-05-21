#include "alarmwidget.h"
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
#include "DrawSubWidget/motionwidget.h"
#include "DrawSubWidget/timeregionwidget.h"
#include "parsexml.h"
#include <QMessageBox>

AlarmWidget::AlarmWidget(QWidget *parent) :
    StackedWidget(parent)
{
    initRegionEditDialog();
    initMotionDetectionWidget();    
    initVideoBlindWidget();
    initAlarmWidget();
}

AlarmWidget::~AlarmWidget()
{
}

void AlarmWidget::initMotionDetectionWidget()
{
    QStringList list;
    m_motionDetectionWidget = new QWidget(this);

    QCheckBox *cb1 = new QCheckBox("启用移动侦测", m_motionDetectionWidget);
    m_motionDetectionMap.insert("Enable", cb1);
    QPushButton *btn1 = new QPushButton("设置侦测区域", m_motionDetectionWidget);
    connect(btn1, &QPushButton::clicked, this, [this](){
        m_regionEditDialog->exec();
    });

    QCheckBox *cb2 = new QCheckBox("报警输出", m_motionDetectionWidget);
    m_motionDetectionMap.insert("Alarm Output", cb2);
    QCheckBox *cb3 = new QCheckBox("录像输出", m_motionDetectionWidget);
    m_motionDetectionMap.insert("Record Video", cb3);
    QLabel *lbl1 = new QLabel("报警持续时间(Sec)(5-300):", m_motionDetectionWidget);
    LineEdit *lineEdit1 = new LineEdit(m_motionDetectionWidget);
    lineEdit1->setValidator(new QIntValidator(5, 300, m_motionDetectionWidget));
    m_motionDetectionMap.insert("Alarm Duration", lineEdit1);

    QLabel *lbl2 = new QLabel("预录时间(Sec)(1-10):");
    LineEdit *lineEdit2 = new LineEdit(m_motionDetectionWidget);
    lineEdit2->setValidator(new QIntValidator(1, 10, m_motionDetectionWidget));
    m_motionDetectionMap.insert("Pre-record time", lineEdit2);

    QLabel *lbl3 = new QLabel("录像持续时间:");
    QComboBox *comboBox1 = new QComboBox(m_motionDetectionWidget);
    list << "100" << "300" << "600";
    comboBox1->addItems(list);
    list.clear();
    m_motionDetectionMap.insert("Record time", comboBox1);

    TimeRegionWidget *region = new TimeRegionWidget(m_motionDetectionWidget);
    m_motionDetectionMap.insert("region", region);

    QCheckBox *cb4 = new QCheckBox("时间段1", m_motionDetectionWidget);
    cb4->setObjectName("Time Period 0");
    connect(cb4, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleTimeSelectState);
    connect(cb4, &QCheckBox::stateChanged, this, &AlarmWidget::handleTimeSelect);
    m_motionDetectionMap.insert("Time Period 0", cb4);
    QTimeEdit *time1 = new QTimeEdit(m_motionDetectionWidget);
    time1->setEnabled(false);
    time1->setObjectName("Time Period 0 start");
    connect(time1, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handleTimeChange);
    m_motionDetectionMap.insert("Time Period 0 start", time1);
    QLabel *lbl4 = new QLabel("-", m_motionDetectionWidget);
    QTimeEdit *time2 = new QTimeEdit(m_motionDetectionWidget);
    time2->setTime(QTime(23, 59, 59));
    time2->setEnabled(false);
    time2->setObjectName("Time Period 0 end");
    connect(time2, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handleTimeChange);
    m_motionDetectionMap.insert("Time Period 0 end", time2);

    QCheckBox *cb5 = new QCheckBox("时间段2", m_motionDetectionWidget);
    cb5->setObjectName("Time Period 1");
    connect(cb5, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleTimeSelectState);
    connect(cb5, &QCheckBox::stateChanged, this, &AlarmWidget::handleTimeSelect);
    m_motionDetectionMap.insert("Time Period 1", cb5);
    QTimeEdit *time3 = new QTimeEdit(m_motionDetectionWidget);
    time3->setEnabled(false);
    time3->setObjectName("Time Period 1 start");
    connect(time3, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handleTimeChange);
    m_motionDetectionMap.insert("Time Period 1 start", time3);
    QLabel *lbl5 = new QLabel("-", m_motionDetectionWidget);
    QTimeEdit *time4 = new QTimeEdit(m_motionDetectionWidget);
    time4->setTime(QTime(23, 59, 59));
    time4->setEnabled(false);
    time4->setObjectName("Time Period 1 end");
    connect(time4, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handleTimeChange);
    m_motionDetectionMap.insert("Time Period 1 end", time4);

    QCheckBox *cb6 = new QCheckBox("时间段3", m_motionDetectionWidget);
    cb6->setObjectName("Time Period 2");
    connect(cb6, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleTimeSelectState);
    connect(cb6, &QCheckBox::stateChanged, this, &AlarmWidget::handleTimeSelect);
    m_motionDetectionMap.insert("Time Period 2", cb6);
    QTimeEdit *time5 = new QTimeEdit(m_motionDetectionWidget);
    time5->setEnabled(false);
    time5->setObjectName("Time Period 2 start");
    connect(time5, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handleTimeChange);
    m_motionDetectionMap.insert("Time Period 2 start", time5);
    QLabel *lbl6 = new QLabel("-", m_motionDetectionWidget);
    QTimeEdit *time6 = new QTimeEdit(m_motionDetectionWidget);
    time6->setTime(QTime(23, 59, 59));
    time6->setEnabled(false);
    time6->setObjectName("Time Period 2 end");
    connect(time6, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handleTimeChange);
    m_motionDetectionMap.insert("Time Period 2 end", time6);

    QCheckBox *cb7 = new QCheckBox("时间段4", m_motionDetectionWidget);
    cb7->setObjectName("Time Period 3");
    connect(cb7, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleTimeSelectState);
    connect(cb7, &QCheckBox::stateChanged, this, &AlarmWidget::handleTimeSelect);
    m_motionDetectionMap.insert("Time Period 3", cb7);
    QTimeEdit *time7 = new QTimeEdit(m_motionDetectionWidget);
    time7->setEnabled(false);
    time7->setObjectName("Time Period 3 start");
    connect(time7, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handleTimeChange);
    m_motionDetectionMap.insert("Time Period 3 start", time7);
    QLabel *lbl7 = new QLabel("-", m_motionDetectionWidget);
    QTimeEdit *time8 = new QTimeEdit(m_motionDetectionWidget);
    time8->setTime(QTime(23, 59, 59));
    time8->setEnabled(false);
    time8->setObjectName("Time Period 3 end");
    connect(time8, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handleTimeChange);
    m_motionDetectionMap.insert("Time Period 3 end", time8);

    QPushButton *btn2 = new QPushButton("全选", m_motionDetectionWidget);
    connect(btn2, &QPushButton::clicked, this, &AlarmWidget::handleSelectAllWeek);

    QCheckBox *cb8 = new QCheckBox("周天", m_motionDetectionWidget);
    cb8->setObjectName("week 0");
    connect(cb8, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleWeekSelectState);
    m_motionDetectionMap.insert("week 0", cb8);

    QCheckBox *cb9 = new QCheckBox("周一", m_motionDetectionWidget);
    cb9->setObjectName("week 1");
    connect(cb9, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleWeekSelectState);
    m_motionDetectionMap.insert("week 1", cb9);

    QCheckBox *cb10 = new QCheckBox("周二", m_motionDetectionWidget);
    cb10->setObjectName("week 2");
    connect(cb10, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleWeekSelectState);
    m_motionDetectionMap.insert("week 2", cb10);

    QCheckBox *cb11 = new QCheckBox("周三", m_motionDetectionWidget);
    cb11->setObjectName("week 3");
    connect(cb11, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleWeekSelectState);
    m_motionDetectionMap.insert("week 3", cb11);

    QCheckBox *cb12 = new QCheckBox("周四", m_motionDetectionWidget);
    cb12->setObjectName("week 4");
    connect(cb12, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleWeekSelectState);
    m_motionDetectionMap.insert("week 4", cb12);

    QCheckBox *cb13 = new QCheckBox("周五", m_motionDetectionWidget);
    cb13->setObjectName("week 5");
    connect(cb13, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleWeekSelectState);
    m_motionDetectionMap.insert("week 5", cb13);

    QCheckBox *cb14 = new QCheckBox("周六", m_motionDetectionWidget);
    cb14->setObjectName("week 6");
    connect(cb14, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleWeekSelectState);
    m_motionDetectionMap.insert("week 6", cb14);

    QPushButton *btn3 = new QPushButton("保存", m_motionDetectionWidget);
    connect(btn3, &QPushButton::clicked, this, &AlarmWidget::handlePrepareData);

    QGridLayout *layout1 = new QGridLayout;
    layout1->addWidget(cb1,      0, 0, 1, 2);
    layout1->addWidget(btn1,       0, 2, 1, 2);

    layout1->addWidget(cb2,      1, 0, 1, 2);
    layout1->addWidget(cb3,      1, 2, 1, 2);
    layout1->addWidget(lbl1,       1, 4, 1, 2);
    layout1->addWidget(lineEdit1,  1, 6, 1, 2);

    layout1->addWidget(lbl2,       2, 0, 1, 2);
    layout1->addWidget(lineEdit2,  2, 2, 1, 2);
    layout1->addWidget(lbl3,       2, 4, 1, 2);
    layout1->addWidget(comboBox1,  2, 6, 1, 2);

    layout1->addWidget(region,     3, 0, 7, 6);

    QHBoxLayout *layout4 = new QHBoxLayout;
    layout4->addWidget(cb4);
    layout4->addWidget(time1);
    layout4->addWidget(lbl4);
    layout4->addWidget(time2);
    layout1->addLayout(layout4, 10, 0, 1, 3);

    QHBoxLayout *layout5 = new QHBoxLayout;
    layout5->addWidget(cb5);
    layout5->addWidget(time3);
    layout5->addWidget(lbl5);
    layout5->addWidget(time4);
    layout1->addLayout(layout5, 11, 0, 1, 3);

    QHBoxLayout *layout6 = new QHBoxLayout;
    layout6->addWidget(cb6);
    layout6->addWidget(time5);
    layout6->addWidget(lbl6);
    layout6->addWidget(time6);
    layout1->addLayout(layout6, 12, 0, 1, 3);

    QHBoxLayout *layout7 = new QHBoxLayout;
    layout7->addWidget(cb7);
    layout7->addWidget(time7);
    layout7->addWidget(lbl7);
    layout7->addWidget(time8);
    layout1->addLayout(layout7, 13, 0, 1, 3);

    layout1->addWidget(btn2,       14, 0, 1, 1);
    layout1->addWidget(cb8,        14, 1, 1, 1);
    layout1->addWidget(cb9,        14, 2, 1, 1);
    layout1->addWidget(cb10,        14, 3, 1, 1);
    layout1->addWidget(cb11,        14, 4, 1, 1);
    layout1->addWidget(cb12,        14, 5, 1, 1);
    layout1->addWidget(cb13,        14, 6, 1, 1);
    layout1->addWidget(cb14,        14, 7, 1, 1);

    layout1->addWidget(btn3,       15, 0, 1, 8, Qt::AlignCenter);

    setAlignment(m_motionDetectionWidget, layout1, Qt::AlignTop | Qt::AlignHCenter);
    addWidget(m_motionDetectionWidget);
}

void AlarmWidget::initVideoBlindWidget()
{
    QStringList list;
    m_videoBlindWidget = new QWidget(this);

    QCheckBox *cb1 = new QCheckBox("启用遮挡报警", m_videoBlindWidget);
    m_videoBlindMap.insert("Enable", cb1);
    QCheckBox *cb2 = new QCheckBox("报警输出", m_videoBlindWidget);
    m_videoBlindMap.insert("Output", cb2);
    QCheckBox *cb3 = new QCheckBox("录像输出", m_videoBlindWidget);
    m_videoBlindMap.insert("Record", cb3);

    QLabel *lbl1 = new QLabel("录像持续时间(Sec)(5-300):");
    LineEdit *lineEdit1 = new LineEdit(m_videoBlindWidget);
    lineEdit1->setValidator(new QIntValidator(5, 300, m_videoBlindWidget));
    m_videoBlindMap.insert("Alarm time", lineEdit1);
    QLabel *lbl2 = new QLabel("侦测灵敏度：");
    QComboBox *comboBox1 = new QComboBox(m_videoBlindWidget);
    list << "低" << "中" << "高";
    comboBox1->addItems(list);
    list.clear();
    m_videoBlindMap.insert("Sensitivity", comboBox1);

    TimeRegionWidget *region = new TimeRegionWidget(m_videoBlindWidget);
    m_videoBlindMap.insert("region", region);

    QCheckBox *cb4 = new QCheckBox("时间段1", m_videoBlindWidget);
    cb4->setObjectName("Time Period 0");
    connect(cb4, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleTimeSelectState);
    connect(cb4, &QCheckBox::stateChanged, this, &AlarmWidget::handleTimeSelect);
    m_videoBlindMap.insert("Time Period 0", cb4);
    QTimeEdit *time1 = new QTimeEdit(m_videoBlindWidget);
    time1->setEnabled(false);
    time1->setObjectName("Time Period 0 start");
    connect(time1, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handleTimeChange);
    m_videoBlindMap.insert("Time Period 0 start", time1);
    QLabel *lbl3 = new QLabel("-", m_videoBlindWidget);
    QTimeEdit *time2 = new QTimeEdit(m_videoBlindWidget);
    time2->setTime(QTime(23, 59, 59));
    time2->setEnabled(false);
    time2->setObjectName("Time Period 0 end");
    connect(time2, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handleTimeChange);
    m_videoBlindMap.insert("Time Period 0 end", time2);

    QCheckBox *cb5 = new QCheckBox("时间段2", m_videoBlindWidget);
    cb5->setObjectName("Time Period 1");
    connect(cb5, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleTimeSelectState);
    connect(cb5, &QCheckBox::stateChanged, this, &AlarmWidget::handleTimeSelect);
    m_videoBlindMap.insert("Time Period 1", cb5);
    QTimeEdit *time3 = new QTimeEdit(m_videoBlindWidget);
    time3->setEnabled(false);
    time3->setObjectName("Time Period 1 start");
    connect(time3, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handleTimeChange);
    m_videoBlindMap.insert("Time Period 1 start", time3);
    QLabel *lbl4 = new QLabel("-", m_videoBlindWidget);
    QTimeEdit *time4 = new QTimeEdit(m_videoBlindWidget);
    time4->setTime(QTime(23, 59, 59));
    time4->setEnabled(false);
    time4->setObjectName("Time Period 1 end");
    connect(time4, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handleTimeChange);
    m_videoBlindMap.insert("Time Period 1 end", time4);

    QCheckBox *cb6 = new QCheckBox("时间段3", m_videoBlindWidget);
    cb6->setObjectName("Time Period 2");
    connect(cb6, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleTimeSelectState);
    connect(cb6, &QCheckBox::stateChanged, this, &AlarmWidget::handleTimeSelect);
    m_videoBlindMap.insert("Time Period 2", cb6);
    QTimeEdit *time5 = new QTimeEdit(m_videoBlindWidget);
    time5->setEnabled(false);
    time5->setObjectName("Time Period 2 start");
    connect(time5, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handleTimeChange);
    m_videoBlindMap.insert("Time Period 2 start", time5);
    QLabel *lbl5 = new QLabel("-", m_videoBlindWidget);
    QTimeEdit *time6 = new QTimeEdit(m_videoBlindWidget);
    time6->setTime(QTime(23, 59, 59));
    time6->setEnabled(false);
    time6->setObjectName("Time Period 2 end");
    connect(time6, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handleTimeChange);
    m_videoBlindMap.insert("Time Period 2 end", time6);

    QCheckBox *cb7 = new QCheckBox("时间段4", m_videoBlindWidget);
    cb7->setObjectName("Time Period 3");
    connect(cb7, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleTimeSelectState);
    connect(cb7, &QCheckBox::stateChanged, this, &AlarmWidget::handleTimeSelect);
    m_videoBlindMap.insert("Time Period 3", cb7);
    QTimeEdit *time7 = new QTimeEdit(m_videoBlindWidget);
    time7->setEnabled(false);
    time7->setObjectName("Time Period 3 start");
    connect(time7, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handleTimeChange);
    m_videoBlindMap.insert("Time Period 3 start", time7);
    QLabel *lbl6 = new QLabel("-", m_videoBlindWidget);
    QTimeEdit *time8 = new QTimeEdit(m_videoBlindWidget);
    time8->setTime(QTime(23, 59, 59));
    time8->setEnabled(false);
    time8->setObjectName("Time Period 3 end");
    connect(time8, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handleTimeChange);
    m_videoBlindMap.insert("Time Period 3 end", time8);

    QPushButton *btn2 = new QPushButton("全选", m_videoBlindWidget);
    connect(btn2, &QPushButton::clicked, this, &AlarmWidget::handleSelectAllWeek);

    QCheckBox *cb8 = new QCheckBox("周天", m_videoBlindWidget);
    cb8->setObjectName("week 0");
    connect(cb8, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleWeekSelectState);
    m_videoBlindMap.insert("week 0", cb8);

    QCheckBox *cb9 = new QCheckBox("周一", m_videoBlindWidget);
    cb9->setObjectName("week 1");
    connect(cb9, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleWeekSelectState);
    m_videoBlindMap.insert("week 1", cb9);

    QCheckBox *cb10 = new QCheckBox("周二", m_videoBlindWidget);
    cb10->setObjectName("week 2");
    connect(cb10, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleWeekSelectState);
    m_videoBlindMap.insert("week 2", cb10);

    QCheckBox *cb11 = new QCheckBox("周三", m_videoBlindWidget);
    cb11->setObjectName("week 3");
    connect(cb11, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleWeekSelectState);
    m_videoBlindMap.insert("week 3", cb11);

    QCheckBox *cb12 = new QCheckBox("周四", m_videoBlindWidget);
    cb12->setObjectName("week 4");
    connect(cb12, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleWeekSelectState);
    m_videoBlindMap.insert("week 4", cb12);

    QCheckBox *cb13 = new QCheckBox("周五", m_videoBlindWidget);
    cb13->setObjectName("week 5");
    connect(cb13, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleWeekSelectState);
    m_videoBlindMap.insert("week 5", cb13);

    QCheckBox *cb14 = new QCheckBox("周六", m_videoBlindWidget);
    cb14->setObjectName("week 6");
    connect(cb14, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleWeekSelectState);
    m_videoBlindMap.insert("week 6", cb14);

    QPushButton *btn3 = new QPushButton("保存", m_videoBlindWidget);
    connect(btn3, &QPushButton::clicked, this, &AlarmWidget::handlePrepareData);

    QGridLayout *layout1 = new QGridLayout;
    layout1->addWidget(cb1,       0, 0, 1, 2);
    layout1->addWidget(cb2,       0, 2, 1, 2);
    layout1->addWidget(cb3,       0, 4, 1, 2);

    layout1->addWidget(lbl1,       1, 0, 1, 2);
    layout1->addWidget(lineEdit1,  1, 2, 1, 2);
    layout1->addWidget(lbl2,       1, 4, 1, 2);
    layout1->addWidget(comboBox1,  1, 6, 1, 2);

    layout1->addWidget(region,     2, 0, 7, 6);

    QHBoxLayout *layout4 = new QHBoxLayout;
    layout4->addWidget(cb4);
    layout4->addWidget(time1);
    layout4->addWidget(lbl3);
    layout4->addWidget(time2);
    layout1->addLayout(layout4, 9, 0, 1, 3);

    QHBoxLayout *layout5 = new QHBoxLayout;
    layout5->addWidget(cb5);
    layout5->addWidget(time3);
    layout5->addWidget(lbl4);
    layout5->addWidget(time4);
    layout1->addLayout(layout5, 10, 0, 1, 3);

    QHBoxLayout *layout6 = new QHBoxLayout;
    layout6->addWidget(cb6);
    layout6->addWidget(time5);
    layout6->addWidget(lbl5);
    layout6->addWidget(time6);
    layout1->addLayout(layout6, 11, 0, 1, 3);

    QHBoxLayout *layout7 = new QHBoxLayout;
    layout7->addWidget(cb7);
    layout7->addWidget(time7);
    layout7->addWidget(lbl6);
    layout7->addWidget(time8);
    layout1->addLayout(layout7, 12, 0, 1, 3);

    layout1->addWidget(btn2,    13, 0, 1, 1);
    layout1->addWidget(cb8,     13, 1, 1, 1);
    layout1->addWidget(cb9,     13, 2, 1, 1);
    layout1->addWidget(cb10,    13, 3, 1, 1);
    layout1->addWidget(cb11,    13, 4, 1, 1);
    layout1->addWidget(cb12,    13, 5, 1, 1);
    layout1->addWidget(cb13,    13, 6, 1, 1);
    layout1->addWidget(cb14,    13, 7, 1, 1);

    layout1->addWidget(btn3,    14, 0, 1, 8, Qt::AlignCenter);

    setAlignment(m_videoBlindWidget, layout1, Qt::AlignTop | Qt::AlignHCenter);
    addWidget(m_videoBlindWidget);
}

void AlarmWidget::initAlarmWidget()
{
    QStringList list;
    m_alarmWidget = new QWidget(this);

    QCheckBox *cb1 = new QCheckBox("启用探头报警", m_alarmWidget);
    m_alarmMap.insert("Enable", cb1);
    QCheckBox *cb2 = new QCheckBox("报警输出", m_alarmWidget);
    m_alarmMap.insert("Output", cb2);
    QCheckBox *cb3 = new QCheckBox("录像输出", m_alarmWidget);
    m_alarmMap.insert("Record", cb3);

    QLabel *lbl1 = new QLabel("报警持续时间(Sec)(5-300):");
    LineEdit *lineEdit1 = new LineEdit(m_alarmWidget);
    lineEdit1->setValidator(new QIntValidator(5, 300, m_alarmWidget));
    m_alarmMap.insert("Alarm time", lineEdit1);
    QLabel *lbl2 = new QLabel("探头类型：");
    QComboBox *comboBox1 = new QComboBox(m_alarmWidget);
    list << "常闭" << "常开";
    comboBox1->addItems(list);
    list.clear();
    m_alarmMap.insert("Type", comboBox1);

    TimeRegionWidget *region = new TimeRegionWidget(m_alarmWidget);
    m_alarmMap.insert("region", region);

    QCheckBox *cb4 = new QCheckBox("时间段1", m_alarmWidget);
    cb4->setObjectName("Time Period 0");
    connect(cb4, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleTimeSelectState);
    connect(cb4, &QCheckBox::stateChanged, this, &AlarmWidget::handleTimeSelect);
    m_alarmMap.insert("Time Period 0", cb4);
    QTimeEdit *time1 = new QTimeEdit(m_alarmWidget);
    time1->setEnabled(false);
    time1->setObjectName("Time Period 0 start");
    connect(time1, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handleTimeChange);
    m_alarmMap.insert("Time Period 0 start", time1);
    QLabel *lbl3 = new QLabel("-", m_alarmWidget);
    QTimeEdit *time2 = new QTimeEdit(m_alarmWidget);
    time2->setEnabled(false);
    time2->setObjectName("Time Period 0 end");
    connect(time2, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handleTimeChange);
    m_alarmMap.insert("Time Period 0 end", time2);

    QCheckBox *cb5 = new QCheckBox("时间段2", m_alarmWidget);
    cb5->setObjectName("Time Period 1");
    connect(cb5, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleTimeSelectState);
    connect(cb5, &QCheckBox::stateChanged, this, &AlarmWidget::handleTimeSelect);
    m_alarmMap.insert("Time Period 1", cb5);
    QTimeEdit *time3 = new QTimeEdit(m_alarmWidget);
    time3->setEnabled(false);
    time3->setObjectName("Time Period 1 start");
    connect(time3, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handleTimeChange);
    m_alarmMap.insert("Time Period 1 start", time3);
    QLabel *lbl4 = new QLabel("-", m_alarmWidget);
    QTimeEdit *time4 = new QTimeEdit(m_alarmWidget);
    time4->setEnabled(false);
    time4->setObjectName("Time Period 1 end");
    connect(time4, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handleTimeChange);
    m_alarmMap.insert("Time Period 1 end", time4);

    QCheckBox *cb6 = new QCheckBox("时间段3", m_alarmWidget);
    cb6->setObjectName("Time Period 2");
    connect(cb6, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleTimeSelectState);
    connect(cb6, &QCheckBox::stateChanged, this, &AlarmWidget::handleTimeSelect);
    m_alarmMap.insert("Time Period 2", cb6);
    QTimeEdit *time5 = new QTimeEdit(m_alarmWidget);
    time5->setEnabled(false);
    time5->setObjectName("Time Period 2 start");
    connect(time5, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handleTimeChange);
    m_alarmMap.insert("Time Period 2 start", time5);
    QLabel *lbl5 = new QLabel("-", m_alarmWidget);
    QTimeEdit *time6 = new QTimeEdit(m_alarmWidget);
    time6->setEnabled(false);
    time6->setObjectName("Time Period 2 end");
    connect(time6, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handleTimeChange);
    m_alarmMap.insert("Time Period 2 end", time6);

    QCheckBox *cb7 = new QCheckBox("时间段4", m_alarmWidget);
    cb7->setObjectName("Time Period 3");
    connect(cb7, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleTimeSelectState);
    connect(cb7, &QCheckBox::stateChanged, this, &AlarmWidget::handleTimeSelect);
    m_alarmMap.insert("Time Period 3", cb7);
    QTimeEdit *time7 = new QTimeEdit(m_alarmWidget);
    time7->setEnabled(false);
    time7->setObjectName("Time Period 3 start");
    connect(time7, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handleTimeChange);
    m_alarmMap.insert("Time Period 3 start", time7);
    QLabel *lbl6 = new QLabel("-", m_alarmWidget);
    QTimeEdit *time8 = new QTimeEdit(m_alarmWidget);
    time8->setEnabled(false);
    time8->setObjectName("Time Period 3 end");
    connect(time8, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handleTimeChange);
    m_alarmMap.insert("Time Period 3 end", time8);

    QPushButton *btn2 = new QPushButton("全选", m_alarmWidget);
    connect(btn2, &QPushButton::clicked, this, &AlarmWidget::handleSelectAllWeek);

    QCheckBox *cb8 = new QCheckBox("周天", m_alarmWidget);
    cb8->setObjectName("week 0");
    connect(cb8, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleWeekSelectState);
    m_alarmMap.insert("week 0", cb8);

    QCheckBox *cb9 = new QCheckBox("周一", m_alarmWidget);
    cb9->setObjectName("week 1");
    connect(cb9, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleWeekSelectState);
    m_alarmMap.insert("week 1", cb9);

    QCheckBox *cb10 = new QCheckBox("周二", m_alarmWidget);
    cb10->setObjectName("week 2");
    connect(cb10, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleWeekSelectState);
    m_alarmMap.insert("week 2", cb10);

    QCheckBox *cb11 = new QCheckBox("周三", m_alarmWidget);
    cb11->setObjectName("week 3");
    connect(cb11, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleWeekSelectState);
    m_alarmMap.insert("week 3", cb11);

    QCheckBox *cb12 = new QCheckBox("周四", m_alarmWidget);
    cb12->setObjectName("week 4");
    connect(cb12, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleWeekSelectState);
    m_alarmMap.insert("week 4", cb12);

    QCheckBox *cb13 = new QCheckBox("周五", m_alarmWidget);
    cb13->setObjectName("week 5");
    connect(cb13, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleWeekSelectState);
    m_alarmMap.insert("week 5", cb13);

    QCheckBox *cb14 = new QCheckBox("周六", m_alarmWidget);
    cb14->setObjectName("week 6");
    connect(cb14, &QCheckBox::stateChanged, region, &TimeRegionWidget::handleWeekSelectState);
    m_alarmMap.insert("week 6", cb14);

    QPushButton *btn3 = new QPushButton("保存", m_alarmWidget);
    connect(btn3, &QPushButton::clicked, this, &AlarmWidget::handlePrepareData);

    QGridLayout *layout1 = new QGridLayout;
    layout1->addWidget(cb1,       0, 0, 1, 2);
    layout1->addWidget(cb2,       0, 2, 1, 2);
    layout1->addWidget(cb3,       0, 4, 1, 2);

    layout1->addWidget(lbl1,       1, 0, 1, 2);
    layout1->addWidget(lineEdit1,  1, 2, 1, 2);
    layout1->addWidget(lbl2,       1, 4, 1, 2);
    layout1->addWidget(comboBox1,  1, 6, 1, 2);

    layout1->addWidget(region,     2, 0, 7, 6);

    QHBoxLayout *layout4 = new QHBoxLayout;
    layout4->addWidget(cb4);
    layout4->addWidget(time1);
    layout4->addWidget(lbl3);
    layout4->addWidget(time2);
    layout1->addLayout(layout4, 9, 0, 1, 3);

    QHBoxLayout *layout5 = new QHBoxLayout;
    layout5->addWidget(cb5);
    layout5->addWidget(time3);
    layout5->addWidget(lbl4);
    layout5->addWidget(time4);
    layout1->addLayout(layout5, 10, 0, 1, 3);

    QHBoxLayout *layout6 = new QHBoxLayout;
    layout6->addWidget(cb6);
    layout6->addWidget(time5);
    layout6->addWidget(lbl5);
    layout6->addWidget(time6);
    layout1->addLayout(layout6, 11, 0, 1, 3);

    QHBoxLayout *layout7 = new QHBoxLayout;
    layout7->addWidget(cb7);
    layout7->addWidget(time7);
    layout7->addWidget(lbl6);
    layout7->addWidget(time8);
    layout1->addLayout(layout7, 12, 0, 1, 3);

    layout1->addWidget(btn2,    13, 0, 1, 1);
    layout1->addWidget(cb8,     13, 1, 1, 1);
    layout1->addWidget(cb9,     13, 2, 1, 1);
    layout1->addWidget(cb10,    13, 3, 1, 1);
    layout1->addWidget(cb11,    13, 4, 1, 1);
    layout1->addWidget(cb12,    13, 5, 1, 1);
    layout1->addWidget(cb13,    13, 6, 1, 1);
    layout1->addWidget(cb14,    13, 7, 1, 1);

    layout1->addWidget(btn3,    14, 0, 1, 8, Qt::AlignCenter);

    setAlignment(m_alarmWidget, layout1, Qt::AlignTop | Qt::AlignHCenter);
    addWidget(m_alarmWidget);
}

void AlarmWidget::initRegionEditDialog()
{
    QStringList list;
    m_regionEditDialog = new QDialog(this);

    MotionWidget *region = new MotionWidget(m_regionEditDialog);
    region->setFixedSize(440, 360);
    m_regionEditMap.insert("region", region);

    QLabel *lbl1 = new QLabel("侦测灵敏度", m_regionEditDialog);
    QComboBox *comboBox1 = new QComboBox(m_regionEditDialog);
    list << "低" << "中" << "高";
    comboBox1->addItems(list);
    list.clear();
    m_regionEditMap.insert("Sensitivity", comboBox1);

    QLabel *lbl2 = new QLabel("触发阈值", m_regionEditDialog);
    QSlider *slider1 = new QSlider(Qt::Horizontal, m_regionEditDialog);
    slider1->setRange(0, 100);
    QLabel *lbl3 = new QLabel(m_regionEditDialog);
    m_regionEditMap.insert("Threshold", slider1);
    m_regionEditMap.insert("ThresholdTag", lbl3);
    connect(slider1, &QSlider::valueChanged, this, [this](int value){
        static_cast<QLabel *>(m_regionEditMap["ThresholdTag"])->setText(QString::number(value));
    });

    QPushButton *btn1 = new QPushButton("全屏", m_regionEditDialog);
    connect(btn1, &QPushButton::clicked, region, &MotionWidget::handleFullScreen);
    QPushButton *btn2 = new QPushButton("清屏", m_regionEditDialog);
    connect(btn2, &QPushButton::clicked, region, &MotionWidget::handleCleanScreen);
    QPushButton *btn3 = new QPushButton("保存", m_regionEditDialog);
    btn3->setObjectName("MotionRegion");
    connect(btn3, &QPushButton::clicked, this, &AlarmWidget::handlePrepareData);

    QGridLayout *layout1 = new QGridLayout;
    layout1->addWidget(region,      0, 0, 8, 10);

    layout1->addWidget(lbl1,        0, 11, 1, 1);
    layout1->addWidget(comboBox1,   0, 12, 1, 1);

    layout1->addWidget(lbl2,        1, 11, 1, 1);
    layout1->addWidget(slider1,    1, 12, 1, 1);
    layout1->addWidget(lbl3,        1, 13, 1, 3);

    layout1->addWidget(btn1,      2, 11, 1, 1, Qt::AlignCenter);
    layout1->addWidget(btn2,      2, 12, 1, 1, Qt::AlignCenter);

    layout1->addWidget(btn3,      3, 11, 1, 2, Qt::AlignCenter);

    setAlignment(m_regionEditDialog, layout1, Qt::AlignTop | Qt::AlignHCenter);
}

void AlarmWidget::setCurrentIndex(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    switch(index.row()){
    case 0: {
        emit signalGetParameter(VidiconProtocol::MOTION);
        break;
    }
    case 1: {
        emit signalGetParameter(VidiconProtocol::BLIND);
        break;
    }
    case 2: {
        emit signalGetParameter(VidiconProtocol::SENSOR);
        break;
    }
    default:
        break;
    }

    StackedWidget::setCurrentIndex(index.row());
}

void AlarmWidget::handlePrepareData()
{
    switch(currentIndex()) {
    case 0: {
        MotionDetectionParameter param;
        param.Enabled = static_cast<QCheckBox *>(m_motionDetectionMap["Enable"])->checkState() == Qt::Checked ? 1 : 0;
        param.AlarmOutput = static_cast<QCheckBox *>(m_motionDetectionMap["Alarm Output"])->checkState() == Qt::Checked ? 1 : 0;
        param.VideoOutput = static_cast<QCheckBox *>(m_motionDetectionMap["Record Video"])->checkState() == Qt::Checked ? 1 : 0;
        param.AlarmDuration = static_cast<LineEdit *>(m_motionDetectionMap["Alarm Duration"])->text().toInt();
        param.PreRecTime = static_cast<LineEdit *>(m_motionDetectionMap["Pre-record time"])->text().toInt();
        param.DelayRecTime = static_cast<QComboBox *>(m_motionDetectionMap["Record time"])->currentText().toInt();
        param.weeksStateMap = static_cast<TimeRegionWidget *>(m_motionDetectionMap["region"])->getWeeksState();
        param.Plans = static_cast<TimeRegionWidget *>(m_motionDetectionMap["region"])->getPlans();
        if(sender()->objectName() == "MotionRegion") {
            char **map = static_cast<MotionWidget *>(m_regionEditMap["region"])->getMotionRegionMap();
            for(int i=0; i<REGIONROW; i++) {
                for(int j=0; j<REGIONCOLUMN; j++) {
                    param.AreaMask.append(QString::number(map[i][j]));
                }
            }
            param.Sensitivity = static_cast<QComboBox *>(m_regionEditMap["Sensitivity"])->currentIndex();
            param.AlarmThreshold = static_cast<QSlider *>(m_regionEditMap["Threshold"])->value();
            param.onlyRegion = true;
        }else {
            param.onlyRegion = false;
        }
        emit signalSetParameter(VidiconProtocol::MOTION, QVariant::fromValue(param));
        break;
    }
    case 1: {
        VideoBlindAlarmParameter param;
        param.Enabled = static_cast<QCheckBox *>(m_videoBlindMap["Enable"])->checkState() == Qt::Checked ? 1 : 0;
        param.AlarmOutput = static_cast<QCheckBox *>(m_videoBlindMap["Output"])->checkState() == Qt::Checked ? 1 : 0;
        param.VideoOutput = static_cast<QCheckBox *>(m_videoBlindMap["Record"])->checkState() == Qt::Checked ? 1 : 0;
        param.AlarmDuration = static_cast<LineEdit *>(m_videoBlindMap["Alarm time"])->text().toInt();
        param.Sensitivity = static_cast<QComboBox *>(m_videoBlindMap["Sensitivity"])->currentIndex();
        param.weeksStateMap = static_cast<TimeRegionWidget *>(m_videoBlindMap["region"])->getWeeksState();
        param.Plans = static_cast<TimeRegionWidget *>(m_videoBlindMap["region"])->getPlans();
        emit signalSetParameter(VidiconProtocol::BLIND, QVariant::fromValue(param));
        break;
    }
    case 2: {
        SensorAlarmParameter param;
        param.Enabled = static_cast<QCheckBox *>(m_alarmMap["Enable"])->checkState() == Qt::Checked ? 1 : 0;
        param.AlarmOutput = static_cast<QCheckBox *>(m_alarmMap["Output"])->checkState() == Qt::Checked ? 1 : 0;
        param.VideoOutput = static_cast<QCheckBox *>(m_alarmMap["Record"])->checkState() == Qt::Checked ? 1 : 0;
        param.AlarmDuration = static_cast<LineEdit *>(m_alarmMap["Alarm time"])->text().toInt();
        param.SensorType = static_cast<QComboBox *>(m_alarmMap["Type"])->currentIndex();
        param.weeksStateMap = static_cast<TimeRegionWidget *>(m_alarmMap["region"])->getWeeksState();
        param.Plans = static_cast<TimeRegionWidget *>(m_alarmMap["region"])->getPlans();
        emit signalSetParameter(VidiconProtocol::SENSOR, QVariant::fromValue(param));
        break;
    }
    default:
        break;
    }
}

void AlarmWidget::handleReceiveData(VidiconProtocol::Type type, QByteArray data)
{    
    bool isOK = false;

    switch(type) {
    case VidiconProtocol::MOTION: {
        MotionDetectionParameter param;
        param.Plans = static_cast<TimeRegionWidget *>(m_motionDetectionMap["region"])->getPlans();

        isOK = ParseXML::getInstance()->parseMotionParameter(&param, data);
        if (isOK) {
            static_cast<QCheckBox *>(m_motionDetectionMap["Enable"])->setChecked(param.Enabled ? true : false);
            static_cast<QCheckBox *>(m_motionDetectionMap["Alarm Output"])->setChecked(param.AlarmOutput ? true : false);
            static_cast<QCheckBox *>(m_motionDetectionMap["Record Video"])->setChecked(param.VideoOutput ? true : false);
            static_cast<LineEdit *>(m_motionDetectionMap["Alarm Duration"])->setText(QString::number(param.AlarmDuration));
            static_cast<LineEdit *>(m_motionDetectionMap["Pre-record time"])->setText(QString::number(param.PreRecTime));
            static_cast<QComboBox *>(m_motionDetectionMap["Record time"])->setCurrentText(QString::number(param.DelayRecTime));

            static_cast<QComboBox *>(m_regionEditMap["Sensitivity"])->setCurrentIndex(param.Sensitivity);
            static_cast<QSlider *>(m_regionEditMap["Threshold"])->setValue(param.AlarmThreshold);
            static_cast<TimeRegionWidget *>(m_motionDetectionMap["region"])->update();
            int week = QDate::currentDate().dayOfWeek() == 7 ? 0 : QDate::currentDate().dayOfWeek();
            static_cast<QCheckBox *>(m_motionDetectionMap[QString("week %1").arg(week)])->setCheckState(Qt::Checked);

            for(int i=0;i <4; i++) {
                static_cast<QCheckBox *>(m_motionDetectionMap[QString("Time Period %1").arg(i)])->setChecked(param.Plans[week][i].PlanTimeEnabled ? true : false);
                static_cast<QTimeEdit *>(m_motionDetectionMap[QString("Time Period %1 start").arg(i)])->setTime(param.Plans[week][i].BeginTime);
                static_cast<QTimeEdit *>(m_motionDetectionMap[QString("Time Period %1 end").arg(i)])->setTime(param.Plans[week][i].EndTime);
                static_cast<QTimeEdit *>(m_motionDetectionMap[QString("Time Period %1 start").arg(i)])->setEnabled(param.Plans[week][i].PlanTimeEnabled ? true : false);
                static_cast<QTimeEdit *>(m_motionDetectionMap[QString("Time Period %1 end").arg(i)])->setEnabled(param.Plans[week][i].PlanTimeEnabled ? true : false);
            }
            char **map = static_cast<MotionWidget *>(m_regionEditMap["region"])->getMotionRegionMap();
            for(int i=0; i<REGIONROW; i++) {
                for(int j=0; j<REGIONCOLUMN; j++) {
                    map[i][j] = param.AreaMask.mid(i * REGIONCOLUMN + j, 1).toInt();
                }
            }
            static_cast<MotionWidget *>(m_regionEditMap["region"])->update();
        }
        break;
    }
    case VidiconProtocol::BLIND: {
        VideoBlindAlarmParameter param;
        param.Plans = static_cast<TimeRegionWidget *>(m_videoBlindMap["region"])->getPlans();

        isOK = ParseXML::getInstance()->parseBlindParameter(&param, data);
        if (isOK) {
            static_cast<QCheckBox *>(m_videoBlindMap["Enable"])->setChecked(param.Enabled ? true : false);
            static_cast<QCheckBox *>(m_videoBlindMap["Output"])->setChecked(param.AlarmOutput ? true : false);
            static_cast<QCheckBox *>(m_videoBlindMap["Record"])->setChecked(param.VideoOutput ? true : false);
            static_cast<LineEdit *>(m_videoBlindMap["Alarm time"])->setText(QString::number(param.AlarmDuration));
            static_cast<QComboBox *>(m_videoBlindMap["Sensitivity"])->setCurrentIndex(param.Sensitivity);

            int week = QDate::currentDate().dayOfWeek() == 7 ? 0 : QDate::currentDate().dayOfWeek();
            static_cast<QCheckBox *>(m_videoBlindMap[QString("week %1").arg(week)])->setCheckState(Qt::Checked);

            for(int i=0;i <4; i++) {
                static_cast<QCheckBox *>(m_videoBlindMap[QString("Time Period %1").arg(i)])->setChecked(param.Plans[week][i].PlanTimeEnabled ? true : false);
                static_cast<QTimeEdit *>(m_videoBlindMap[QString("Time Period %1 start").arg(i)])->setTime(param.Plans[week][i].BeginTime);
                static_cast<QTimeEdit *>(m_videoBlindMap[QString("Time Period %1 end").arg(i)])->setTime(param.Plans[week][i].EndTime);
                static_cast<QTimeEdit *>(m_videoBlindMap[QString("Time Period %1 start").arg(i)])->setEnabled(param.Plans[week][i].PlanTimeEnabled ? true : false);
                static_cast<QTimeEdit *>(m_videoBlindMap[QString("Time Period %1 end").arg(i)])->setEnabled(param.Plans[week][i].PlanTimeEnabled ? true : false);
            }
        }
        break;
    }
    case VidiconProtocol::SENSOR: {
        SensorAlarmParameter param;
        param.Plans = static_cast<TimeRegionWidget *>(m_alarmMap["region"])->getPlans();

        isOK = ParseXML::getInstance()->parseSensorParameter(&param, data);
        if (isOK) {
            static_cast<QCheckBox *>(m_alarmMap["Enable"])->setChecked(param.Enabled ? true : false);
            static_cast<QCheckBox *>(m_alarmMap["Output"])->setChecked(param.AlarmOutput ? true : false);
            static_cast<QCheckBox *>(m_alarmMap["Record"])->setChecked(param.VideoOutput ? true : false);
            static_cast<LineEdit *>(m_alarmMap["Alarm time"])->setText(QString::number(param.AlarmDuration));
            static_cast<QComboBox *>(m_alarmMap["Type"])->setCurrentIndex(param.SensorType);

            int week = QDate::currentDate().dayOfWeek() == 7 ? 0 : QDate::currentDate().dayOfWeek();
            static_cast<QCheckBox *>(m_alarmMap[QString("week %1").arg(week)])->setCheckState(Qt::Checked);

            for(int i=0;i <4; i++) {
                static_cast<QCheckBox *>(m_alarmMap[QString("Time Period %1").arg(i)])->setChecked(param.Plans[week][i].PlanTimeEnabled ? true : false);
                static_cast<QTimeEdit *>(m_alarmMap[QString("Time Period %1 start").arg(i)])->setTime(param.Plans[week][i].BeginTime);
                static_cast<QTimeEdit *>(m_alarmMap[QString("Time Period %1 end").arg(i)])->setTime(param.Plans[week][i].EndTime);
                static_cast<QTimeEdit *>(m_alarmMap[QString("Time Period %1 start").arg(i)])->setEnabled(param.Plans[week][i].PlanTimeEnabled ? true : false);
                static_cast<QTimeEdit *>(m_alarmMap[QString("Time Period %1 end").arg(i)])->setEnabled(param.Plans[week][i].PlanTimeEnabled ? true : false);
            }
        }
    }
    default:
        return;
    }

    if (isOK)
        qDebug() << "#AlarmWidget# handleReceiveData, ParameterType:" << type << "parse data success...";
    else
        qDebug() << "#AlarmWidget# handleReceiveData, ParameterType:" << type << "parse data error...";
}

void AlarmWidget::handleTimeSelect(int state)
{
    TimeRegionWidget *region = NULL;
    QMap<QString, QWidget *> map;
    switch (currentIndex()) {
    case 0:
        region = static_cast<TimeRegionWidget *>(m_motionDetectionMap.value("region"));
        map = m_motionDetectionMap;
        break;
    case 1:
        region = static_cast<TimeRegionWidget *>(m_videoBlindMap.value("region"));
        map = m_videoBlindMap;
        break;
    case 2:
        region = static_cast<TimeRegionWidget *>(m_alarmMap.value("region"));
        map = m_alarmMap;
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

void AlarmWidget::handleSelectAllWeek()
{
    QMap<QString, QWidget *> map;
    switch (currentIndex()) {
    case 0:
        map = m_motionDetectionMap;
        break;
    case 1:
        map = m_videoBlindMap;
        break;
    case 2:
        map = m_alarmMap;
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
