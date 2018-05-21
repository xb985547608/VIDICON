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
#include "parsexml.h"
#include "DrawSubWidget/osdwidget.h"
#include <QCheckBox>
#include <QTimeEdit>
#include "DrawSubWidget/privacywidget.h"
#include "DrawSubWidget/imagebasewidget.h"

MediaWidget::MediaWidget(QWidget *parent) :
    StackedWidget(parent)
{
    initAudioVideoWidget();
    initPrivacyWidget();
    initImageWidget();
//    initROIWidget();
    initOSDWidget();

    setStyleSheet("QSlider{border-radius:10px}"
                  "QSlider::groove:horizontal {"
                  "border: -3px solid;"
                  "height: 14px;"
                  "left: 3px; right: 3px;}"
                  "QSlider::handle:horizontal {"
                  "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #fdfcfb, stop:1 #e2d1c3);"
                  "border: 1px solid #5c5c5c;"
                  "width: 16px;"
                  "margin: -2px 0;"
                  "border-radius: 8px;}"
                  "QSlider::add-page:horizontal{"
                  "background: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:1 #BDBDBD, stop:0 #DBDBDB);}"
                  "QSlider::sub-page:horizontal{"
                  "background: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 #868f96, stop:1 #596164);}");
}

MediaWidget::~MediaWidget()
{
}

