#include "tabalarm.h"
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

TabAlarm::TabAlarm(QWidget *parent) : QTabWidget(parent)
{
    initRegionEditDialog();
    initMotionDetectionWidget();    
    initVideoBlindWidget();
    initAlarmWidget();

    connect(VidiconProtocol::getInstance(), &VidiconProtocol::signalSendData, this, &TabAlarm::handlerReceiveData);
    connect(this, &TabAlarm::signalSetParameter, VidiconProtocol::getInstance(), &VidiconProtocol::handlerSetParameter);
    connect(this, &TabAlarm::signalGetParameter, VidiconProtocol::getInstance(), &VidiconProtocol::handlerGetParameter);
    connect(this, &TabAlarm::currentChanged, this, [this](){
        handlerSwitchTab(QModelIndex());
    });

    emit signalGetParameter(MOTIONALARAPARAMETER);
    emit signalGetParameter(BLINDALARMPARAMETER);
    emit signalGetParameter(SENSORALARMPARAMETER);
}

TabAlarm::~TabAlarm()
{
    qDebug("delete TabAlarm");
}

void TabAlarm::initMotionDetectionWidget()
{
    QStringList list;
    motionDetectionWidget = new QWidget(this);

    QCheckBox *cb1 = new QCheckBox("Enable", motionDetectionWidget);
    motionDetectionMap.insert("Enable", cb1);
    QPushButton *btn1 = new QPushButton("Regional Edit", motionDetectionWidget);
    connect(btn1, &QPushButton::clicked, this, [this](){
        regionEditDialog->exec();
    });

    QCheckBox *cb2 = new QCheckBox("Alarm Output", motionDetectionWidget);
    motionDetectionMap.insert("Alarm Output", cb2);
    QCheckBox *cb3 = new QCheckBox("Record Video", motionDetectionWidget);
    motionDetectionMap.insert("Record Video", cb3);
    QLabel *lbl1 = new QLabel("Alarm Duration(Sec)(5-300):", motionDetectionWidget);
    QLineEdit *lineEdit1 = new QLineEdit(motionDetectionWidget);
    motionDetectionMap.insert("Alarm Duration", lineEdit1);

    QLabel *lbl2 = new QLabel("Pre-record time(Sec)(1-10):");
    QLineEdit *lineEdit2 = new QLineEdit(motionDetectionWidget);
    motionDetectionMap.insert("Pre-record time", lineEdit2);

    QLabel *lbl3 = new QLabel("Record time:");
    QComboBox *comboBox1 = new QComboBox(motionDetectionWidget);
    list << "100" << "300" << "600";
    comboBox1->addItems(list);
    list.clear();
    motionDetectionMap.insert("Record time", comboBox1);

    TimeRegionWidget *region = new TimeRegionWidget(motionDetectionWidget);
    motionDetectionMap.insert("region", region);

    QCheckBox *cb4 = new QCheckBox("Time Period 0", motionDetectionWidget);
    cb4->setObjectName("Time Period 0");
    connect(cb4, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerTimeSelectState);
    connect(cb4, &QCheckBox::stateChanged, this, &TabAlarm::handlerTimeSelect);
    motionDetectionMap.insert("Time Period 0", cb4);
    QTimeEdit *time1 = new QTimeEdit(motionDetectionWidget);
    time1->setEnabled(false);
    time1->setObjectName("Time Period 0 start");
    connect(time1, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handlerTimeChange);
    motionDetectionMap.insert("Time Period 0 start", time1);
    QLabel *lbl4 = new QLabel("-", motionDetectionWidget);
    QTimeEdit *time2 = new QTimeEdit(motionDetectionWidget);
    time2->setTime(QTime(23, 59, 59));
    time2->setEnabled(false);
    time2->setObjectName("Time Period 0 end");
    connect(time2, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handlerTimeChange);
    motionDetectionMap.insert("Time Period 0 end", time2);

    QCheckBox *cb5 = new QCheckBox("Time Period 1", motionDetectionWidget);
    cb5->setObjectName("Time Period 1");
    connect(cb5, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerTimeSelectState);
    connect(cb5, &QCheckBox::stateChanged, this, &TabAlarm::handlerTimeSelect);
    motionDetectionMap.insert("Time Period 1", cb5);
    QTimeEdit *time3 = new QTimeEdit(motionDetectionWidget);
    time3->setEnabled(false);
    time3->setObjectName("Time Period 1 start");
    connect(time3, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handlerTimeChange);
    motionDetectionMap.insert("Time Period 1 start", time3);
    QLabel *lbl5 = new QLabel("-", motionDetectionWidget);
    QTimeEdit *time4 = new QTimeEdit(motionDetectionWidget);
    time4->setTime(QTime(23, 59, 59));
    time4->setEnabled(false);
    time4->setObjectName("Time Period 1 end");
    connect(time4, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handlerTimeChange);
    motionDetectionMap.insert("Time Period 1 end", time4);

    QCheckBox *cb6 = new QCheckBox("Time Period 2", motionDetectionWidget);
    cb6->setObjectName("Time Period 2");
    connect(cb6, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerTimeSelectState);
    connect(cb6, &QCheckBox::stateChanged, this, &TabAlarm::handlerTimeSelect);
    motionDetectionMap.insert("Time Period 2", cb6);
    QTimeEdit *time5 = new QTimeEdit(motionDetectionWidget);
    time5->setEnabled(false);
    time5->setObjectName("Time Period 2 start");
    connect(time5, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handlerTimeChange);
    motionDetectionMap.insert("Time Period 2 start", time5);
    QLabel *lbl6 = new QLabel("-", motionDetectionWidget);
    QTimeEdit *time6 = new QTimeEdit(motionDetectionWidget);
    time6->setTime(QTime(23, 59, 59));
    time6->setEnabled(false);
    time6->setObjectName("Time Period 2 end");
    connect(time6, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handlerTimeChange);
    motionDetectionMap.insert("Time Period 2 end", time6);

    QCheckBox *cb7 = new QCheckBox("Time Period 3", motionDetectionWidget);
    cb7->setObjectName("Time Period 3");
    connect(cb7, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerTimeSelectState);
    connect(cb7, &QCheckBox::stateChanged, this, &TabAlarm::handlerTimeSelect);
    motionDetectionMap.insert("Time Period 3", cb7);
    QTimeEdit *time7 = new QTimeEdit(motionDetectionWidget);
    time7->setEnabled(false);
    time7->setObjectName("Time Period 3 start");
    connect(time7, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handlerTimeChange);
    motionDetectionMap.insert("Time Period 3 start", time7);
    QLabel *lbl7 = new QLabel("-", motionDetectionWidget);
    QTimeEdit *time8 = new QTimeEdit(motionDetectionWidget);
    time8->setTime(QTime(23, 59, 59));
    time8->setEnabled(false);
    time8->setObjectName("Time Period 3 end");
    connect(time8, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handlerTimeChange);
    motionDetectionMap.insert("Time Period 3 end", time8);

    QPushButton *btn2 = new QPushButton("Select All", motionDetectionWidget);
    connect(btn2, &QPushButton::clicked, this, &TabAlarm::handlerSelectAllWeek);

    QCheckBox *cb8 = new QCheckBox("Sunday", motionDetectionWidget);
    cb8->setObjectName("week 0");
    connect(cb8, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerWeekSelectState);
    motionDetectionMap.insert("week 0", cb8);

    QCheckBox *cb9 = new QCheckBox("Monday", motionDetectionWidget);
    cb9->setObjectName("week 1");
    connect(cb9, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerWeekSelectState);
    motionDetectionMap.insert("week 1", cb9);

    QCheckBox *cb10 = new QCheckBox("Tuesday", motionDetectionWidget);
    cb10->setObjectName("week 2");
    connect(cb10, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerWeekSelectState);
    motionDetectionMap.insert("week 2", cb10);

    QCheckBox *cb11 = new QCheckBox("Wednesday", motionDetectionWidget);
    cb11->setObjectName("week 3");
    connect(cb11, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerWeekSelectState);
    motionDetectionMap.insert("week 3", cb11);

    QCheckBox *cb12 = new QCheckBox("Thursday", motionDetectionWidget);
    cb12->setObjectName("week 4");
    connect(cb12, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerWeekSelectState);
    motionDetectionMap.insert("week 4", cb12);

    QCheckBox *cb13 = new QCheckBox("Friday", motionDetectionWidget);
    cb13->setObjectName("week 5");
    connect(cb13, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerWeekSelectState);
    motionDetectionMap.insert("week 5", cb13);

    QCheckBox *cb14 = new QCheckBox("Saturday", motionDetectionWidget);
    cb14->setObjectName("week 6");
    connect(cb14, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerWeekSelectState);
    motionDetectionMap.insert("week 6", cb14);

    QPushButton *btn3 = new QPushButton("Save", motionDetectionWidget);
    connect(btn3, &QPushButton::clicked, this, &TabAlarm::handlerPrepareData);

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

    QVBoxLayout *layout2 = new QVBoxLayout;
    layout2->addLayout(layout1);
    layout2->addStretch();

    QHBoxLayout *layout3 = new QHBoxLayout(motionDetectionWidget);
    layout3->addStretch(1);
    layout3->addLayout(layout2, 4);
    layout3->addStretch(1);

    addTab(motionDetectionWidget, "Motion Detection");
}

