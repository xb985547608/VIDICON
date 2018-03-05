#include "mediawidget.h"
#include <QGridLayout>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QSlider>
#include <QDebug>
#include <QModelIndex>
#include "Protocol/vidiconprotocol.h"
#include "parsexml.h"
#include "DrawSubWidget/osdwidget.h"
#include <QCheckBox>
#include <QTimeEdit>
#include "DrawSubWidget/privacywidget.h"
#include "DrawSubWidget/imagewidget.h"
#include "waitingshade.h"

MediaWidget::MediaWidget(QWidget *parent) : QStackedWidget(parent)
{
    initAudioVideoWidget();
    initPrivacyWidget();
    initImageWidget();
    initROIWidget();
    initOSDWidget();

    connect(VidiconProtocol::getInstance(), &VidiconProtocol::signalSendData, this, &MediaWidget::handlerReceiveData);
    connect(this, &MediaWidget::signalSetParameter, VidiconProtocol::getInstance(), &VidiconProtocol::handlerSetParameter);
    connect(this, &MediaWidget::signalGetParameter, VidiconProtocol::getInstance(), &VidiconProtocol::handlerGetParameter);
    connect(this, &MediaWidget::currentChanged, this, [this](){
        handlerSwitchTab(QModelIndex());
    });
}

MediaWidget::~MediaWidget()
{
    qDebug("delete TabMedia");
}