void MediaWidget::initAudioVideoWidget()
{
    QStringList list;
    m_audioVideoWidget = new QWidget(this);

    QLabel *lbl1 = new QLabel("Standard", m_audioVideoWidget);
    QComboBox *comboBox1 = new QComboBox(m_audioVideoWidget);
    list << "PAL" << "NTSC";
    comboBox1->addItems(list);
    list.clear();
    m_audioVideoMap.insert("Standard 0", comboBox1);

    QLabel *lbl25 = new QLabel("Standard", m_audioVideoWidget);
    QComboBox *comboBox13 = new QComboBox(m_audioVideoWidget);
    list << "PAL" << "NTSC";
    comboBox13->addItems(list);
    list.clear();
    m_audioVideoMap.insert("Standard 1", comboBox13);

    QLabel *lbl2 = new QLabel("码流类型", m_audioVideoWidget);
    QLabel *lbl3 = new QLabel("主码流", m_audioVideoWidget);

    QLabel *lbl4 = new QLabel("码流类型", m_audioVideoWidget);
    QLabel *lbl5 = new QLabel("子码流", m_audioVideoWidget);


    QLabel *lbl6 = new QLabel("帧率", m_audioVideoWidget);
    QComboBox *comboBox2 = new QComboBox(m_audioVideoWidget);
    for(int i=1; i<=25; i++){
        list.append(QString::number(i));
    }
    comboBox2->addItems(list);
    list.clear();
    m_audioVideoMap.insert("Frame Rate 0", comboBox2);

    QLabel *lbl7 = new QLabel("帧率", m_audioVideoWidget);
    QComboBox *comboBox3 = new QComboBox(m_audioVideoWidget);
    for(int i=1; i<=25; i++){
        list.append(QString::number(i));
    }
    comboBox3->addItems(list);
    list.clear();
    m_audioVideoMap.insert("Frame Rate 1", comboBox3);

    QLabel *lbl8 = new QLabel("编码类型", m_audioVideoWidget);
    QComboBox *comboBox4 = new QComboBox(m_audioVideoWidget);
    list << "H264" << "H265";
    comboBox4->addItems(list);
    list.clear();
    m_audioVideoMap.insert("Codec 0", comboBox4);

    QLabel *lbl9 = new QLabel("编码类型", m_audioVideoWidget);
    QComboBox *comboBox5 = new QComboBox(m_audioVideoWidget);
    list << "H264" << "H265";
    comboBox5->addItems(list);
    list.clear();
    m_audioVideoMap.insert("Codec 1", comboBox5);

    QLabel *lbl10 = new QLabel("码流模式", m_audioVideoWidget);
    QComboBox *comboBox6 = new QComboBox(m_audioVideoWidget);
    list << "动态比特率" << "静态比特率";
    comboBox6->addItems(list);
    list.clear();
    m_audioVideoMap.insert("Bitrate Mode 0", comboBox6);

    QLabel *lbl11 = new QLabel("码流模式", m_audioVideoWidget);
    QComboBox *comboBox7 = new QComboBox(m_audioVideoWidget);
    list << "动态比特率" << "静态比特率";
    comboBox7->addItems(list);
    list.clear();
    m_audioVideoMap.insert("Bitrate Mode 1", comboBox7);

    QLabel *lbl12 = new QLabel("分辨率", m_audioVideoWidget);
    QComboBox *comboBox8 = new QComboBox(m_audioVideoWidget);
    list << "1920*1080" << "1280*720";
    comboBox8->addItems(list);
    list.clear();
    m_audioVideoMap.insert("Resolution 0", comboBox8);

    QLabel *lbl13 = new QLabel("分辨率", m_audioVideoWidget);
    QComboBox *comboBox9 = new QComboBox(m_audioVideoWidget);
    list << "352*288" << "704*576";
    comboBox9->addItems(list);
    list.clear();
    m_audioVideoMap.insert("Resolution 1", comboBox9);

    QLabel *lbl14 = new QLabel("视频质量", m_audioVideoWidget);
    QComboBox *comboBox10 = new QComboBox(m_audioVideoWidget);
    list << "差" << "正常" << "好" << "更好" << "最好";
    comboBox10->addItems(list);
    list.clear();
    m_audioVideoMap.insert("Video quality 0", comboBox10);

    QLabel *lbl15 = new QLabel("视频质量", m_audioVideoWidget);
    QComboBox *comboBox11 = new QComboBox(m_audioVideoWidget);
    list << "差" << "正常" << "好" << "更好" << "最好";
    comboBox11->addItems(list);
    list.clear();
    m_audioVideoMap.insert("Video quality 1", comboBox11);

    QLabel *lbl16 = new QLabel("码流大小", m_audioVideoWidget);
    LineEdit *lineEdit1 = new LineEdit(m_audioVideoWidget);
    lineEdit1->setValidator(new QIntValidator(36, 10240, this));
    connect(lineEdit1, &LineEdit::textChanged, this, &MediaWidget::checkInput);
    QLabel *lbl17 = new QLabel("(Kbit)(36-10240)", m_audioVideoWidget);
    m_audioVideoMap.insert("Video Rate 0", lineEdit1);

    QLabel *lbl18 = new QLabel("码流大小", m_audioVideoWidget);
    LineEdit *lineEdit2 = new LineEdit(m_audioVideoWidget);
    lineEdit2->setValidator(new QIntValidator(20, 2048, this));
    connect(lineEdit2, &LineEdit::textChanged, this, &MediaWidget::checkInput);
    QLabel *lbl19 = new QLabel("(Kbit)(20-2048)", m_audioVideoWidget);
    m_audioVideoMap.insert("Video Rate 1", lineEdit2);

    QLabel *lbl20 = new QLabel("I帧间隔", m_audioVideoWidget);
    LineEdit *lineEdit3 = new LineEdit(m_audioVideoWidget);
    lineEdit3->setValidator(new QIntValidator(1, 150, this));
    connect(lineEdit3, &LineEdit::textChanged, this, &MediaWidget::checkInput);
    QLabel *lbl21 = new QLabel("(FPS)(1-150)", m_audioVideoWidget);
    m_audioVideoMap.insert("I Frame Interval 0", lineEdit3);

    QLabel *lbl22 = new QLabel("I帧间隔", m_audioVideoWidget);
    LineEdit *lineEdit4 = new LineEdit(m_audioVideoWidget);
    lineEdit4->setValidator(new QIntValidator(1, 150, this));
    connect(lineEdit4, &LineEdit::textChanged, this, &MediaWidget::checkInput);
    QLabel *lbl23 = new QLabel("(FPS)(1-150)", m_audioVideoWidget);
    m_audioVideoMap.insert("I Frame Interval 1", lineEdit4);

    QRadioButton *radioBtn = new QRadioButton("启用音频", m_audioVideoWidget);
    m_audioVideoMap.insert("Audio Enable", radioBtn);

    QLabel *lbl24 = new QLabel("编码类型", m_audioVideoWidget);
    QComboBox *comboBox12 = new QComboBox(m_audioVideoWidget);
    list << "G711A" << "G711U" /*<< "G726" << "AAC"*/;
    comboBox12->addItems(list);
    list.clear();
    m_audioVideoMap.insert("Audio Codec", comboBox12);

    QPushButton *btn = new QPushButton("保存", m_audioVideoWidget);
    connect(btn, &QPushButton::clicked, this, &MediaWidget::handlePrepareData);
    m_audioVideoMap.insert("save", btn);

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

    setAlignment(m_audioVideoWidget, layout, Qt::AlignTop | Qt::AlignHCenter);
    addWidget(m_audioVideoWidget);
}

void MediaWidget::initPrivacyWidget()
{
    m_privacyWidget = new QWidget(this);

    PrivacyWidget *displayArea = new PrivacyWidget(m_privacyWidget);
    displayArea->setFixedSize(400, 400);
    displayArea->setStyleSheet("background-color:black");
    m_privacyMap.insert("DisplayArea", displayArea);

    QRadioButton *radioBtn1 = new QRadioButton("使能(支持4个区域)", m_privacyWidget);
    m_privacyMap.insert("Enable", radioBtn1);

    QPushButton *btn1 = new QPushButton("全屏", m_privacyWidget);
    connect(btn1, &QPushButton::clicked, displayArea, &PrivacyWidget::fullScreen);
    m_privacyMap.insert("FullScreen", btn1);

    QPushButton *btn2 = new QPushButton("清屏", m_privacyWidget);
    connect(btn2, &QPushButton::clicked, displayArea, &PrivacyWidget::clearScreen);
    m_privacyMap.insert("ClearScreen", btn2);

    QPushButton *btn3 = new QPushButton("保存", m_privacyWidget);
    m_privacyMap.insert("Save", btn3);
    connect(btn3, &QPushButton::clicked, this, &MediaWidget::handlePrepareData);

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
    layout4->addStretch();
    layout4->addLayout(layout1);
    layout4->addLayout(layout2);
    layout4->addLayout(layout3);
    layout4->addStretch();

    setAlignment(m_privacyWidget, layout4, Qt::AlignTop | Qt::AlignHCenter);
    addWidget(m_privacyWidget);
}

