/********************************************************************************
** Form generated from reading UI file 'previewform.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PREVIEWFORM_H
#define UI_PREVIEWFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PreviewForm
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *toolBar;
    QHBoxLayout *horizontalLayout;
    QRadioButton *mainStream;
    QRadioButton *subStream;
    QSpacerItem *horizontalSpacer;
    QPushButton *alarm;
    QPushButton *motion;
    QPushButton *blind;
    QPushButton *manualRecord;
    QPushButton *autoRecord;
    QPushButton *voice;
    QPushButton *snapshot;
    QWidget *displayArea;

    void setupUi(QWidget *PreviewForm)
    {
        if (PreviewForm->objectName().isEmpty())
            PreviewForm->setObjectName(QStringLiteral("PreviewForm"));
        PreviewForm->resize(764, 487);
        PreviewForm->setMinimumSize(QSize(600, 400));
        verticalLayout = new QVBoxLayout(PreviewForm);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        toolBar = new QWidget(PreviewForm);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        toolBar->setMinimumSize(QSize(0, 40));
        toolBar->setMaximumSize(QSize(16777215, 40));
        toolBar->setStyleSheet(QLatin1String("QWidget#toolBar{\n"
"	background-color: #DCDCDC\n"
"}"));
        horizontalLayout = new QHBoxLayout(toolBar);
        horizontalLayout->setSpacing(13);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(15, 0, 14, 0);
        mainStream = new QRadioButton(toolBar);
        mainStream->setObjectName(QStringLiteral("mainStream"));
        mainStream->setMinimumSize(QSize(91, 18));
        mainStream->setMaximumSize(QSize(91, 18));

        horizontalLayout->addWidget(mainStream);

        subStream = new QRadioButton(toolBar);
        subStream->setObjectName(QStringLiteral("subStream"));
        subStream->setMinimumSize(QSize(91, 18));
        subStream->setMaximumSize(QSize(91, 18));

        horizontalLayout->addWidget(subStream);

        horizontalSpacer = new QSpacerItem(258, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        alarm = new QPushButton(toolBar);
        alarm->setObjectName(QStringLiteral("alarm"));
        alarm->setMinimumSize(QSize(25, 25));
        alarm->setMaximumSize(QSize(25, 25));
        alarm->setStyleSheet(QLatin1String("QPushButton[State=\"enable\"]{\n"
"	border-image:url(:images/alarmhint.png) 0 40 0 40 \n"
"}\n"
"QPushButton[State=\"disable\"]{\n"
"	border-image:url(:images/alarmhint.png) 0 80 0 0\n"
"}\n"
"QPushButton[State=\"flicker\"]{\n"
"	border-image:url(:images/alarmhint.png) 0 0 0 80\n"
"}"));

        horizontalLayout->addWidget(alarm);

        motion = new QPushButton(toolBar);
        motion->setObjectName(QStringLiteral("motion"));
        motion->setMinimumSize(QSize(25, 25));
        motion->setMaximumSize(QSize(25, 25));
        motion->setStyleSheet(QLatin1String("QPushButton[State=\"enable\"]{\n"
"	border-image:url(:images/motion.png) 0 40 0 40 \n"
"}\n"
"QPushButton[State=\"disable\"]{\n"
"	border-image:url(:images/motion.png) 0 80 0 0\n"
"}\n"
"QPushButton[State=\"flicker\"]{\n"
"	border-image:url(:images/motion.png) 0 0 0 80\n"
"}"));

        horizontalLayout->addWidget(motion);

        blind = new QPushButton(toolBar);
        blind->setObjectName(QStringLiteral("blind"));
        blind->setMinimumSize(QSize(25, 25));
        blind->setMaximumSize(QSize(25, 25));
        blind->setStyleSheet(QLatin1String("QPushButton[State=\"enable\"]{\n"
"	border-image:url(:images/blind.png) 0 40 0 40 \n"
"}\n"
"QPushButton[State=\"disable\"]{\n"
"	border-image:url(:images/blind.png) 0 80 0 0\n"
"}\n"
"QPushButton[State=\"flicker\"]{\n"
"	border-image:url(:images/blind.png) 0 0 0 80\n"
"}"));

        horizontalLayout->addWidget(blind);

        manualRecord = new QPushButton(toolBar);
        manualRecord->setObjectName(QStringLiteral("manualRecord"));
        manualRecord->setMinimumSize(QSize(25, 25));
        manualRecord->setMaximumSize(QSize(25, 25));
        manualRecord->setStyleSheet(QLatin1String("QPushButton{\n"
"	border-image:url(:images/manualRecord.png) 0 40 0 0\n"
"}\n"
"QPushButton:hover{\n"
"	border-image:url(:images/manualRecord.png) 0 0 0 40\n"
"}"));

        horizontalLayout->addWidget(manualRecord);

        autoRecord = new QPushButton(toolBar);
        autoRecord->setObjectName(QStringLiteral("autoRecord"));
        autoRecord->setMinimumSize(QSize(25, 25));
        autoRecord->setMaximumSize(QSize(25, 25));
        autoRecord->setStyleSheet(QLatin1String("QPushButton{\n"
"	border-image:url(:images/autoRecord.png) 0 40 0 0\n"
"}"));

        horizontalLayout->addWidget(autoRecord);

        voice = new QPushButton(toolBar);
        voice->setObjectName(QStringLiteral("voice"));
        voice->setMinimumSize(QSize(25, 25));
        voice->setMaximumSize(QSize(25, 25));
        voice->setStyleSheet(QLatin1String("QPushButton{\n"
"	border-image:url(:images/voice.png) 0 40 0 0\n"
"}\n"
"QPushButton:hover{\n"
"	border-image:url(:images/voice.png) 0 0 0 40\n"
"}"));

        horizontalLayout->addWidget(voice);

        snapshot = new QPushButton(toolBar);
        snapshot->setObjectName(QStringLiteral("snapshot"));
        snapshot->setMinimumSize(QSize(25, 25));
        snapshot->setMaximumSize(QSize(25, 25));
        snapshot->setStyleSheet(QLatin1String("QPushButton{\n"
"	border-image:url(:images/snapshot.png) 0 40 0 0\n"
"}\n"
"QPushButton:hover{\n"
"	border-image:url(:images/snapshot.png) 0 0 0 40\n"
"}"));

        horizontalLayout->addWidget(snapshot);


        verticalLayout->addWidget(toolBar);

        displayArea = new QWidget(PreviewForm);
        displayArea->setObjectName(QStringLiteral("displayArea"));
        displayArea->setStyleSheet(QStringLiteral("background-color:black"));

        verticalLayout->addWidget(displayArea);


        retranslateUi(PreviewForm);

        QMetaObject::connectSlotsByName(PreviewForm);
    } // setupUi

    void retranslateUi(QWidget *PreviewForm)
    {
        PreviewForm->setWindowTitle(QApplication::translate("PreviewForm", "Form", Q_NULLPTR));
        mainStream->setText(QApplication::translate("PreviewForm", "Main Stream", Q_NULLPTR));
        subStream->setText(QApplication::translate("PreviewForm", "Sub Stream", Q_NULLPTR));
        alarm->setText(QString());
        motion->setText(QString());
        blind->setText(QString());
        manualRecord->setText(QString());
        autoRecord->setText(QString());
        voice->setText(QString());
        snapshot->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class PreviewForm: public Ui_PreviewForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PREVIEWFORM_H