void TabAlarm::initVideoBlindWidget()
{
    QStringList list;
    videoBlindWidget = new QWidget(this);

    QCheckBox *cb1 = new QCheckBox("Enable", videoBlindWidget);
    videoBlindMap.insert("Enable", cb1);
    QCheckBox *cb2 = new QCheckBox("Output", videoBlindWidget);
    videoBlindMap.insert("Output", cb2);
    QCheckBox *cb3 = new QCheckBox("Record", videoBlindWidget);
    videoBlindMap.insert("Record", cb3);

    QLabel *lbl1 = new QLabel("Alarm time(Sec)(5-300):");
    QLineEdit *lineEdit1 = new QLineEdit(videoBlindWidget);
    videoBlindMap.insert("Alarm time", lineEdit1);
    QLabel *lbl2 = new QLabel("Sensitivity:");
    QComboBox *comboBox1 = new QComboBox(videoBlindWidget);
    list << "Low" << "Medium" << "High";
    comboBox1->addItems(list);
    list.clear();
    videoBlindMap.insert("Sensitivity", comboBox1);

    TimeRegionWidget *region = new TimeRegionWidget(videoBlindWidget);
    videoBlindMap.insert("region", region);

    QCheckBox *cb4 = new QCheckBox("Time Period 0", videoBlindWidget);
    cb4->setObjectName("Time Period 0");
    connect(cb4, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerTimeSelectState);
    connect(cb4, &QCheckBox::stateChanged, this, &TabAlarm::handlerTimeSelect);
    videoBlindMap.insert("Time Period 0", cb4);
    QTimeEdit *time1 = new QTimeEdit(videoBlindWidget);
    time1->setEnabled(false);
    time1->setObjectName("Time Period 0 start");
    connect(time1, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handlerTimeChange);
    videoBlindMap.insert("Time Period 0 start", time1);
    QLabel *lbl3 = new QLabel("-", videoBlindWidget);
    QTimeEdit *time2 = new QTimeEdit(videoBlindWidget);
    time2->setTime(QTime(23, 59, 59));
    time2->setEnabled(false);
    time2->setObjectName("Time Period 0 end");
    connect(time2, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handlerTimeChange);
    videoBlindMap.insert("Time Period 0 end", time2);

    QCheckBox *cb5 = new QCheckBox("Time Period 1", videoBlindWidget);
    cb5->setObjectName("Time Period 1");
    connect(cb5, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerTimeSelectState);
    connect(cb5, &QCheckBox::stateChanged, this, &TabAlarm::handlerTimeSelect);
    videoBlindMap.insert("Time Period 1", cb5);
    QTimeEdit *time3 = new QTimeEdit(videoBlindWidget);
    time3->setEnabled(false);
    time3->setObjectName("Time Period 1 start");
    connect(time3, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handlerTimeChange);
    videoBlindMap.insert("Time Period 1 start", time3);
    QLabel *lbl4 = new QLabel("-", videoBlindWidget);
    QTimeEdit *time4 = new QTimeEdit(videoBlindWidget);
    time4->setTime(QTime(23, 59, 59));
    time4->setEnabled(false);
    time4->setObjectName("Time Period 1 end");
    connect(time4, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handlerTimeChange);
    videoBlindMap.insert("Time Period 1 end", time4);

    QCheckBox *cb6 = new QCheckBox("Time Period 2", videoBlindWidget);
    cb6->setObjectName("Time Period 2");
    connect(cb6, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerTimeSelectState);
    connect(cb6, &QCheckBox::stateChanged, this, &TabAlarm::handlerTimeSelect);
    videoBlindMap.insert("Time Period 2", cb6);
    QTimeEdit *time5 = new QTimeEdit(videoBlindWidget);
    time5->setEnabled(false);
    time5->setObjectName("Time Period 2 start");
    connect(time5, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handlerTimeChange);
    videoBlindMap.insert("Time Period 2 start", time5);
    QLabel *lbl5 = new QLabel("-", videoBlindWidget);
    QTimeEdit *time6 = new QTimeEdit(videoBlindWidget);
    time6->setTime(QTime(23, 59, 59));
    time6->setEnabled(false);
    time6->setObjectName("Time Period 2 end");
    connect(time6, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handlerTimeChange);
    videoBlindMap.insert("Time Period 2 end", time6);

    QCheckBox *cb7 = new QCheckBox("Time Period 3", videoBlindWidget);
    cb7->setObjectName("Time Period 3");
    connect(cb7, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerTimeSelectState);
    connect(cb7, &QCheckBox::stateChanged, this, &TabAlarm::handlerTimeSelect);
    videoBlindMap.insert("Time Period 3", cb7);
    QTimeEdit *time7 = new QTimeEdit(videoBlindWidget);
    time7->setEnabled(false);
    time7->setObjectName("Time Period 3 start");
    connect(time7, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handlerTimeChange);
    videoBlindMap.insert("Time Period 3 start", time7);
    QLabel *lbl6 = new QLabel("-", videoBlindWidget);
    QTimeEdit *time8 = new QTimeEdit(videoBlindWidget);
    time8->setTime(QTime(23, 59, 59));
    time8->setEnabled(false);
    time8->setObjectName("Time Period 3 end");
    connect(time8, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handlerTimeChange);
    videoBlindMap.insert("Time Period 3 end", time8);

    QPushButton *btn2 = new QPushButton("Select All", videoBlindWidget);
    connect(btn2, &QPushButton::clicked, this, &TabAlarm::handlerSelectAllWeek);

    QCheckBox *cb8 = new QCheckBox("Sunday", videoBlindWidget);
    cb8->setObjectName("week 0");
    connect(cb8, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerWeekSelectState);
    videoBlindMap.insert("week 0", cb8);

    QCheckBox *cb9 = new QCheckBox("Monday", videoBlindWidget);
    cb9->setObjectName("week 1");
    connect(cb9, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerWeekSelectState);
    videoBlindMap.insert("week 1", cb9);

    QCheckBox *cb10 = new QCheckBox("Tuesday", videoBlindWidget);
    cb10->setObjectName("week 2");
    connect(cb10, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerWeekSelectState);
    videoBlindMap.insert("week 2", cb10);

    QCheckBox *cb11 = new QCheckBox("Wednesday", videoBlindWidget);
    cb11->setObjectName("week 3");
    connect(cb11, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerWeekSelectState);
    videoBlindMap.insert("week 3", cb11);

    QCheckBox *cb12 = new QCheckBox("Thursday", videoBlindWidget);
    cb12->setObjectName("week 4");
    connect(cb12, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerWeekSelectState);
    videoBlindMap.insert("week 4", cb12);

    QCheckBox *cb13 = new QCheckBox("Friday", videoBlindWidget);
    cb13->setObjectName("week 5");
    connect(cb13, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerWeekSelectState);
    videoBlindMap.insert("week 5", cb13);

    QCheckBox *cb14 = new QCheckBox("Saturday", videoBlindWidget);
    cb14->setObjectName("week 6");
    connect(cb14, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerWeekSelectState);
    videoBlindMap.insert("week 6", cb14);

    QPushButton *btn3 = new QPushButton("Save", videoBlindWidget);
    connect(btn3, &QPushButton::clicked, this, &TabAlarm::handlerPrepareData);

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

    QVBoxLayout *layout2 = new QVBoxLayout;
    layout2->addLayout(layout1);
    layout2->addStretch();

    QHBoxLayout *layout3 = new QHBoxLayout(videoBlindWidget);
    layout3->addStretch(1);
    layout3->addLayout(layout2, 4);
    layout3->addStretch(1);

    addTab(videoBlindWidget, "Video Blind");
}