void MediaWidget::initImageWidget()
{
    QStringList list;
    m_imageWidget = new QWidget(this);

    ImageBaseWidget *displayArea = new ImageBaseWidget(m_imageWidget);
    displayArea->setFixedSize(400, 400);
    displayArea->setStyleSheet("background-color:black");
    m_imageMap.insert("DisplayArea", displayArea);

    QLabel *lbl1 = new QLabel("色度：",m_imageWidget);
    QSlider *slider1 = new QSlider(Qt::Horizontal, m_imageWidget);
    slider1->setRange(1, 100);
    slider1->setFixedWidth(150);
    QLabel *lbl2 = new QLabel(m_imageWidget);
    lbl2->setFixedWidth(20);
    m_imageMap.insert("HueLevel", slider1);
    connect(slider1, &QSlider::valueChanged, this, [lbl2](int value){
        lbl2->setText(QString::number(value));
    });

    QLabel *lbl3 = new QLabel("亮度：",m_imageWidget);
    QSlider *slider2 = new QSlider(Qt::Horizontal, m_imageWidget);
    slider2->setRange(1, 100);
    slider2->setFixedWidth(150);
    QLabel *lbl4 = new QLabel(m_imageWidget);
    m_imageMap.insert("BrightnessLevel", slider2);
    connect(slider2, &QSlider::valueChanged, this, [lbl4](int value){
        lbl4->setText(QString::number(value));
    });

    QLabel *lbl5 = new QLabel("对比度：",m_imageWidget);
    QSlider *slider3 = new QSlider(Qt::Horizontal, m_imageWidget);
    slider3->setRange(1, 100);
    slider3->setFixedWidth(150);
    QLabel *lbl6 = new QLabel(m_imageWidget);
    m_imageMap.insert("ContrastLevel", slider3);
    connect(slider3, &QSlider::valueChanged, this, [lbl6](int value){
        lbl6->setText(QString::number(value));
    });

    QLabel *lbl7 = new QLabel("饱和度：",m_imageWidget);
    QSlider *slider4 = new QSlider(Qt::Horizontal, m_imageWidget);
    slider4->setRange(1, 100);
    slider4->setFixedWidth(150);
    QLabel *lbl8 = new QLabel(m_imageWidget);
    m_imageMap.insert("SaturationLevel", slider4);
    connect(slider4, &QSlider::valueChanged, this, [lbl8](int value){
        lbl8->setText(QString::number(value));
    });

    QLabel *lbl9 = new QLabel("锐度：",m_imageWidget);
    QSlider *slider5 = new QSlider(Qt::Horizontal, m_imageWidget);
    slider5->setRange(1, 100);
    slider5->setFixedWidth(150);
    QLabel *lbl10 = new QLabel(m_imageWidget);
    m_imageMap.insert("Sharpness", slider5);
    connect(slider5, &QSlider::valueChanged, this, [lbl10](int value){
        lbl10->setText(QString::number(value));
    });

    QLabel *lbl11 = new QLabel("镜像：", m_imageWidget);
    QComboBox *comboBox1 = new QComboBox(m_imageWidget);
    list << "左" << "右";
    comboBox1->addItems(list);
    list.clear();
    m_imageMap.insert("Mirror", comboBox1);

    QLabel *lbl12 = new QLabel("翻转：", m_imageWidget);
    QComboBox *comboBox2 = new QComboBox(m_imageWidget);
    list << "上" << "下";
    comboBox2->addItems(list);
    list.clear();
    m_imageMap.insert("Turn", comboBox2);

    QLabel *lbl13 = new QLabel("视角：", m_imageWidget);
    QComboBox *comboBox3 = new QComboBox(m_imageWidget);
    list << "正常" << "顺时针旋转90" << "顺时针旋转270" << "顺时针旋转180";
    comboBox3->addItems(list);
    list.clear();
    m_imageMap.insert("VisionMode", comboBox3);

    QLabel *lbl14 = new QLabel("3D降噪：", m_imageWidget);
    QComboBox *comboBox4 = new QComboBox(m_imageWidget);
    list << "失能" << "使能";
    comboBox4->addItems(list);
    list.clear();
    m_imageMap.insert("NoiseReduceMode", comboBox4);

    lbl14->hide();
    comboBox4->hide();

    QLabel *lbl15 = new QLabel("快门调节：", m_imageWidget);
    QComboBox *comboBox5 = new QComboBox(m_imageWidget);
    list << "1/25" << "1/30" << "1/50" << "1/60" << "1/100" << "1/120" << "1/250" << "1/500" << "1/1000"
         << "1/2000" << "1/4000" << "1/10000" << "1/30000" << "1/100000";
    comboBox5->addItems(list);
    list.clear();
    m_imageMap.insert("Shutter", comboBox5);

    lbl15->hide();
    comboBox5->hide();

    QLabel *lbl16 = new QLabel("鱼眼矫正：", m_imageWidget);
    QComboBox *comboBox6 = new QComboBox(m_imageWidget);
    list << "失能" << "使能";
    comboBox6->addItems(list);
    list.clear();
    m_imageMap.insert("Fisheye correction", comboBox6);

    lbl16->hide();
    comboBox6->hide();

    QLabel *lbl17 = new QLabel("防闪烁：", m_imageWidget);
    QComboBox *comboBox7 = new QComboBox(m_imageWidget);
    list << "失能" << "使能";
    comboBox7->addItems(list);
    list.clear();
    m_imageMap.insert("AntiFlashMode", comboBox7);

    QLabel *lbl18 = new QLabel("曝光：", m_imageWidget);
    QComboBox *comboBox8 = new QComboBox(m_imageWidget);
    list << "自动" << "手动";
    comboBox8->addItems(list);
    list.clear();
    m_imageMap.insert("ExposureMode", comboBox8);

    lbl18->hide();
    comboBox8->hide();

    QLabel *lbl19 = new QLabel("背光补偿：", m_imageWidget);
    QComboBox *comboBox9 = new QComboBox(m_imageWidget);
    list << "失能" << "背光补偿" << "宽动态" << "强光抑制" << "数字宽动态";
    comboBox9->addItems(list);
    list.clear();
    connect(comboBox9, static_cast<void (QComboBox:: *)(int)>(&QComboBox::currentIndexChanged), this, [this](int /*index*/){
        bool isShow = /*index != 0 ? true :*/ false;
        static_cast<QSlider *>(m_imageMap["BLCValue"])->setVisible(isShow);
        static_cast<QLabel *>(m_imageMap["BLCValuetag"])->setVisible(isShow);
    });
    m_imageMap.insert("BLCMode", comboBox9);

    lbl19->hide();
    comboBox9->hide();

    QSlider *slider6 = new QSlider(Qt::Horizontal, m_imageWidget);
    slider6->setRange(1, 100);
    slider6->setFixedWidth(150);
    QLabel *lbl20 = new QLabel(m_imageWidget);
    lbl20->setFixedWidth(20);
    m_imageMap.insert("BLCValuetag", lbl20);
    m_imageMap.insert("BLCValue", slider6);
    connect(slider6, &QSlider::valueChanged, this, [lbl20](int value){
        lbl20->setText(QString::number(value));
    });

    slider6->hide();

    QLabel *lbl21 = new QLabel("昼夜模式", m_imageWidget);
    QComboBox *comboBox10 = new QComboBox(m_imageWidget);
    list << "全彩" << "全黑白" << "自动调节" << "时间调节";
    comboBox10->addItems(list);
    list.clear();
    connect(comboBox10, static_cast<void (QComboBox:: *)(int)>(&QComboBox::currentIndexChanged), this, [this](int index){
        bool isShow = index == 3 ? true : false;
        static_cast<QLabel *>(m_imageMap["time1tag"])->setEnabled(isShow);
        static_cast<QTimeEdit *>(m_imageMap["time1"])->setEnabled(isShow);
        static_cast<QLabel *>(m_imageMap["time2tag"])->setEnabled(isShow);
        static_cast<QTimeEdit *>(m_imageMap["time2"])->setEnabled(isShow);
    });
    m_imageMap.insert("Day/Night", comboBox10);

    QLabel *lbl22 = new QLabel("白昼开始点:", m_imageWidget);
    m_imageMap.insert("time1tag", lbl22);
    QTimeEdit *time1 = new QTimeEdit(m_imageWidget);
    m_imageMap.insert("time1", time1);
    QLabel *lbl23 = new QLabel("黑夜开始点:", m_imageWidget);
    m_imageMap.insert("time2tag", lbl23);
    QTimeEdit *time2 = new QTimeEdit(m_imageWidget);
    m_imageMap.insert("time2", time2);

    comboBox10->setCurrentIndex(1);

    QPushButton *btn = new QPushButton("保存", m_imageWidget);
    connect(btn, &QPushButton::clicked, this, &MediaWidget::handlePrepareData);

    QGridLayout *layout1 = new QGridLayout;
    layout1->addWidget(displayArea, 0, 0, 12, 2, Qt::AlignRight);

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
    layout1->addWidget(lbl17,     8, 2, 1, 1, Qt::AlignRight);
    layout1->addWidget(comboBox7, 8, 3, 1, 2);

    layout1->addWidget(lbl18,     11, 0, 1, 1, Qt::AlignRight);
    layout1->addWidget(comboBox8, 11, 1, 1, 1);

    layout1->addWidget(lbl19,     12, 0, 1, 1, Qt::AlignRight);
    layout1->addWidget(comboBox9, 12, 1, 1, 1);

    layout1->addWidget(slider6,   13, 1, 1, 1, Qt::AlignLeft);
    layout1->addWidget(lbl20,     13, 1, 1, 1, Qt::AlignRight);

    layout1->addWidget(lbl21,     9, 2, 1, 1, Qt::AlignRight);
    layout1->addWidget(comboBox10,9, 3, 1, 2);

    layout1->addWidget(lbl22,     10, 2, 1, 1, Qt::AlignRight);
    layout1->addWidget(time1,     10, 3, 1, 2);
    layout1->addWidget(lbl23,     11, 2, 1, 1, Qt::AlignRight);
    layout1->addWidget(time2,     11, 3, 1, 2);

    layout1->addWidget(btn,       12, 0, 1, 5, Qt::AlignCenter);

    setAlignment(m_imageWidget, layout1, Qt::AlignTop | Qt::AlignHCenter);
    addWidget(m_imageWidget);
}