void MediaWidget::initAudioVideoWidget()
{
    QStringList list;
    audioVideoWidget = new QWidget(this);

    QLabel *lbl1 = new QLabel("Standard", audioVideoWidget);
    QComboBox *comboBox1 = new QComboBox(audioVideoWidget);
    list << "PAL" << "NTSC";
    comboBox1->addItems(list);
    list.clear();
    audioVideoMap.insert("Standard 0", comboBox1);

    QLabel *lbl25 = new QLabel("Standard", audioVideoWidget);
    QComboBox *comboBox13 = new QComboBox(audioVideoWidget);
    list << "PAL" << "NTSC";
    comboBox13->addItems(list);
    list.clear();
    audioVideoMap.insert("Standard 1", comboBox13);

    QLabel *lbl2 = new QLabel("码流类型", audioVideoWidget);
    QLabel *lbl3 = new QLabel("主码流", audioVideoWidget);

    QLabel *lbl4 = new QLabel("码流类型", audioVideoWidget);
    QLabel *lbl5 = new QLabel("子码流", audioVideoWidget);


    QLabel *lbl6 = new QLabel("帧率", audioVideoWidget);
    QComboBox *comboBox2 = new QComboBox(audioVideoWidget);
    for(int i=1; i<=25; i++){
        list.append(QString::number(i));
    }
    comboBox2->addItems(list);
    list.clear();
    audioVideoMap.insert("Frame Rate 0", comboBox2);

    QLabel *lbl7 = new QLabel("帧率", audioVideoWidget);
    QComboBox *comboBox3 = new QComboBox(audioVideoWidget);
    for(int i=1; i<=25; i++){
        list.append(QString::number(i));
    }
    comboBox3->addItems(list);
    list.clear();
    audioVideoMap.insert("Frame Rate 1", comboBox3);

    QLabel *lbl8 = new QLabel("编码类型", audioVideoWidget);
    QComboBox *comboBox4 = new QComboBox(audioVideoWidget);
    list << "H264" << "H265";
    comboBox4->addItems(list);
    list.clear();
    audioVideoMap.insert("Codec 0", comboBox4);

    QLabel *lbl9 = new QLabel("编码类型", audioVideoWidget);
    QComboBox *comboBox5 = new QComboBox(audioVideoWidget);
    list << "H264" << "H265";
    comboBox5->addItems(list);
    list.clear();
    audioVideoMap.insert("Codec 1", comboBox5);

    QLabel *lbl10 = new QLabel("码流模式", audioVideoWidget);
    QComboBox *comboBox6 = new QComboBox(audioVideoWidget);
    list << "CBR" << "VBR";
    comboBox6->addItems(list);
    list.clear();
    audioVideoMap.insert("Bitrate Mode 0", comboBox6);

    QLabel *lbl11 = new QLabel("码流模式", audioVideoWidget);
    QComboBox *comboBox7 = new QComboBox(audioVideoWidget);
    list << "CBR" << "VBR";
    comboBox7->addItems(list);
    list.clear();
    audioVideoMap.insert("Bitrate Mode 1", comboBox7);

    QLabel *lbl12 = new QLabel("分辨率", audioVideoWidget);
    QComboBox *comboBox8 = new QComboBox(audioVideoWidget);
    list << "1920*1080" << "1280*720";
    comboBox8->addItems(list);
    list.clear();
    audioVideoMap.insert("Resolution 0", comboBox8);

    QLabel *lbl13 = new QLabel("分辨率", audioVideoWidget);
    QComboBox *comboBox9 = new QComboBox(audioVideoWidget);
    list << "352*288" << "704*576";
    comboBox9->addItems(list);
    list.clear();
    audioVideoMap.insert("Resolution 1", comboBox9);

    QLabel *lbl14 = new QLabel("视频质量", audioVideoWidget);
    QComboBox *comboBox10 = new QComboBox(audioVideoWidget);
    list << "差" << "正常" << "好" << "更好" << "最好";
    comboBox10->addItems(list);
    list.clear();
    audioVideoMap.insert("Video quality 0", comboBox10);

    QLabel *lbl15 = new QLabel("视频质量", audioVideoWidget);
    QComboBox *comboBox11 = new QComboBox(audioVideoWidget);
    list << "差" << "正常" << "好" << "更好" << "最好";
    comboBox11->addItems(list);
    list.clear();
    audioVideoMap.insert("Video quality 1", comboBox11);

    QLabel *lbl16 = new QLabel("码流大小", audioVideoWidget);
    QLineEdit *lineEdit1 = new QLineEdit(audioVideoWidget);
    lineEdit1->setValidator(new QIntValidator(36, 10240, this));
    QLabel *lbl17 = new QLabel("(Kbit)(36-10240)", audioVideoWidget);
    audioVideoMap.insert("Video Rate 0", lineEdit1);

    QLabel *lbl18 = new QLabel("码流大小", audioVideoWidget);
    QLineEdit *lineEdit2 = new QLineEdit(audioVideoWidget);
    lineEdit2->setValidator(new QIntValidator(20, 2048, this));
    QLabel *lbl19 = new QLabel("(Kbit)(20-2048)", audioVideoWidget);
    audioVideoMap.insert("Video Rate 1", lineEdit2);

    QLabel *lbl20 = new QLabel("I帧间隔", audioVideoWidget);
    QLineEdit *lineEdit3 = new QLineEdit(audioVideoWidget);
    lineEdit3->setValidator(new QIntValidator(1, 150, this));
    QLabel *lbl21 = new QLabel("(FPS)(1-150)", audioVideoWidget);
    audioVideoMap.insert("I Frame Interval 0", lineEdit3);

    QLabel *lbl22 = new QLabel("I帧间隔", audioVideoWidget);
    QLineEdit *lineEdit4 = new QLineEdit(audioVideoWidget);
    lineEdit4->setValidator(new QIntValidator(1, 150, this));
    QLabel *lbl23 = new QLabel("(FPS)(1-150)", audioVideoWidget);
    audioVideoMap.insert("I Frame Interval 1", lineEdit4);

    QRadioButton *radioBtn = new QRadioButton("启用音频", audioVideoWidget);
    audioVideoMap.insert("Audio Enable", radioBtn);

    QLabel *lbl24 = new QLabel("编码类型", audioVideoWidget);
    QComboBox *comboBox12 = new QComboBox(audioVideoWidget);
    list << "G711A" << "G711U" /*<< "G726" << "AAC"*/;
    comboBox12->addItems(list);
    list.clear();
    audioVideoMap.insert("Audio Codec", comboBox12);

    QPushButton *btn = new QPushButton("保存", audioVideoWidget);
    connect(btn, &QPushButton::clicked, this, [this](){handlerPrepareData();});

    QGridLayout *layout = new QGridLayout;
    layout->setSpacing(10);

    //TODO:  暂不支持标准切换功能
//    layout->addWidget(lbl1,      0, 0, 1, 3);
//    layout->addWidget(comboBox1, 0, 3, 1, 5);
//    layout->addWidget(lbl25,     0, 8, 1, 3);
//    layout->addWidget(comboBox13,0, 11, 1, 5);
    lbl1->setVisible(false);
    comboBox1->setVisible(false);
    lbl25->setVisible(false);
    comboBox13->setVisible(false);

    layout->addWidget(lbl2, 1, 0, 1, 3);
    layout->addWidget(lbl3, 1, 3, 1, 5);
    layout->addWidget(lbl4, 1, 8, 1, 3);
    layout->addWidget(lbl5, 1, 11,1, 5);

    layout->addWidget(lbl6,      2, 0,  1, 3);
    layout->addWidget(comboBox2, 2, 3,  1, 5);
    layout->addWidget(lbl7,      2, 8,  1, 3);
    layout->addWidget(comboBox3, 2, 11, 1, 5);

    layout->addWidget(lbl8,      3, 0,  1, 3);
    layout->addWidget(comboBox4, 3, 3,  1, 5);
    layout->addWidget(lbl9,      3, 8,  1, 3);
    layout->addWidget(comboBox5, 3, 11, 1, 5);

    layout->addWidget(lbl10,      4, 0,  1, 3);
    layout->addWidget(comboBox6,  4, 3,  1, 5);
    layout->addWidget(lbl11,      4, 8,  1, 3);
    layout->addWidget(comboBox7,  4, 11, 1, 5);

    layout->addWidget(lbl12,      5, 0,  1, 3);
    layout->addWidget(comboBox8,  5, 3,  1, 5);
    layout->addWidget(lbl13,      5, 8,  1, 3);
    layout->addWidget(comboBox9,  5, 11, 1, 5);

    layout->addWidget(lbl14,      6, 0,  1, 3);
    layout->addWidget(comboBox10, 6, 3,  1, 5);
    layout->addWidget(lbl15,      6, 8,  1, 3);
    layout->addWidget(comboBox11, 6, 11, 1, 5);

    layout->addWidget(lbl16,     7, 0,  1, 3);
    layout->addWidget(lineEdit1, 7, 3,  1, 3);
    layout->addWidget(lbl17,     7, 6,  1, 2);
    layout->addWidget(lbl18,     7, 8,  1, 3);
    layout->addWidget(lineEdit2, 7, 11, 1, 3);
    layout->addWidget(lbl19,     7, 14, 1, 2);

    layout->addWidget(lbl20,     8, 0,  1, 3);
    layout->addWidget(lineEdit3, 8, 3,  1, 3);
    layout->addWidget(lbl21,     8, 6,  1, 2);
    layout->addWidget(lbl22,     8, 8,  1, 3);
    layout->addWidget(lineEdit4, 8, 11, 1, 3);
    layout->addWidget(lbl23,     8, 14, 1, 2);

    layout->addWidget(radioBtn, 9, 0, 1, 3);

    layout->addWidget(lbl24,       10, 0,  1, 3);
    layout->addWidget(comboBox12,  10, 3,  1, 5);

    layout->addWidget(btn, 11, 7, 1, 2);

    QVBoxLayout *vLayout = new QVBoxLayout(audioVideoWidget);
    vLayout->addLayout(layout);
    vLayout->addStretch();

    addWidget(audioVideoWidget);

}