void TabAlarm::initAlarmWidget()
{
    QStringList list;
    alarmWidget = new QWidget(this);

    QCheckBox *cb1 = new QCheckBox("Enable", alarmWidget);
    alarmMap.insert("Enable", cb1);
    QCheckBox *cb2 = new QCheckBox("Output", alarmWidget);
    alarmMap.insert("Output", cb2);
    QCheckBox *cb3 = new QCheckBox("Record", alarmWidget);
    alarmMap.insert("Record", cb3);

    QLabel *lbl1 = new QLabel("Alarm time(Sec)(5-300):");
    QLineEdit *lineEdit1 = new QLineEdit(alarmWidget);
    alarmMap.insert("Alarm time", lineEdit1);
    QLabel *lbl2 = new QLabel("Type:");
    QComboBox *comboBox1 = new QComboBox(alarmWidget);
    list << "NC" << "NO";
    comboBox1->addItems(list);
    list.clear();
    alarmMap.insert("Type", comboBox1);

    TimeRegionWidget *region = new TimeRegionWidget(alarmWidget);
    alarmMap.insert("region", region);

    QCheckBox *cb4 = new QCheckBox("Time Period 0", alarmWidget);
    cb4->setObjectName("Time Period 0");
    connect(cb4, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerTimeSelectState);
    connect(cb4, &QCheckBox::stateChanged, this, &TabAlarm::handlerTimeSelect);
    alarmMap.insert("Time Period 0", cb4);
    QTimeEdit *time1 = new QTimeEdit(alarmWidget);
    time1->setEnabled(false);
    time1->setObjectName("Time Period 0 start");
    connect(time1, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handlerTimeChange);
    alarmMap.insert("Time Period 0 start", time1);
    QLabel *lbl3 = new QLabel("-", alarmWidget);
    QTimeEdit *time2 = new QTimeEdit(alarmWidget);
    time2->setEnabled(false);
    time2->setObjectName("Time Period 0 end");
    connect(time2, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handlerTimeChange);
    alarmMap.insert("Time Period 0 end", time2);

    QCheckBox *cb5 = new QCheckBox("Time Period 1", alarmWidget);
    cb5->setObjectName("Time Period 1");
    connect(cb5, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerTimeSelectState);
    connect(cb5, &QCheckBox::stateChanged, this, &TabAlarm::handlerTimeSelect);
    alarmMap.insert("Time Period 1", cb5);
    QTimeEdit *time3 = new QTimeEdit(alarmWidget);
    time3->setEnabled(false);
    time3->setObjectName("Time Period 1 start");
    connect(time3, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handlerTimeChange);
    alarmMap.insert("Time Period 1 start", time3);
    QLabel *lbl4 = new QLabel("-", alarmWidget);
    QTimeEdit *time4 = new QTimeEdit(alarmWidget);
    time4->setEnabled(false);
    time4->setObjectName("Time Period 1 end");
    connect(time4, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handlerTimeChange);
    alarmMap.insert("Time Period 1 end", time4);

    QCheckBox *cb6 = new QCheckBox("Time Period 2", alarmWidget);
    cb6->setObjectName("Time Period 2");
    connect(cb6, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerTimeSelectState);
    connect(cb6, &QCheckBox::stateChanged, this, &TabAlarm::handlerTimeSelect);
    alarmMap.insert("Time Period 2", cb6);
    QTimeEdit *time5 = new QTimeEdit(alarmWidget);
    time5->setEnabled(false);
    time5->setObjectName("Time Period 2 start");
    connect(time5, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handlerTimeChange);
    alarmMap.insert("Time Period 2 start", time5);
    QLabel *lbl5 = new QLabel("-", alarmWidget);
    QTimeEdit *time6 = new QTimeEdit(alarmWidget);
    time6->setEnabled(false);
    time6->setObjectName("Time Period 2 end");
    connect(time6, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handlerTimeChange);
    alarmMap.insert("Time Period 2 end", time6);

    QCheckBox *cb7 = new QCheckBox("Time Period 3", alarmWidget);
    cb7->setObjectName("Time Period 3");
    connect(cb7, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerTimeSelectState);
    connect(cb7, &QCheckBox::stateChanged, this, &TabAlarm::handlerTimeSelect);
    alarmMap.insert("Time Period 3", cb7);
    QTimeEdit *time7 = new QTimeEdit(alarmWidget);
    time7->setEnabled(false);
    time7->setObjectName("Time Period 3 start");
    connect(time7, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handlerTimeChange);
    alarmMap.insert("Time Period 3 start", time7);
    QLabel *lbl6 = new QLabel("-", alarmWidget);
    QTimeEdit *time8 = new QTimeEdit(alarmWidget);
    time8->setEnabled(false);
    time8->setObjectName("Time Period 3 end");
    connect(time8, &QTimeEdit::timeChanged, region, &TimeRegionWidget::handlerTimeChange);
    alarmMap.insert("Time Period 3 end", time8);

    QPushButton *btn2 = new QPushButton("Select All", alarmWidget);
    connect(btn2, &QPushButton::clicked, this, &TabAlarm::handlerSelectAllWeek);

    QCheckBox *cb8 = new QCheckBox("Sunday", alarmWidget);
    cb8->setObjectName("week 0");
    connect(cb8, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerWeekSelectState);
    alarmMap.insert("week 0", cb8);

    QCheckBox *cb9 = new QCheckBox("Monday", alarmWidget);
    cb9->setObjectName("week 1");
    connect(cb9, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerWeekSelectState);
    alarmMap.insert("week 1", cb9);

    QCheckBox *cb10 = new QCheckBox("Tuesday", alarmWidget);
    cb10->setObjectName("week 2");
    connect(cb10, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerWeekSelectState);
    alarmMap.insert("week 2", cb10);

    QCheckBox *cb11 = new QCheckBox("Wednesday", alarmWidget);
    cb11->setObjectName("week 3");
    connect(cb11, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerWeekSelectState);
    alarmMap.insert("week 3", cb11);

    QCheckBox *cb12 = new QCheckBox("Thursday", alarmWidget);
    cb12->setObjectName("week 4");
    connect(cb12, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerWeekSelectState);
    alarmMap.insert("week 4", cb12);

    QCheckBox *cb13 = new QCheckBox("Friday", alarmWidget);
    cb13->setObjectName("week 5");
    connect(cb13, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerWeekSelectState);
    alarmMap.insert("week 5", cb13);

    QCheckBox *cb14 = new QCheckBox("Saturday", alarmWidget);
    cb14->setObjectName("week 6");
    connect(cb14, &QCheckBox::stateChanged, region, &TimeRegionWidget::handlerWeekSelectState);
    alarmMap.insert("week 6", cb14);

    QPushButton *btn3 = new QPushButton("Save", alarmWidget);
    connect(btn3, &QPushButton::clicked, this, &TabAlarm::handlerPrepareData);

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

    QVBoxLayout *layout2 = new QVBoxLayout;
    layout2->addLayout(layout1);
    layout2->addStretch();

    QHBoxLayout *layout3 = new QHBoxLayout(alarmWidget);
    layout3->addStretch(1);
    layout3->addLayout(layout2, 4);
    layout3->addStretch(1);

    addTab(alarmWidget, "Alarm");
}