void MediaWidget::initROIWidget()
{
    QStringList list;
    m_roiWidget = new QWidget(this);

    QWidget *displayArea = new QWidget(m_roiWidget);
    displayArea->setFixedSize(400, 400);
    displayArea->setStyleSheet("background-color:black");
    m_ROIMap.insert("DisplayArea", displayArea);

    QRadioButton *radioBtn1 = new QRadioButton("使能(支持4个区域)", m_roiWidget);
    m_ROIMap.insert("Enable", radioBtn1);

    QLabel *lbl1 = new QLabel("模式", m_roiWidget);
    lbl1->setFixedWidth(50);
    QComboBox *comboBox = new QComboBox(m_roiWidget);
    comboBox->setFixedWidth(150);
    list << "清晰" << "模式";
    comboBox->addItems(list);
    list.clear();
    m_ROIMap.insert("Mode", comboBox);

    QPushButton *btn1 = new QPushButton("全屏", m_roiWidget);
    m_ROIMap.insert("FullScreen", btn1);

    QPushButton *btn2 = new QPushButton("清屏", m_roiWidget);
    m_ROIMap.insert("ClearScreen", btn2);

    QPushButton *btn3 = new QPushButton("保存", m_roiWidget);
    m_ROIMap.insert("Save", btn3);

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

    setAlignment(m_roiWidget, layout4, Qt::AlignTop | Qt::AlignHCenter);
    addWidget(m_roiWidget);
}