void MediaWidget::initPrivacyWidget()
{
    privacyWidget = new QWidget(this);

    PrivacyWidget *displayArea = new PrivacyWidget(privacyWidget);
    displayArea->setFixedSize(400, 400);
    displayArea->setStyleSheet("background-color:black");
    privacyMap.insert("DisplayArea", displayArea);

    QRadioButton *radioBtn1 = new QRadioButton("使能(支持4个区域)", privacyWidget);
    privacyMap.insert("Enable", radioBtn1);

    QPushButton *btn1 = new QPushButton("全屏", privacyWidget);
    privacyMap.insert("FullScreen", btn1);

    QPushButton *btn2 = new QPushButton("清屏", privacyWidget);
    privacyMap.insert("ClearScreen", btn2);

    QPushButton *btn3 = new QPushButton("保存", privacyWidget);
    privacyMap.insert("Save", btn3);
    connect(btn3, &QPushButton::clicked, this, &MediaWidget::handlerPrepareData);

    QHBoxLayout *layout1 = new QHBoxLayout;
    layout1->addStretch();
    layout1->addWidget(displayArea);
    layout1->addStretch();

    QHBoxLayout *layout2 = new QHBoxLayout;
    layout2->addWidget(radioBtn1);
    layout2->addStretch();

    QHBoxLayout *layout3 = new QHBoxLayout;
    layout3->addWidget(btn1);
    layout3->addWidget(btn2);
    layout3->addWidget(btn3);

    QVBoxLayout *layout4 = new QVBoxLayout;
    layout4->addLayout(layout1);
    layout4->addLayout(layout2);
    layout4->addLayout(layout3);
    layout4->addStretch();

    QHBoxLayout *layout5 = new QHBoxLayout(privacyWidget);
    layout5->addStretch();
    layout5->addLayout(layout4);
    layout5->addStretch();

    addWidget(privacyWidget);
}