void TabAlarm::initRegionEditDialog()
{
    QStringList list;
    regionEditDialog = new QDialog(this);

    MotionWidget *region = new MotionWidget(regionEditDialog);
    region->setFixedSize(440, 360);
    regionEditMap.insert("region", region);

    QLabel *lbl1 = new QLabel("Sensitivity", regionEditDialog);
    QComboBox *comboBox1 = new QComboBox(regionEditDialog);
    list << "Low" << "Medium" << "High";
    comboBox1->addItems(list);
    list.clear();
    regionEditMap.insert("Sensitivity", comboBox1);

    QLabel *lbl2 = new QLabel("Threshold", regionEditDialog);
    QSlider *slider1 = new QSlider(Qt::Horizontal, regionEditDialog);
    slider1->setRange(0, 100);
    QLabel *lbl3 = new QLabel(regionEditDialog);
    regionEditMap.insert("Threshold", slider1);
    regionEditMap.insert("ThresholdTag", lbl3);
    connect(slider1, &QSlider::valueChanged, this, [this](int value){
        static_cast<QLabel *>(regionEditMap["ThresholdTag"])->setText(QString::number(value));
    });

    QPushButton *btn1 = new QPushButton("FullScreen", regionEditDialog);
    connect(btn1, &QPushButton::clicked, region, &MotionWidget::handlerFullScreen);
    QPushButton *btn2 = new QPushButton("ClearScreen", regionEditDialog);
    connect(btn2, &QPushButton::clicked, region, &MotionWidget::handlerCleanScreen);
    QPushButton *btn3 = new QPushButton("Save", regionEditDialog);
    btn3->setObjectName("MotionRegion");
    connect(btn3, &QPushButton::clicked, this, &TabAlarm::handlerPrepareData);

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

    QVBoxLayout *layout2 = new QVBoxLayout;
    layout2->addLayout(layout1);
    layout2->addStretch();

    QHBoxLayout *layout3 = new QHBoxLayout(regionEditDialog);
    layout3->addStretch();
    layout3->addLayout(layout2);
    layout3->addStretch();

}