void MediaWidget::initOSDWidget()
{
    m_osdWidget = new QWidget(this);

    OSDWidget *displayArea = new OSDWidget(m_osdWidget);
    displayArea->setFixedSize(400, 400);
    displayArea->setStyleSheet("background-color:black");
    m_OSDMap.insert("DisplayArea", displayArea);

    QCheckBox *cb1 = new QCheckBox("显示时间", m_osdWidget);
    m_OSDMap.insert("Show Clock", cb1);

    QCheckBox *cb2 = new QCheckBox("显示帧率码率", m_osdWidget);
    m_OSDMap.insert("Show FPS", cb2);

    QCheckBox *cb3 = new QCheckBox("显示通道名称", m_osdWidget);
    m_OSDMap.insert("Show Channel Name", cb3);
    LineEdit *lineEdit1 = new LineEdit("cam00", m_osdWidget);
    m_OSDMap.insert("Channel Name", lineEdit1);

    QCheckBox *cb4 = new QCheckBox("显示车号-车厢号-车位号", m_osdWidget);
    m_OSDMap.insert("Show User Info", cb4);
    LineEdit *lineEdit2 = new LineEdit(m_osdWidget);
    lineEdit2->setAttribute(Qt::WA_InputMethodEnabled, false);
    m_OSDMap.insert("User Info", lineEdit2);

    QPushButton *btn = new QPushButton("保存", m_osdWidget);
    btn->setFixedWidth(50);
    connect(btn, &QPushButton::clicked, this, &MediaWidget::handlePrepareData);

    QGridLayout *layout1 = new QGridLayout;
    layout1->addWidget(displayArea, 0, 0, 8, 2);

    layout1->addWidget(cb1, 8, 0, 1, 1);
    layout1->addWidget(cb2, 9, 0, 1, 1);

    layout1->addWidget(cb3,     10, 0, 1, 1);
    layout1->addWidget(lineEdit1, 10, 1, 1, 1);

    layout1->addWidget(cb4,     11, 0, 1, 1);
    layout1->addWidget(lineEdit2, 11, 1, 1, 1);

    layout1->addWidget(btn, 12, 0, 1, 2, Qt::AlignCenter);

    setAlignment(m_osdWidget, layout1, Qt::AlignTop | Qt::AlignHCenter);
    addWidget(m_osdWidget);
}

