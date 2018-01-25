/********************************************************************************
** Form generated from reading UI file 'volumeform.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VOLUMEFORM_H
#define UI_VOLUMEFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VolumeForm
{
public:
    QPushButton *btnVolumeAdd;
    QPushButton *btnVolumeSub;
    QSlider *sliderPreviewVolume;

    void setupUi(QWidget *VolumeForm)
    {
        if (VolumeForm->objectName().isEmpty())
            VolumeForm->setObjectName(QStringLiteral("VolumeForm"));
        VolumeForm->resize(35, 161);
        VolumeForm->setMinimumSize(QSize(35, 161));
        VolumeForm->setMaximumSize(QSize(35, 161));
        VolumeForm->setStyleSheet(QLatin1String("border-image:url(:/images/volumeSlider-BackGround.png);\n"
"background-color:#F7F7F7;\n"
"border:0px"));
        btnVolumeAdd = new QPushButton(VolumeForm);
        btnVolumeAdd->setObjectName(QStringLiteral("btnVolumeAdd"));
        btnVolumeAdd->setGeometry(QRect(0, -3, 35, 35));
        btnVolumeAdd->setMinimumSize(QSize(35, 35));
        btnVolumeAdd->setMaximumSize(QSize(35, 35));
        btnVolumeAdd->setStyleSheet(QStringLiteral("border-image:url(:/images/volumeSlider-Add.png);"));
        btnVolumeSub = new QPushButton(VolumeForm);
        btnVolumeSub->setObjectName(QStringLiteral("btnVolumeSub"));
        btnVolumeSub->setGeometry(QRect(0, 131, 35, 35));
        btnVolumeSub->setStyleSheet(QStringLiteral("border-image:url(:/images/volumeSlider-Sub.png);"));
        sliderPreviewVolume = new QSlider(VolumeForm);
        sliderPreviewVolume->setObjectName(QStringLiteral("sliderPreviewVolume"));
        sliderPreviewVolume->setGeometry(QRect(-1, 30, 37, 102));
        sliderPreviewVolume->setMinimumSize(QSize(35, 0));
        sliderPreviewVolume->setStyleSheet(QLatin1String("border-image:url();\n"
"background-color:#F7F7F7;\n"
"border:0px"));
        sliderPreviewVolume->setMinimum(1);
        sliderPreviewVolume->setMaximum(100);
        sliderPreviewVolume->setOrientation(Qt::Vertical);

        retranslateUi(VolumeForm);

        QMetaObject::connectSlotsByName(VolumeForm);
    } // setupUi

    void retranslateUi(QWidget *VolumeForm)
    {
        VolumeForm->setWindowTitle(QApplication::translate("VolumeForm", "Form", Q_NULLPTR));
        btnVolumeAdd->setText(QString());
        btnVolumeSub->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class VolumeForm: public Ui_VolumeForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VOLUMEFORM_H