void MediaWidget::initImageWidget()
{
    QStringList list;
    imageWidget = new QWidget(this);

    ImageWidget *displayArea = new ImageWidget(imageWidget);
    displayArea->setFixedSize(250, 250);
    displayArea->setStyleSheet("background-color:black");
    imageMap.insert("DisplayArea", displayArea);

    QLabel *lbl1 = new QLabel("色度：",imageWidget);
    QSlider *slider1 = new QSlider(Qt::Horizontal, imageWidget);
    slider1->setRange(1, 100);
    slider1->setFixedWidth(100);
    QLabel *lbl2 = new QLabel(imageWidget);
    lbl2->setFixedWidth(20);
    imageMap.insert("HueLevel", slider1);
    connect(slider1, &QSlider::valueChanged, this, [lbl2](int value){
        lbl2->setText(QString::number(value));
    });

    QLabel *lbl3 = new QLabel("亮度：",imageWidget);
    QSlider *slider2 = new QSlider(Qt::Horizontal, imageWidget);
    slider2->setRange(1, 100);
    slider2->setFixedWidth(100);
    QLabel *lbl4 = new QLabel(imageWidget);
    imageMap.insert("BrightnessLevel", slider2);
    connect(slider2, &QSlider::valueChanged, this, [lbl4](int value){
        lbl4->setText(QString::number(value));
    });

    QLabel *lbl5 = new QLabel("对比度：",imageWidget);
    QSlider *slider3 = new QSlider(Qt::Horizontal, imageWidget);
    slider3->setRange(1, 100);
    slider3->setFixedWidth(100);
    QLabel *lbl6 = new QLabel(imageWidget);
    imageMap.insert("ContrastLevel", slider3);
    connect(slider3, &QSlider::valueChanged, this, [lbl6](int value){
        lbl6->setText(QString::number(value));
    });

    QLabel *lbl7 = new QLabel("饱和度：",imageWidget);
    QSlider *slider4 = new QSlider(Qt::Horizontal, imageWidget);
    slider4->setRange(1, 100);
    slider4->setFixedWidth(100);
    QLabel *lbl8 = new QLabel(imageWidget);
    imageMap.insert("SaturationLevel", slider4);
    connect(slider4, &QSlider::valueChanged, this, [lbl8](int value){
        lbl8->setText(QString::number(value));
    });

    QLabel *lbl9 = new QLabel("锐度：",imageWidget);
    QSlider *slider5 = new QSlider(Qt::Horizontal, imageWidget);
    slider5->setRange(1, 100);
    slider5->setFixedWidth(100);
    QLabel *lbl10 = new QLabel(imageWidget);
    imageMap.insert("Sharpness", slider5);
    connect(slider5, &QSlider::valueChanged, this, [lbl10](int value){
        lbl10->setText(QString::number(value));
    });

    QLabel *lbl11 = new QLabel("镜像：", imageWidget);
    QComboBox *comboBox1 = new QComboBox(imageWidget);
    list << "左" << "右";
    comboBox1->addItems(list);
    list.clear();
    imageMap.insert("Mirror", comboBox1);

    QLabel *lbl12 = new QLabel("翻转：", imageWidget);
    QComboBox *comboBox2 = new QComboBox(imageWidget);
    list << "上" << "下";
    comboBox2->addItems(list);
    list.clear();
    imageMap.insert("Turn", comboBox2);

    QLabel *lbl13 = new QLabel("视角：", imageWidget);
    QComboBox *comboBox3 = new QComboBox(imageWidget);
    list << "正常" << "顺时针旋转90" << "顺时针旋转270" << "顺时针旋转180";
    comboBox3->addItems(list);
    list.clear();
    imageMap.insert("VisionMode", comboBox3);

    QLabel *lbl14 = new QLabel("3D降噪：", imageWidget);
    QComboBox *comboBox4 = new QComboBox(imageWidget);
    list << "失能" << "使能";
    comboBox4->addItems(list);
    list.clear();
    imageMap.insert("NoiseReduceMode", comboBox4);

    QLabel *lbl15 = new QLabel("快门调节：", imageWidget);
    QComboBox *comboBox5 = new QComboBox(imageWidget);
    list << "1/25" << "1/30" << "1/50" << "1/60" << "1/100" << "1/120" << "1/250" << "1/500" << "1/1000"
         << "1/2000" << "1/4000" << "1/10000" << "1/30000" << "1/100000";
    comboBox5->addItems(list);
    list.clear();
    imageMap.insert("Shutter", comboBox5);

    QLabel *lbl16 = new QLabel("鱼眼矫正：", imageWidget);
    QComboBox *comboBox6 = new QComboBox(imageWidget);
    list << "失能" << "使能";
    comboBox6->addItems(list);
    list.clear();
    imageMap.insert("Fisheye correction", comboBox6);

    QLabel *lbl17 = new QLabel("防闪烁：", imageWidget);
    QComboBox *comboBox7 = new QComboBox(imageWidget);
    list << "失能" << "使能";
    comboBox7->addItems(list);
    list.clear();
    imageMap.insert("AntiFlashMode", comboBox7);

    QLabel *lbl18 = new QLabel("曝光：", imageWidget);
    QComboBox *comboBox8 = new QComboBox(imageWidget);
    list << "自动" << "手动";
    comboBox8->addItems(list);
    list.clear();
    imageMap.insert("ExposureMode", comboBox8);

    QLabel *lbl19 = new QLabel("背光补偿：", imageWidget);
    QComboBox *comboBox9 = new QComboBox(imageWidget);
    list << "失能" << "背光补偿" << "宽动态" << "强光抑制" << "数字宽动态";
    comboBox9->addItems(list);
    list.clear();
    connect(comboBox9, static_cast<void (QComboBox:: *)(int)>(&QComboBox::currentIndexChanged), this, [this](int index){
        bool isShow = index != 0 ? true : false;
        static_cast<QSlider *>(imageMap["BLCValue"])->setVisible(isShow);
        static_cast<QLabel *>(imageMap["BLCValuetag"])->setVisible(isShow);
    });
    imageMap.insert("BLCMode", comboBox9);

    QSlider *slider6 = new QSlider(Qt::Horizontal, imageWidget);
    slider6->setRange(1, 100);
    slider6->setFixedWidth(100);
    QLabel *lbl20 = new QLabel(imageWidget);
    lbl20->setFixedWidth(20);
    imageMap.insert("BLCValuetag", lbl20);
    imageMap.insert("BLCValue", slider6);
    connect(slider6, &QSlider::valueChanged, this, [lbl20](int value){
        lbl20->setText(QString::number(value));
    });

    QLabel *lbl21 = new QLabel("昼夜模式", imageWidget);
    QComboBox *comboBox10 = new QComboBox(imageWidget);
    list << "全彩" << "全黑白" << "自动调节" << "时间调节";
    comboBox10->addItems(list);
    list.clear();
    connect(comboBox10, static_cast<void (QComboBox:: *)(int)>(&QComboBox::currentIndexChanged), this, [this](int index){
        bool isShow = index == 3 ? true : false;
        static_cast<QLabel *>(imageMap["time1tag"])->setVisible(isShow);
        static_cast<QTimeEdit *>(imageMap["time1"])->setVisible(isShow);
        static_cast<QLabel *>(imageMap["time2tag"])->setVisible(isShow);
        static_cast<QTimeEdit *>(imageMap["time2"])->setVisible(isShow);
    });
    imageMap.insert("Day/Night", comboBox10);

    QLabel *lbl22 = new QLabel("白昼开始点:", imageWidget);
    imageMap.insert("time1tag", lbl22);
    QTimeEdit *time1 = new QTimeEdit(imageWidget);
    imageMap.insert("time1", time1);
    QLabel *lbl23 = new QLabel("黑夜开始点:", imageWidget);
    imageMap.insert("time2tag", lbl23);
    QTimeEdit *time2 = new QTimeEdit(imageWidget);
    imageMap.insert("time2", time2);

    QPushButton *btn = new QPushButton("保存", imageWidget);
    connect(btn, &QPushButton::clicked, this, &MediaWidget::handlerPrepareData);

    QGridLayout *layout1 = new QGridLayout;
    layout1->addWidget(displayArea, 0, 0, 8, 2, Qt::AlignRight);

    layout1->addWidget(lbl1,    0, 2, 1, 1, Qt::AlignRight);
    layout1->addWidget(slider1, 0, 3, 1, 1);
    layout1->addWidget(lbl2,    0, 4, 1, 1);

    layout1->addWidget(lbl3,    1, 2, 1, 1, Qt::AlignRight);
    layout1->addWidget(slider2, 1, 3, 1, 1);
    layout1->addWidget(lbl4,    1, 4, 1, 1);

    layout1->addWidget(lbl5,    2, 2, 1, 1, Qt::AlignRight);
    layout1->addWidget(slider3, 2, 3, 1, 1);
    layout1->addWidget(lbl6,    2, 4, 1, 1);

    layout1->addWidget(lbl7,    3, 2, 1, 1, Qt::AlignRight);
    layout1->addWidget(slider4, 3, 3, 1, 1);
    layout1->addWidget(lbl8,    3, 4, 1, 1);

    layout1->addWidget(lbl9,    4, 2, 1, 1, Qt::AlignRight);
    layout1->addWidget(slider5, 4, 3, 1, 1);
    layout1->addWidget(lbl10,   4, 4, 1, 1);

    layout1->addWidget(lbl11,     5, 2, 1, 1, Qt::AlignRight);
    layout1->addWidget(comboBox1, 5, 3, 1, 2);

    layout1->addWidget(lbl12,     6, 2, 1, 1, Qt::AlignRight);
    layout1->addWidget(comboBox2, 6, 3, 1, 2);

    layout1->addWidget(lbl13,     7, 2, 1, 1, Qt::AlignRight);
    layout1->addWidget(comboBox3, 7, 3, 1, 2);

    layout1->addWidget(lbl14,     9, 0, 1, 1, Qt::AlignRight);
    layout1->addWidget(comboBox4, 9, 1, 1, 1);
    layout1->addWidget(lbl15,     9, 2, 1, 1, Qt::AlignRight);
    layout1->addWidget(comboBox5, 9, 3, 1, 2);

    layout1->addWidget(lbl16,     10, 0, 1, 1, Qt::AlignRight);
    layout1->addWidget(comboBox6, 10, 1, 1, 1);
    layout1->addWidget(lbl17,     10, 2, 1, 1, Qt::AlignRight);
    layout1->addWidget(comboBox7, 10, 3, 1, 2);

    layout1->addWidget(lbl18,     11, 0, 1, 1, Qt::AlignRight);
    layout1->addWidget(comboBox8, 11, 1, 1, 1);

    layout1->addWidget(lbl19,     12, 0, 1, 1, Qt::AlignRight);
    layout1->addWidget(comboBox9, 12, 1, 1, 1);

    layout1->addWidget(slider6,   13, 1, 1, 1, Qt::AlignLeft);
    layout1->addWidget(lbl20,     13, 1, 1, 1, Qt::AlignRight);

    layout1->addWidget(lbl21,     14, 0, 1, 1, Qt::AlignRight);
    layout1->addWidget(comboBox10,14, 1, 1, 1);

    layout1->addWidget(lbl22,     15, 0, 1, 1, Qt::AlignRight);
    layout1->addWidget(time1,     15, 1, 1, 1);

    layout1->addWidget(lbl23,     16, 0, 1, 1, Qt::AlignRight);
    layout1->addWidget(time2,     16, 1, 1, 1);

    layout1->addWidget(btn,       17, 0, 1, 5, Qt::AlignCenter);

    QVBoxLayout *layout2 = new QVBoxLayout;
    layout2->addLayout(layout1);
    layout2->addStretch();

    QHBoxLayout *layout3 = new QHBoxLayout(imageWidget);
    layout3->addStretch(1);
    layout3->addLayout(layout2, 10);
    layout3->addStretch(1);

    addWidget(imageWidget);
}