void MediaWidget::setCurrentIndex(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    m_notPass.clear();

    switch(index.row()){
    case 0: {
        VideoBasic param1;
        param1.Channel = 0;
        param1.StreamType = 0;
        emit signalGetParameter(VidiconProtocol::VIDEOENCODING, QVariant::fromValue(param1));
        VideoBasic param2;
        param2.Channel = 0;
        param2.StreamType = 1;
        emit signalGetParameter(VidiconProtocol::VIDEOENCODING, QVariant::fromValue(param2));
        emit signalGetParameter(VidiconProtocol::AUDIOENCODING);
        break;
    }
    case 2: {
        emit signalGetParameter(VidiconProtocol::IMAGE);
    }
    case 3: {
        emit signalGetParameter(VidiconProtocol::OSD);
    }
    default:
        break;
    }

    StackedWidget::setCurrentIndex(index.row());
}

void MediaWidget::handlePrepareData()
{
    switch (currentIndex()) {
    case 0: {
        for(int i=0; i<2; i++) {
            VideoEncodingParameter param;
            param.Channel = 0;
            param.StreamType = i;
            param.VideoCodecType = static_cast<QComboBox *>(m_audioVideoMap[QString("Codec %1").arg(i)])->currentText();
            QStringList list = static_cast<QComboBox *>(m_audioVideoMap[QString("Resolution %1").arg(i)])->currentText().split('*');
            param.VideoResolutionWidth = QString(list.at(0)).toInt();
            param.VideoResolutionHeight = QString(list.at(1)).toInt();
            param.VideoQualityControlType = static_cast<QComboBox *>(m_audioVideoMap[QString("Bitrate Mode %1").arg(i)])->currentText();
            param.ConstantBitRate = static_cast<LineEdit *>(m_audioVideoMap[QString("Video Rate %1").arg(i)])->text().toInt();
            param.FixedQuality = static_cast<QComboBox *>(m_audioVideoMap[QString("Video quality %1").arg(i)])->currentIndex() + 1;
            param.FrameRate = static_cast<QComboBox *>(m_audioVideoMap[QString("Frame Rate %1").arg(i)])->currentText().toInt();
            param.SnapShotImageType = "JPEG";
            param.GovLength = static_cast<LineEdit *>(m_audioVideoMap[QString("I Frame Interval %1").arg(i)])->text().toInt();

            emit signalSetParameter(VidiconProtocol::VIDEOENCODING, QVariant::fromValue(param));
        }
        AudioEncodingParameter param;
        param.Enabled = static_cast<QRadioButton *>(m_audioVideoMap["Audio Enable"])->isChecked() ? 1 : 0;
        param.Encoding = static_cast<QComboBox *>(m_audioVideoMap["Audio Codec"])->currentText();
        param.Bitrate = 16000;
        param.SampleRate = 8000;
        emit signalSetParameter(VidiconProtocol::AUDIOENCODING, QVariant::fromValue(param));

        break;
    }
    case 1: {
        QList<PrivacyMaskParameter> param;
        PrivacyWidget *w = static_cast<PrivacyWidget *>(m_privacyMap["DisplayArea"]);
        const QRect *rects = w->getRects();
        for(int i=0; i<4; i++) {
            PrivacyMaskParameter temp;
            temp.Enabled = static_cast<QRadioButton *>(m_privacyMap["Enable"])->isChecked() ? 1 : 0;
            temp.PosX   = rects[i].topLeft().x() * XSCALEMAX / w->size().width();
            temp.PosY   = rects[i].topLeft().y() * YSCALEMAX / w->size().height();
            temp.Width  = rects[i].width() * XSCALEMAX / w->size().width();
            temp.Height = rects[i].height() * YSCALEMAX / w->size().height();
            temp.PosX   = qBound(0, temp.PosX  , 1000);
            temp.PosY   = qBound(0, temp.PosY  , 1000);
            temp.Width  = qBound(0, temp.Width , 1000);
            temp.Height = qBound(0, temp.Height, 1000);
            param.append(temp);
        }
        emit signalSetParameter(VidiconProtocol::PRIVACY, QVariant::fromValue(param));
        w->reset();

        break;
    }
    case 2: {
        ImageParameter param;
        param.HueLevel = static_cast<QSlider *>(m_imageMap["HueLevel"])->value();
        param.BrightnessLevel = static_cast<QSlider *>(m_imageMap["BrightnessLevel"])->value();
        param.ContrastLevel = static_cast<QSlider *>(m_imageMap["ContrastLevel"])->value();
        param.SaturationLevel = static_cast<QSlider *>(m_imageMap["SaturationLevel"])->value();
        param.Sharpness = static_cast<QSlider *>(m_imageMap["Sharpness"])->value();
        param.Mirror = static_cast<QComboBox *>(m_imageMap["Mirror"])->currentIndex();
        param.Turn = static_cast<QComboBox *>(m_imageMap["Turn"])->currentIndex();
        param.VisionMode = static_cast<QComboBox *>(m_imageMap["VisionMode"])->currentIndex() + 1;
        param.NoiseReduceMode = static_cast<QComboBox *>(m_imageMap["NoiseReduceMode"])->currentIndex();
        param.Shutter = static_cast<QComboBox *>(m_imageMap["Shutter"])->currentIndex();
        param.LDCEnabled = static_cast<QComboBox *>(m_imageMap["Fisheye correction"])->currentIndex();
        param.AntiFlashMode = static_cast<QComboBox *>(m_imageMap["AntiFlashMode"])->currentIndex();
        param.ExposureMode = static_cast<QComboBox *>(m_imageMap["ExposureMode"])->currentIndex();
        param.BLCMode = static_cast<QComboBox *>(m_imageMap["BLCMode"])->currentIndex();
        param.BLCIntensity = static_cast<QSlider *>(m_imageMap["BLCValue"])->value();
        param.WDRIntensity = static_cast<QSlider *>(m_imageMap["BLCValue"])->value();
        param.HLCIntensity = static_cast<QSlider *>(m_imageMap["BLCValue"])->value();
        param.DWDRIntensity = static_cast<QSlider *>(m_imageMap["BLCValue"])->value();
        param.IrcutFilterMode = static_cast<QComboBox *>(m_imageMap["Day/Night"])->currentIndex() + 1;
        param.HighLowLevel = 0;
        param.BeginTime = static_cast<QTimeEdit *>(m_imageMap["time1"])->time();
        param.EndTime = static_cast<QTimeEdit *>(m_imageMap["time2"])->time();

        emit signalSetParameter(VidiconProtocol::IMAGE, QVariant::fromValue(param));

        break;
    }
    case 3: {
        QList<OSDParameter> &param = static_cast<OSDWidget *>(m_OSDMap["DisplayArea"])->getOSDParameters();
        for(int i=0; i<param.count(); i++) {
            switch(param[i].OSDType) {
            case 0: {
                param[i].Enabled = static_cast<QCheckBox *>(m_OSDMap["Show Channel Name"])->checkState() ? 1 : 0;
                param[i].OSDText = static_cast<LineEdit *>(m_OSDMap["Channel Name"])->text();
                break;
            }
            case 1: {
                param[i].Enabled = static_cast<QCheckBox *>(m_OSDMap["Show Clock"])->checkState() ? 1 : 0;
                break;
            }
            case 2: {
                param[i].Enabled = static_cast<QCheckBox *>(m_OSDMap["Show FPS"])->checkState() ? 1 : 0;
                break;
            }
            case 3: {
                param[i].Enabled = static_cast<QCheckBox *>(m_OSDMap["Show User Info"])->checkState() ? 1 : 0;
                param[i].OSDText = static_cast<LineEdit *>(m_OSDMap["User Info"])->text();
                break;
            }
            default:
                break;
            }
        }
        emit signalSetParameter(VidiconProtocol::OSD, QVariant::fromValue(param));
        break;
    }
    default:
        break;
    }
}

