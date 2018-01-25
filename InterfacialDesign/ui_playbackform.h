/********************************************************************************
** Form generated from reading UI file 'playbackform.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLAYBACKFORM_H
#define UI_PLAYBACKFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PlaybackForm
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QWidget *displayArea;
    QWidget *rightBar;
    QWidget *timeslider;

    void setupUi(QWidget *PlaybackForm)
    {
        if (PlaybackForm->objectName().isEmpty())
            PlaybackForm->setObjectName(QStringLiteral("PlaybackForm"));
        PlaybackForm->resize(815, 531);
        PlaybackForm->setMinimumSize(QSize(600, 400));
        verticalLayout = new QVBoxLayout(PlaybackForm);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        displayArea = new QWidget(PlaybackForm);
        displayArea->setObjectName(QStringLiteral("displayArea"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(3);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(displayArea->sizePolicy().hasHeightForWidth());
        displayArea->setSizePolicy(sizePolicy);
        displayArea->setStyleSheet(QStringLiteral("background-color:black"));

        horizontalLayout->addWidget(displayArea);

        rightBar = new QWidget(PlaybackForm);
        rightBar->setObjectName(QStringLiteral("rightBar"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(rightBar->sizePolicy().hasHeightForWidth());
        rightBar->setSizePolicy(sizePolicy1);
        rightBar->setMinimumSize(QSize(200, 0));
        rightBar->setMaximumSize(QSize(200, 16777215));

        horizontalLayout->addWidget(rightBar);


        verticalLayout->addLayout(horizontalLayout);

        timeslider = new QWidget(PlaybackForm);
        timeslider->setObjectName(QStringLiteral("timeslider"));
        timeslider->setMinimumSize(QSize(0, 91));
        timeslider->setMaximumSize(QSize(16777215, 91));

        verticalLayout->addWidget(timeslider);


        retranslateUi(PlaybackForm);

        QMetaObject::connectSlotsByName(PlaybackForm);
    } // setupUi

    void retranslateUi(QWidget *PlaybackForm)
    {
        PlaybackForm->setWindowTitle(QApplication::translate("PlaybackForm", "Form", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class PlaybackForm: public Ui_PlaybackForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLAYBACKFORM_H