void MediaWidget::initROIWidget()
{
    QStringList list;
    roiWidget = new QWidget(this);

    QWidget *displayArea = new QWidget(roiWidget);
    displayArea->setFixedSize(400, 400);
    displayArea->setStyleSheet("background-color:black");
    ROIMap.insert("DisplayArea", displayArea);

    QRadioButton *radioBtn1 = new QRadioButton("使能(支持4个区域)", roiWidget);
    ROIMap.insert("Enable", radioBtn1);

    QLabel *lbl1 = new QLabel("模式", roiWidget);
    lbl1->setFixedWidth(50);
    QComboBox *comboBox = new QComboBox(roiWidget);
    comboBox->setFixedWidth(150);
    list << "清晰" << "模式";
    comboBox->addItems(list);
    list.clear();
    ROIMap.insert("Mode", comboBox);

    QPushButton *btn1 = new QPushButton("全屏", roiWidget);
    ROIMap.insert("FullScreen", btn1);

    QPushButton *btn2 = new QPushButton("清屏", roiWidget);
    ROIMap.insert("ClearScreen", btn2);

    QPushButton *btn3 = new QPushButton("保存", roiWidget);
    ROIMap.insert("Save", btn3);

    QHBoxLayout *layout1 = new QHBoxLayout;
    layout1->addStretch();
    layout1->addWidget(displayArea);
    layout1->addStretch();

    QHBoxLayout *layout2 = new QHBoxLayout;
    layout2->addWidget(radioBtn1);
    layout2->addStretch();

    QHBoxLayout *layout6 = new QHBoxLayout;
    layout6->addWidget(lbl1);
    layout6->addWidget(comboBox);
    layout6->addStretch();

    QHBoxLayout *layout3 = new QHBoxLayout;
    layout3->addWidget(btn1);
    layout3->addWidget(btn2);
    layout3->addWidget(btn3);

    QVBoxLayout *layout4 = new QVBoxLayout;
    layout4->addStretch();
    layout4->addLayout(layout1);
    layout4->addLayout(layout2);
    layout4->addLayout(layout6);
    layout4->addLayout(layout3);
    layout4->addStretch();

    QHBoxLayout *layout5 = new QHBoxLayout(roiWidget);
    layout5->addStretch();
    layout5->addLayout(layout4);
    layout5->addStretch();

    addWidget(roiWidget);
}