void MediaWidget::handleReceiveData(VidiconProtocol::Type type, QByteArray data)
{
    bool isOK = false;

    switch(type){
    case VidiconProtocol::VIDEOENCODING:{
        VideoEncodingParameter param;

        isOK = ParseXML::getInstance()->parseVideoEncodingParameter(&param, data);
        if (isOK) {
            static_cast<QComboBox *>(m_audioVideoMap[QString("Frame Rate %1").arg(param.StreamType)])->setCurrentText(QString::number(param.FrameRate));
            static_cast<QComboBox *>(m_audioVideoMap[QString("Codec %1").arg(param.StreamType)])->setCurrentText(param.VideoCodecType);
            static_cast<QComboBox *>(m_audioVideoMap[QString("Bitrate Mode %1").arg(param.StreamType)])->setCurrentText(param.VideoQualityControlType);
            static_cast<QComboBox *>(m_audioVideoMap[QString("Resolution %1").arg(param.StreamType)])->setCurrentText(QString("%1*%2").arg(param.VideoResolutionWidth).arg(param.VideoResolutionHeight));
            static_cast<QComboBox *>(m_audioVideoMap[QString("Video quality %1").arg(param.StreamType)])->setCurrentIndex(param.FixedQuality - 1);
            static_cast<LineEdit *>(m_audioVideoMap[QString("Video Rate %1").arg(param.StreamType)])->setText(QString::number(param.ConstantBitRate));
            static_cast<LineEdit *>(m_audioVideoMap[QString("I Frame Interval %1").arg(param.StreamType)])->setText(QString::number(param.GovLength));
        }
        break;
    }
    case VidiconProtocol::AUDIOENCODING:{
        AudioEncodingParameter param;

        isOK = ParseXML::getInstance()->parseAudioEncodingParameter(&param, data);
        if (isOK) {
            static_cast<QRadioButton *>(m_audioVideoMap["Audio Enable"])->setChecked(param.Enabled == 0 ? Qt::Unchecked : Qt::Checked);
            static_cast<QComboBox *>(m_audioVideoMap["Audio Codec"])->setCurrentText(param.Encoding);
        }
        break;
    }
    case VidiconProtocol::OSD:{
        QList<OSDParameter> &param = static_cast<OSDWidget *>(m_OSDMap["DisplayArea"])->getOSDParameters();

        isOK = ParseXML::getInstance()->parseOSDParameter(param, data);
        if (isOK) {
            for(int i=0; i<param.count(); i++) {
                switch(param[i].OSDType) {
                case 0: {
                    static_cast<QCheckBox *>(m_OSDMap["Show Channel Name"])->setChecked(param[i].Enabled ? true : false);
                    static_cast<LineEdit *>(m_OSDMap["Channel Name"])->setText(param[i].OSDText);
                    break;
                }
                case 1: {
                    static_cast<QCheckBox *>(m_OSDMap["Show Clock"])->setChecked(param[i].Enabled ? true : false);
                    break;
                }
                case 2: {
                    static_cast<QCheckBox *>(m_OSDMap["Show FPS"])->setChecked(param[i].Enabled ? true : false);
                    break;
                }
                case 3: {
                    static_cast<QCheckBox *>(m_OSDMap["Show User Info"])->setChecked(param[i].Enabled ? true : false);
                    static_cast<LineEdit *>(m_OSDMap["User Info"])->setText(param[i].OSDText);
                    break;
                }
                default:
                    break;
                }
            }
        }
        break;
    }
    case VidiconProtocol::IMAGE: {
        ImageParameter param;

        isOK = ParseXML::getInstance()->parseImageParameter(&param, data);
        if (isOK) {
            static_cast<QSlider *>(m_imageMap["HueLevel"])->setValue(param.HueLevel);
            static_cast<QSlider *>(m_imageMap["BrightnessLevel"])->setValue(param.BrightnessLevel);
            static_cast<QSlider *>(m_imageMap["ContrastLevel"])->setValue(param.ContrastLevel);
            static_cast<QSlider *>(m_imageMap["SaturationLevel"])->setValue(param.SaturationLevel);
            static_cast<QSlider *>(m_imageMap["Sharpness"])->setValue(param.Sharpness);
            static_cast<QComboBox *>(m_imageMap["Mirror"])->setCurrentIndex(param.Mirror);
            static_cast<QComboBox *>(m_imageMap["Turn"])->setCurrentIndex(param.Turn);
            static_cast<QComboBox *>(m_imageMap["VisionMode"])->setCurrentIndex(param.VisionMode - 1);
            static_cast<QComboBox *>(m_imageMap["NoiseReduceMode"])->setCurrentIndex(param.NoiseReduceMode);
            static_cast<QComboBox *>(m_imageMap["Shutter"])->setCurrentIndex(param.Shutter);
            static_cast<QComboBox *>(m_imageMap["Fisheye correction"])->setCurrentIndex(param.LDCEnabled);
            static_cast<QComboBox *>(m_imageMap["AntiFlashMode"])->setCurrentIndex(param.AntiFlashMode);
            static_cast<QComboBox *>(m_imageMap["ExposureMode"])->setCurrentIndex(param.ExposureMode);
            static_cast<QComboBox *>(m_imageMap["BLCMode"])->setCurrentIndex(param.BLCMode);
            switch(param.BLCMode) {
            case 1:
                static_cast<QSlider *>(m_imageMap["BLCValue"])->setValue(param.BLCIntensity);
                break;
            case 2:
                static_cast<QSlider *>(m_imageMap["BLCValue"])->setValue(param.WDRIntensity);
                break;
            case 3:
                static_cast<QSlider *>(m_imageMap["BLCValue"])->setValue(param.HLCIntensity);
                break;
            case 4:
                static_cast<QSlider *>(m_imageMap["BLCValue"])->setValue(param.DWDRIntensity);
                break;
            default:
                break;
            }
            static_cast<QComboBox *>(m_imageMap["Day/Night"])->setCurrentIndex(param.IrcutFilterMode - 1);
            static_cast<QTimeEdit *>(m_imageMap["time1"])->setTime(param.BeginTime);
            static_cast<QTimeEdit *>(m_imageMap["time2"])->setTime(param.EndTime);
        }
        break;
    }
    default:
        return;
    }

    if (isOK)
        qDebug() << "#MediaWidget# handleReceiveData, ParameterType:" << type << "parse data success...";
    else
        qDebug() << "#MediaWidget# handleReceiveData, ParameterType:" << type << "parse data error...";
}

void MediaWidget::checkInput(QString text)
{
    QLineEdit *le = dynamic_cast<QLineEdit *>(sender());
    if (le == NULL)
        return;

    QPushButton *btn = NULL;
    switch (currentIndex()) {
    case 0:
        btn = dynamic_cast<QPushButton *>(m_audioVideoMap.value("save", NULL));
        break;
    default:
        break;
    }

    if (btn == NULL)
        return;

    if (le->validator() == NULL)
        return;

    int pos = 0;
    if (le->validator()->validate(text, pos) == QValidator::Acceptable) {
        m_notPass.removeAll(le);
    } else {
        m_notPass.append(le);
    }

    btn->setEnabled(m_notPass.isEmpty());
}