void TabAlarm::handlerSwitchTab(const QModelIndex &index)
{
    int type = index.row();
    if(sender() != this) {
        setCurrentIndex(index.row());
    }else {
        type = currentIndex();
    }

    switch(type){
    case 0: {
        emit signalGetParameter(MOTIONALARAPARAMETER);
        break;
    }
    case 1: {
        emit signalGetParameter(BLINDALARMPARAMETER);
        break;
    }
    case 2: {
        emit signalGetParameter(SENSORALARMPARAMETER);
        break;
    }
    default:
        break;
    }
}

void TabAlarm::handlerTimeSelect(int state)
{
    TimeRegionWidget *region = NULL;
    QMap<QString, QWidget *> map;
    switch (currentIndex()) {
    case 0:
        region = static_cast<TimeRegionWidget *>(motionDetectionMap.value("region"));
        map = motionDetectionMap;
        break;
    case 1:
        region = static_cast<TimeRegionWidget *>(videoBlindMap.value("region"));
        map = videoBlindMap;
        break;
    case 2:
        region = static_cast<TimeRegionWidget *>(alarmMap.value("region"));
        map = alarmMap;
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

void TabAlarm::handlerSelectAllWeek()
{
    QMap<QString, QWidget *> map;
    switch (currentIndex()) {
    case 0:
        map = motionDetectionMap;
        break;
    case 1:
        map = videoBlindMap;
        break;
    case 2:
        map = alarmMap;
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

void TabAlarm::handlerPrepareData()
{
    switch(currentIndex()) {
    case 0: {
        VidiconProtocol::MotionDetectionParameter *param = new VidiconProtocol::MotionDetectionParameter;
        param->Enabled = static_cast<QCheckBox *>(motionDetectionMap["Enable"])->checkState() == Qt::Checked ? 1 : 0;
        param->AlarmOutput = static_cast<QCheckBox *>(motionDetectionMap["Alarm Output"])->checkState() == Qt::Checked ? 1 : 0;
        param->VideoOutput = static_cast<QCheckBox *>(motionDetectionMap["Record Video"])->checkState() == Qt::Checked ? 1 : 0;
        param->AlarmDuration = static_cast<QLineEdit *>(motionDetectionMap["Alarm Duration"])->text().toInt();
        param->PreRecTime = static_cast<QLineEdit *>(motionDetectionMap["Pre-record time"])->text().toInt();
        param->DelayRecTime = static_cast<QComboBox *>(motionDetectionMap["Record time"])->currentText().toInt();
        param->weeksStateMap = static_cast<TimeRegionWidget *>(motionDetectionMap["region"])->getWeeksState();
        param->Plans = static_cast<TimeRegionWidget *>(motionDetectionMap["region"])->getPlans();
        if(sender()->objectName() == "MotionRegion") {
            char **map = static_cast<MotionWidget *>(regionEditMap["region"])->getMotionRegionMap();
            for(int i=0; i<REGIONROW; i++) {
                for(int j=0; j<REGIONCOLUMN; j++) {
                    param->AreaMask.append(QString::number(map[i][j]));
                }
            }
            param->Sensitivity = static_cast<QComboBox *>(regionEditMap["Sensitivity"])->currentIndex();
            param->AlarmThreshold = static_cast<QSlider *>(regionEditMap["Threshold"])->value();
            param->onlyRegion = true;
        }else {
            param->onlyRegion = false;
        }
        emit signalSetParameter(MOTIONALARAPARAMETER, param);
        break;
    }
    case 1: {
        VidiconProtocol::VideoBlindAlarmParameter *param = new VidiconProtocol::VideoBlindAlarmParameter;
        param->Enabled = static_cast<QCheckBox *>(videoBlindMap["Enable"])->checkState() == Qt::Checked ? 1 : 0;
        param->AlarmOutput = static_cast<QCheckBox *>(videoBlindMap["Output"])->checkState() == Qt::Checked ? 1 : 0;
        param->VideoOutput = static_cast<QCheckBox *>(videoBlindMap["Record"])->checkState() == Qt::Checked ? 1 : 0;
        param->AlarmDuration = static_cast<QLineEdit *>(videoBlindMap["Alarm time"])->text().toInt();
        param->Sensitivity = static_cast<QComboBox *>(videoBlindMap["Sensitivity"])->currentIndex();
        param->weeksStateMap = static_cast<TimeRegionWidget *>(videoBlindMap["region"])->getWeeksState();
        param->Plans = static_cast<TimeRegionWidget *>(videoBlindMap["region"])->getPlans();
        emit signalSetParameter(BLINDALARMPARAMETER, param);
        break;
    }
    case 2: {
        VidiconProtocol::SensorAlarmParameter *param = new VidiconProtocol::SensorAlarmParameter;
        param->Enabled = static_cast<QCheckBox *>(alarmMap["Enable"])->checkState() == Qt::Checked ? 1 : 0;
        param->AlarmOutput = static_cast<QCheckBox *>(alarmMap["Output"])->checkState() == Qt::Checked ? 1 : 0;
        param->VideoOutput = static_cast<QCheckBox *>(alarmMap["Record"])->checkState() == Qt::Checked ? 1 : 0;
        param->AlarmDuration = static_cast<QLineEdit *>(alarmMap["Alarm time"])->text().toInt();
        param->SensorType = static_cast<QComboBox *>(alarmMap["Type"])->currentIndex();
        param->weeksStateMap = static_cast<TimeRegionWidget *>(alarmMap["region"])->getWeeksState();
        param->Plans = static_cast<TimeRegionWidget *>(alarmMap["region"])->getPlans();
        emit signalSetParameter(SENSORALARMPARAMETER, param);
        break;
    }
    default:
        break;
    }
}

void TabAlarm::handlerReceiveData(int type, QByteArray data)
{    
    switch(type) {
    case MOTIONALARAPARAMETER: {
        VidiconProtocol::MotionDetectionParameter param;
        param.Plans = static_cast<TimeRegionWidget *>(motionDetectionMap["region"])->getPlans();
        if(ParseXML::getInstance()->parseMotionParameter(&param, data)) {
            static_cast<QCheckBox *>(motionDetectionMap["Enable"])->setChecked(param.Enabled ? true : false);
            static_cast<QCheckBox *>(motionDetectionMap["Alarm Output"])->setChecked(param.AlarmOutput ? true : false);
            static_cast<QCheckBox *>(motionDetectionMap["Record Video"])->setChecked(param.VideoOutput ? true : false);
            static_cast<QLineEdit *>(motionDetectionMap["Alarm Duration"])->setText(QString::number(param.AlarmDuration));
            static_cast<QLineEdit *>(motionDetectionMap["Pre-record time"])->setText(QString::number(param.PreRecTime));
            static_cast<QComboBox *>(motionDetectionMap["Record time"])->setCurrentText(QString::number(param.DelayRecTime));

            static_cast<QComboBox *>(regionEditMap["Sensitivity"])->setCurrentIndex(param.Sensitivity);
            static_cast<QSlider *>(regionEditMap["Threshold"])->setValue(param.AlarmThreshold);
            static_cast<TimeRegionWidget *>(motionDetectionMap["region"])->update();
            int week = QDate::currentDate().dayOfWeek() == 7 ? 0 : QDate::currentDate().dayOfWeek();
            static_cast<QCheckBox *>(motionDetectionMap[QString("week %1").arg(week)])->setCheckState(Qt::Checked);

            for(int i=0;i <4; i++) {
                static_cast<QCheckBox *>(motionDetectionMap[QString("Time Period %1").arg(i)])->setChecked(param.Plans[week][i].PlanTimeEnabled ? true : false);
                static_cast<QTimeEdit *>(motionDetectionMap[QString("Time Period %1 start").arg(i)])->setTime(param.Plans[week][i].BeginTime);
                static_cast<QTimeEdit *>(motionDetectionMap[QString("Time Period %1 end").arg(i)])->setTime(param.Plans[week][i].EndTime);
                static_cast<QTimeEdit *>(motionDetectionMap[QString("Time Period %1 start").arg(i)])->setEnabled(param.Plans[week][i].PlanTimeEnabled ? true : false);
                static_cast<QTimeEdit *>(motionDetectionMap[QString("Time Period %1 end").arg(i)])->setEnabled(param.Plans[week][i].PlanTimeEnabled ? true : false);
            }
            char **map = static_cast<MotionWidget *>(regionEditMap["region"])->getMotionRegionMap();
            for(int i=0; i<REGIONROW; i++) {
                for(int j=0; j<REGIONCOLUMN; j++) {
                    map[i][j] = param.AreaMask.mid(i * REGIONCOLUMN + j, 1).toInt();
                }
            }
            static_cast<MotionWidget *>(regionEditMap["region"])->update();
            qDebug() << "#TabAlarm# handlerReceiveData, ParameterType:" << type << "parse data success...";
        }else {
            qDebug() << "#TabAlarm# handlerReceiveData, ParameterType:" << type << "parse data error...";
        }
        break;
    }
    case BLINDALARMPARAMETER: {
        VidiconProtocol::VideoBlindAlarmParameter param;
        param.Plans = static_cast<TimeRegionWidget *>(videoBlindMap["region"])->getPlans();
        if(ParseXML::getInstance()->parseBlindParameter(&param, data)) {
            static_cast<QCheckBox *>(videoBlindMap["Enable"])->setChecked(param.Enabled ? true : false);
            static_cast<QCheckBox *>(videoBlindMap["Output"])->setChecked(param.AlarmOutput ? true : false);
            static_cast<QCheckBox *>(videoBlindMap["Record"])->setChecked(param.VideoOutput ? true : false);
            static_cast<QLineEdit *>(videoBlindMap["Alarm time"])->setText(QString::number(param.AlarmDuration));
            static_cast<QComboBox *>(videoBlindMap["Sensitivity"])->setCurrentIndex(param.Sensitivity);

            int week = QDate::currentDate().dayOfWeek() == 7 ? 0 : QDate::currentDate().dayOfWeek();
            static_cast<QCheckBox *>(videoBlindMap[QString("week %1").arg(week)])->setCheckState(Qt::Checked);

            for(int i=0;i <4; i++) {
                static_cast<QCheckBox *>(videoBlindMap[QString("Time Period %1").arg(i)])->setChecked(param.Plans[week][i].PlanTimeEnabled ? true : false);
                static_cast<QTimeEdit *>(videoBlindMap[QString("Time Period %1 start").arg(i)])->setTime(param.Plans[week][i].BeginTime);
                static_cast<QTimeEdit *>(videoBlindMap[QString("Time Period %1 end").arg(i)])->setTime(param.Plans[week][i].EndTime);
                static_cast<QTimeEdit *>(videoBlindMap[QString("Time Period %1 start").arg(i)])->setEnabled(param.Plans[week][i].PlanTimeEnabled ? true : false);
                static_cast<QTimeEdit *>(videoBlindMap[QString("Time Period %1 end").arg(i)])->setEnabled(param.Plans[week][i].PlanTimeEnabled ? true : false);
            }
            qDebug() << "#TabAlarm# handlerReceiveData, ParameterType:" << type << "parse data success...";
        }else{
            qDebug() << "#TabAlarm# handlerReceiveData, ParameterType:" << type << "parse data error...";
        }
        break;
    }
    case SENSORALARMPARAMETER: {
        VidiconProtocol::SensorAlarmParameter param;
        param.Plans = static_cast<TimeRegionWidget *>(alarmMap["region"])->getPlans();
        if(ParseXML::getInstance()->parseSensorParameter(&param, data)) {
            static_cast<QCheckBox *>(alarmMap["Enable"])->setChecked(param.Enabled ? true : false);
            static_cast<QCheckBox *>(alarmMap["Output"])->setChecked(param.AlarmOutput ? true : false);
            static_cast<QCheckBox *>(alarmMap["Record"])->setChecked(param.VideoOutput ? true : false);
            static_cast<QLineEdit *>(alarmMap["Alarm time"])->setText(QString::number(param.AlarmDuration));
            static_cast<QComboBox *>(alarmMap["Type"])->setCurrentIndex(param.SensorType);

            int week = QDate::currentDate().dayOfWeek() == 7 ? 0 : QDate::currentDate().dayOfWeek();
            static_cast<QCheckBox *>(alarmMap[QString("week %1").arg(week)])->setCheckState(Qt::Checked);

            for(int i=0;i <4; i++) {
                static_cast<QCheckBox *>(alarmMap[QString("Time Period %1").arg(i)])->setChecked(param.Plans[week][i].PlanTimeEnabled ? true : false);
                static_cast<QTimeEdit *>(alarmMap[QString("Time Period %1 start").arg(i)])->setTime(param.Plans[week][i].BeginTime);
                static_cast<QTimeEdit *>(alarmMap[QString("Time Period %1 end").arg(i)])->setTime(param.Plans[week][i].EndTime);
                static_cast<QTimeEdit *>(alarmMap[QString("Time Period %1 start").arg(i)])->setEnabled(param.Plans[week][i].PlanTimeEnabled ? true : false);
                static_cast<QTimeEdit *>(alarmMap[QString("Time Period %1 end").arg(i)])->setEnabled(param.Plans[week][i].PlanTimeEnabled ? true : false);
            }

            qDebug() << "#TabAlarm# handlerReceiveData, ParameterType:" << type << "parse data success...";
        }else{
            qDebug() << "#TabAlarm# handlerReceiveData, ParameterType:" << type << "parse data error...";
        }
    }
    default:
        break;
    }
}