void MediaWidget::initOSDWidget()
{
    osdWidget = new QWidget(this);

    OSDWidget *displayArea = new OSDWidget(osdWidget);
    displayArea->setFixedSize(400, 400);
    displayArea->setStyleSheet("background-color:black");
    OSDMap.insert("DisplayArea", displayArea);

    QCheckBox *cb1 = new QCheckBox("显示时间", osdWidget);
    OSDMap.insert("Show Clock", cb1);

    QCheckBox *cb2 = new QCheckBox("显示帧率码率", osdWidget);
    OSDMap.insert("Show FPS", cb2);

    QCheckBox *cb3 = new QCheckBox("显示通道名称", osdWidget);
    OSDMap.insert("Show Channel Name", cb3);
    QLineEdit *lineEdit1 = new QLineEdit("cam00", osdWidget);
    //禁止调用输入法
    lineEdit1->setAttribute(Qt::WA_InputMethodEnabled, false);
    OSDMap.insert("Channel Name", lineEdit1);

    QCheckBox *cb4 = new QCheckBox("显示车号-车厢号-车位号", osdWidget);
    OSDMap.insert("Show User Info", cb4);
    QLineEdit *lineEdit2 = new QLineEdit(osdWidget);
    lineEdit2->setAttribute(Qt::WA_InputMethodEnabled, false);
    OSDMap.insert("User Info", lineEdit2);

    QPushButton *btn = new QPushButton("保存", osdWidget);
    btn->setFixedWidth(50);
    connect(btn, &QPushButton::clicked, this, &MediaWidget::handlerPrepareData);

    QGridLayout *layout1 = new QGridLayout;
    layout1->addWidget(displayArea, 0, 0, 8, 2);

    layout1->addWidget(cb1, 8, 0, 1, 1);
    layout1->addWidget(cb2, 9, 0, 1, 1);

    layout1->addWidget(cb3,     10, 0, 1, 1);
    layout1->addWidget(lineEdit1, 10, 1, 1, 1);

    layout1->addWidget(cb4,     11, 0, 1, 1);
    layout1->addWidget(lineEdit2, 11, 1, 1, 1);

    layout1->addWidget(btn, 12, 0, 1, 2, Qt::AlignCenter);

    QVBoxLayout *layout2 = new QVBoxLayout;
    layout2->addStretch();
    layout2->addLayout(layout1);
    layout2->addStretch();

    QHBoxLayout *layout3 = new QHBoxLayout(osdWidget);
    layout3->addStretch();
    layout3->addLayout(layout2);
    layout3->addStretch();

    addWidget(osdWidget);
}

void MediaWidget::refreshParameter()
{
    switch(currentIndex()){
    case 0:
        break;
    default:
        break;
    }
}

void MediaWidget::handlerSwitchTab(const QModelIndex &index)
{
    int type = index.row();
    if(sender() != this) {
        setCurrentIndex(index.row());
    }else {
        type = currentIndex();
    }

    switch(type){
    case 0: {
        emit signalGetParameter(VIDEOENCODINGPARAM, 0);
        emit signalGetParameter(VIDEOENCODINGPARAM, 1);
        emit signalGetParameter(AUDIOENCODINGPARAM);
        break;
    }
    case 2: {
        emit signalGetParameter(IMAGEPARAMETER);
    }
    case 4: {
        emit signalGetParameter(OSDPARAMETER);
    }
    default:
        break;
    }
}

void MediaWidget::handlerPrepareData()
{
    switch (currentIndex()) {
    case 0: {
        for(int i=0; i<2; i++) {
            VidiconProtocol::VideoEncodingParameter *param = new VidiconProtocol::VideoEncodingParameter;
            param->Channel = 0;
            param->StreamType = i;
            param->VideoCodecType = static_cast<QComboBox *>(audioVideoMap[QString("Codec %1").arg(i)])->currentText();
            QStringList list = static_cast<QComboBox *>(audioVideoMap[QString("Resolution %1").arg(i)])->currentText().split('*');
            param->VideoResolutionWidth = QString(list.at(0)).toInt();
            param->VideoResolutionHeight = QString(list.at(1)).toInt();
            param->VideoQualityControlType = static_cast<QComboBox *>(audioVideoMap[QString("Bitrate Mode %1").arg(i)])->currentText();
            param->ConstantBitRate = static_cast<QLineEdit *>(audioVideoMap[QString("Video Rate %1").arg(i)])->text().toInt();
            param->FixedQuality = static_cast<QComboBox *>(audioVideoMap[QString("Video quality %1").arg(i)])->currentIndex() + 1;
            param->FrameRate = static_cast<QComboBox *>(audioVideoMap[QString("Frame Rate %1").arg(i)])->currentText().toInt();
            param->SnapShotImageType = "JPEG";
            param->GovLength = static_cast<QLineEdit *>(audioVideoMap[QString("I Frame Interval %1").arg(i)])->text().toInt();

            emit signalSetParameter(VIDEOENCODINGPARAM, param);
            qDebug() << "#TabMedia# handlerSendData send signal, ParameterType:" << VIDEOENCODINGPARAM
                     << "StreamType:" << i;
        }
        VidiconProtocol::AudioEncodingParameter *param = new VidiconProtocol::AudioEncodingParameter;
        param->Enabled = static_cast<QRadioButton *>(audioVideoMap["Audio Enable"])->isChecked() ? 1 : 0;
        param->Encoding = static_cast<QComboBox *>(audioVideoMap["Audio Codec"])->currentText();
        param->Bitrate = 16000;
        param->SampleRate = 8000;
        emit signalSetParameter(AUDIOENCODINGPARAM, param);
        qDebug() << "#TabMedia# handlerSendData send signal, ParameterType:" << AUDIOENCODINGPARAM;

        break;
    }
    case 1: {
        VidiconProtocol::PrivacyMaskParameter *param = new VidiconProtocol::PrivacyMaskParameter[4];
        PrivacyWidget *w = static_cast<PrivacyWidget *>(privacyMap["DisplayArea"]);
        const QRect *rects = w->getRects();
        for(int i=0; i<4; i++) {
            param[i].Enabled = static_cast<QRadioButton *>(privacyMap["Enable"])->isChecked() ? 1 : 0;
            param[i].PosX = rects[i].topLeft().x() * XSCALEMAX / w->size().width();
            param[i].PosY = rects[i].topLeft().y() * YSCALEMAX / w->size().height();
            param[i].Width = rects[i].width() * XSCALEMAX / w->size().width();
            param[i].Height = rects[i].height() * YSCALEMAX / w->size().height();
        }
        emit signalSetParameter(PRIVACYPARAMETER, param);
        w->reset();
        qDebug() << "#TabMedia# handlerSendData send signal, ParameterType:" << PRIVACYPARAMETER;

        break;
    }
    case 2: {
        VidiconProtocol::ImageParameter *param = new VidiconProtocol::ImageParameter;
        param->HueLevel = static_cast<QSlider *>(imageMap["HueLevel"])->value();
        param->BrightnessLevel = static_cast<QSlider *>(imageMap["BrightnessLevel"])->value();
        param->ContrastLevel = static_cast<QSlider *>(imageMap["ContrastLevel"])->value();
        param->SaturationLevel = static_cast<QSlider *>(imageMap["SaturationLevel"])->value();
        param->Sharpness = static_cast<QSlider *>(imageMap["Sharpness"])->value();
        param->Mirror = static_cast<QComboBox *>(imageMap["Mirror"])->currentIndex();
        param->Turn = static_cast<QComboBox *>(imageMap["Turn"])->currentIndex();
        param->VisionMode = static_cast<QComboBox *>(imageMap["VisionMode"])->currentIndex() + 1;
        param->NoiseReduceMode = static_cast<QComboBox *>(imageMap["NoiseReduceMode"])->currentIndex();
        param->Shutter = static_cast<QComboBox *>(imageMap["Shutter"])->currentIndex();
        param->LDCEnabled = static_cast<QComboBox *>(imageMap["Fisheye correction"])->currentIndex();
        param->AntiFlashMode = static_cast<QComboBox *>(imageMap["AntiFlashMode"])->currentIndex();
        param->ExposureMode = static_cast<QComboBox *>(imageMap["ExposureMode"])->currentIndex();
        param->BLCMode = static_cast<QComboBox *>(imageMap["BLCMode"])->currentIndex();
        param->BLCIntensity = static_cast<QSlider *>(imageMap["BLCValue"])->value();
        param->WDRIntensity = static_cast<QSlider *>(imageMap["BLCValue"])->value();
        param->HLCIntensity = static_cast<QSlider *>(imageMap["BLCValue"])->value();
        param->DWDRIntensity = static_cast<QSlider *>(imageMap["BLCValue"])->value();
        param->IrcutFilterMode = static_cast<QComboBox *>(imageMap["Day/Night"])->currentIndex() + 1;
        param->HighLowLevel = 0;
        param->BeginTime = static_cast<QTimeEdit *>(imageMap["time1"])->time().toString("HH:MM");
        param->EndTime = static_cast<QTimeEdit *>(imageMap["time2"])->time().toString("HH:MM");

        emit signalSetParameter(IMAGEPARAMETER, param);
        qDebug() << "#TabMedia# handlerSendData send signal, ParameterType:" << IMAGEPARAMETER;

        break;
    }
    case 4: {
        VidiconProtocol::OSDParameter *param = static_cast<OSDWidget *>(OSDMap["DisplayArea"])->getOSDParameters();
        for(int i=0; i<4; i++) {
            switch(param[i].OSDType) {
            case 0: {
                param[i].Enabled = static_cast<QCheckBox *>(OSDMap["Show Channel Name"])->checkState() ? 1 : 0;
                param[i].OSDText = static_cast<QLineEdit *>(OSDMap["Channel Name"])->text();
                break;
            }
            case 1: {
                param[i].Enabled = static_cast<QCheckBox *>(OSDMap["Show Clock"])->checkState() ? 1 : 0;
                break;
            }
            case 2: {
                param[i].Enabled = static_cast<QCheckBox *>(OSDMap["Show FPS"])->checkState() ? 1 : 0;
                break;
            }
            case 3: {
                param[i].Enabled = static_cast<QCheckBox *>(OSDMap["Show User Info"])->checkState() ? 1 : 0;
                param[i].OSDText = static_cast<QLineEdit *>(OSDMap["User Info"])->text();
                break;
            }
            default:
                break;
            }
        }
        VidiconProtocol::OSDParameter *temp = new VidiconProtocol::OSDParameter[4];
        for(int i=0; i<4; i++) {
            temp[i] = param[i];
        }
        emit signalSetParameter(OSDPARAMETER, temp);
        qDebug() << "#TabMedia# handlerSendData send signal, ParameterType:" << OSDPARAMETER;
        break;
    }
    default:
        break;
    }
    WaitingShade::getInstance()->show();
}

void MediaWidget::handlerReceiveData(int type, QByteArray data)
{
    switch(type){
    case VIDEOENCODINGPARAM:{
        VidiconProtocol::VideoEncodingParameter param;
        if(ParseXML::getInstance()->parseVideoEncodingParameter(&param, data)) {
            static_cast<QComboBox *>(audioVideoMap[QString("Frame Rate %1").arg(param.StreamType)])->setCurrentText(QString::number(param.FrameRate));
            static_cast<QComboBox *>(audioVideoMap[QString("Codec %1").arg(param.StreamType)])->setCurrentText(param.VideoCodecType);
            static_cast<QComboBox *>(audioVideoMap[QString("Bitrate Mode %1").arg(param.StreamType)])->setCurrentText(param.VideoQualityControlType);
            static_cast<QComboBox *>(audioVideoMap[QString("Resolution %1").arg(param.StreamType)])->setCurrentText(QString("%1*%2").arg(param.VideoResolutionWidth).arg(param.VideoResolutionHeight));
            static_cast<QComboBox *>(audioVideoMap[QString("Video quality %1").arg(param.StreamType)])->setCurrentIndex(param.FixedQuality - 1);
            static_cast<QLineEdit *>(audioVideoMap[QString("Video Rate %1").arg(param.StreamType)])->setText(QString::number(param.ConstantBitRate));
            static_cast<QLineEdit *>(audioVideoMap[QString("I Frame Interval %1").arg(param.StreamType)])->setText(QString::number(param.GovLength));
            qDebug() << "#TabMedia# handlerReceiveData, ParameterType:" << type
                     << "StreamType:" << param.StreamType << "parse data success...";
        }else{
            qDebug() << "#TabMedia# handlerReceiveData, ParameterType:" << type
                     << "StreamType:" << param.StreamType << "parse data error...";
        }
        break;
    }
    case AUDIOENCODINGPARAM:{
        VidiconProtocol::AudioEncodingParameter param;
        if(ParseXML::getInstance()->parseAudioEncodingParameter(&param, data)) {
            static_cast<QRadioButton *>(audioVideoMap["Audio Enable"])->setChecked(param.Enabled == 0 ? Qt::Unchecked : Qt::Checked);
            static_cast<QComboBox *>(audioVideoMap["Audio Codec"])->setCurrentText(param.Encoding);
            qDebug() << "#TabMedia# handlerReceiveData, ParameterType:" << type << "parse data success...";
        }else{
            qDebug() << "#TabMedia# handlerReceiveData, ParameterType:" << type << "parse data error...";
        }
        break;
    }
    case OSDPARAMETER:{
        VidiconProtocol::OSDParameter *param = static_cast<OSDWidget *>(OSDMap["DisplayArea"])->getOSDParameters();
        if(ParseXML::getInstance()->parseOSDParameter(param, data)) {
            for(int i=0; i<4; i++) {
                switch(param[i].OSDType) {
                case 0: {
                    static_cast<QCheckBox *>(OSDMap["Show Channel Name"])->setChecked(param[i].Enabled ? true : false);
                    static_cast<QLineEdit *>(OSDMap["Channel Name"])->setText(param[i].OSDText);
                    break;
                }
                case 1: {
                    static_cast<QCheckBox *>(OSDMap["Show Clock"])->setChecked(param[i].Enabled ? true : false);
                    break;
                }
                case 2: {
                    static_cast<QCheckBox *>(OSDMap["Show FPS"])->setChecked(param[i].Enabled ? true : false);
                    break;
                }
                case 3: {
                    static_cast<QCheckBox *>(OSDMap["Show User Info"])->setChecked(param[i].Enabled ? true : false);
                    static_cast<QLineEdit *>(OSDMap["User Info"])->setText(param[i].OSDText);
                    break;
                }
                default:
                    break;
                }
            }
            qDebug() << "#TabMedia# handlerReceiveData, ParameterType:" << type << "parse data success...";
        }else{
            qDebug() << "#TabMedia# handlerReceiveData, ParameterType:" << type << "parse data error...";
        }
        break;
    }
    case IMAGEPARAMETER: {
        VidiconProtocol::ImageParameter param;
        if(ParseXML::getInstance()->parseImageParameter(&param, data)) {
            static_cast<QSlider *>(imageMap["HueLevel"])->setValue(param.HueLevel);
            static_cast<QSlider *>(imageMap["BrightnessLevel"])->setValue(param.BrightnessLevel);
            static_cast<QSlider *>(imageMap["ContrastLevel"])->setValue(param.ContrastLevel);
            static_cast<QSlider *>(imageMap["SaturationLevel"])->setValue(param.SaturationLevel);
            static_cast<QSlider *>(imageMap["Sharpness"])->setValue(param.Sharpness);
            static_cast<QComboBox *>(imageMap["Mirror"])->setCurrentIndex(param.Mirror);
            static_cast<QComboBox *>(imageMap["Turn"])->setCurrentIndex(param.Turn);
            static_cast<QComboBox *>(imageMap["VisionMode"])->setCurrentIndex(param.VisionMode - 1);
            static_cast<QComboBox *>(imageMap["NoiseReduceMode"])->setCurrentIndex(param.NoiseReduceMode);
            static_cast<QComboBox *>(imageMap["Shutter"])->setCurrentIndex(param.Shutter);
            static_cast<QComboBox *>(imageMap["Fisheye correction"])->setCurrentIndex(param.LDCEnabled);
            static_cast<QComboBox *>(imageMap["AntiFlashMode"])->setCurrentIndex(param.AntiFlashMode);
            static_cast<QComboBox *>(imageMap["ExposureMode"])->setCurrentIndex(param.ExposureMode);
            static_cast<QComboBox *>(imageMap["BLCMode"])->setCurrentIndex(param.BLCMode);
            switch(param.BLCMode) {
            case 1:
                static_cast<QSlider *>(imageMap["BLCValue"])->setValue(param.BLCIntensity);
                break;
            case 2:
                static_cast<QSlider *>(imageMap["BLCValue"])->setValue(param.WDRIntensity);
                break;
            case 3:
                static_cast<QSlider *>(imageMap["BLCValue"])->setValue(param.HLCIntensity);
                break;
            case 4:
                static_cast<QSlider *>(imageMap["BLCValue"])->setValue(param.DWDRIntensity);
                break;
            default:
                break;
            }

            static_cast<QComboBox *>(imageMap["Day/Night"])->setCurrentIndex(param.IrcutFilterMode - 1);
            static_cast<QTimeEdit *>(imageMap["time1"])->setTime(QTime(param.BeginTime.left(2).toInt(), param.BeginTime.right(2).toInt()));
            static_cast<QTimeEdit *>(imageMap["time2"])->setTime(QTime(param.EndTime.left(2).toInt(), param.EndTime.right(2).toInt()));

            qDebug() << "#TabMedia# handlerReceiveData, ParameterType:" << type << "parse data success...";
        }else{
            qDebug() << "#TabMedia# handlerReceiveData, ParameterType:" << type << "parse data error...";
        }
        break;
    }
    default:
        break;
    }
}
