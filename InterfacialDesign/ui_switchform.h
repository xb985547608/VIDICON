/********************************************************************************
** Form generated from reading UI file 'switchform.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SWITCHFORM_H
#define UI_SWITCHFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SwitchForm
{
public:
    QHBoxLayout *horizontalLayout;
    QWidget *widgetMenu;
    QVBoxLayout *verticalLayout;
    QPushButton *btnWelcome;
    QPushButton *btnPreview;
    QPushButton *btnPlayback;
    QPushButton *btnPhoto;
    QPushButton *btnSettings;
    QSpacerItem *verticalSpacer;
    QPushButton *btnLogout;

    void setupUi(QWidget *SwitchForm)
    {
        if (SwitchForm->objectName().isEmpty())
            SwitchForm->setObjectName(QStringLiteral("SwitchForm"));
        SwitchForm->resize(60, 400);
        SwitchForm->setMinimumSize(QSize(60, 400));
        SwitchForm->setMaximumSize(QSize(60, 16777215));
        horizontalLayout = new QHBoxLayout(SwitchForm);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        widgetMenu = new QWidget(SwitchForm);
        widgetMenu->setObjectName(QStringLiteral("widgetMenu"));
        widgetMenu->setMinimumSize(QSize(60, 400));
        widgetMenu->setMaximumSize(QSize(60, 16777215));
        widgetMenu->setStyleSheet(QStringLiteral("background-color:#404244"));
        verticalLayout = new QVBoxLayout(widgetMenu);
        verticalLayout->setSpacing(10);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 10, 0, 10);
        btnWelcome = new QPushButton(widgetMenu);
        btnWelcome->setObjectName(QStringLiteral("btnWelcome"));
        btnWelcome->setMinimumSize(QSize(60, 50));
        btnWelcome->setMaximumSize(QSize(60, 50));
        btnWelcome->setFocusPolicy(Qt::NoFocus);
        btnWelcome->setStyleSheet(QLatin1String("QPushButton{\n"
"	border-image:url(:/images/welcomeBtn.png)\n"
"}\n"
"QPushButton[choice=\"true\"]{\n"
"	background-color:#EDF1F6;\n"
"}\n"
"QPushButton:hover[choice=\"false\"]{\n"
"	background-color:#595B5D;\n"
"}"));

        verticalLayout->addWidget(btnWelcome);

        btnPreview = new QPushButton(widgetMenu);
        btnPreview->setObjectName(QStringLiteral("btnPreview"));
        btnPreview->setMinimumSize(QSize(60, 50));
        btnPreview->setMaximumSize(QSize(60, 50));
        btnPreview->setFocusPolicy(Qt::NoFocus);
        btnPreview->setStyleSheet(QLatin1String("QPushButton{\n"
"	border-image:url(:/images/previewBtn.png)\n"
"}\n"
"QPushButton[choice=\"true\"]{\n"
"	background-color:#EDF1F6;\n"
"}\n"
"QPushButton:hover[choice=\"false\"]{\n"
"	background-color:#595B5D;\n"
"}"));

        verticalLayout->addWidget(btnPreview);

        btnPlayback = new QPushButton(widgetMenu);
        btnPlayback->setObjectName(QStringLiteral("btnPlayback"));
        btnPlayback->setMinimumSize(QSize(60, 50));
        btnPlayback->setMaximumSize(QSize(60, 50));
        btnPlayback->setFocusPolicy(Qt::NoFocus);
        btnPlayback->setStyleSheet(QLatin1String("QPushButton{\n"
"	border-image:url(:/images/playbackBtn.png)\n"
"}\n"
"QPushButton[choice=\"true\"]{\n"
"	background-color:#EDF1F6;\n"
"}\n"
"QPushButton:hover[choice=\"false\"]{\n"
"	background-color:#595B5D;\n"
"}"));

        verticalLayout->addWidget(btnPlayback);

        btnPhoto = new QPushButton(widgetMenu);
        btnPhoto->setObjectName(QStringLiteral("btnPhoto"));
        btnPhoto->setMinimumSize(QSize(60, 50));
        btnPhoto->setMaximumSize(QSize(60, 50));
        btnPhoto->setFocusPolicy(Qt::NoFocus);
        btnPhoto->setStyleSheet(QLatin1String("QPushButton{\n"
"	border-image:url(:/images/photoBtn.png)\n"
"}\n"
"QPushButton[choice=\"true\"]{\n"
"	background-color:#EDF1F6;\n"
"}\n"
"QPushButton:hover[choice=\"false\"]{\n"
"	background-color:#595B5D;\n"
"}"));

        verticalLayout->addWidget(btnPhoto);

        btnSettings = new QPushButton(widgetMenu);
        btnSettings->setObjectName(QStringLiteral("btnSettings"));
        btnSettings->setMinimumSize(QSize(60, 50));
        btnSettings->setMaximumSize(QSize(60, 50));
        btnSettings->setFocusPolicy(Qt::NoFocus);
        btnSettings->setStyleSheet(QLatin1String("QPushButton{\n"
"	border-image:url(:/images/settingsBtn.png)\n"
"}\n"
"QPushButton[choice=\"true\"]{\n"
"	background-color:#EDF1F6;\n"
"}\n"
"QPushButton:hover[choice=\"false\"]{\n"
"	background-color:#595B5D;\n"
"}"));

        verticalLayout->addWidget(btnSettings);

        verticalSpacer = new QSpacerItem(20, 17, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        btnLogout = new QPushButton(widgetMenu);
        btnLogout->setObjectName(QStringLiteral("btnLogout"));
        btnLogout->setMinimumSize(QSize(60, 50));
        btnLogout->setMaximumSize(QSize(60, 50));
        btnLogout->setFocusPolicy(Qt::NoFocus);
        btnLogout->setStyleSheet(QLatin1String("QPushButton{\n"
"	border-image:url(:/images/logoutBtn.png)\n"
"}\n"
"QPushButton:hover{\n"
"	background-color:#595B5D;\n"
"}"));

        verticalLayout->addWidget(btnLogout);


        horizontalLayout->addWidget(widgetMenu);

        QWidget::setTabOrder(btnWelcome, btnPreview);
        QWidget::setTabOrder(btnPreview, btnPlayback);
        QWidget::setTabOrder(btnPlayback, btnPhoto);
        QWidget::setTabOrder(btnPhoto, btnSettings);
        QWidget::setTabOrder(btnSettings, btnLogout);

        retranslateUi(SwitchForm);

        QMetaObject::connectSlotsByName(SwitchForm);
    } // setupUi

    void retranslateUi(QWidget *SwitchForm)
    {
        SwitchForm->setWindowTitle(QApplication::translate("SwitchForm", "Form", Q_NULLPTR));
        btnWelcome->setText(QString());
        btnPreview->setText(QString());
        btnPlayback->setText(QString());
        btnPhoto->setText(QString());
        btnSettings->setText(QString());
        btnLogout->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class SwitchForm: public Ui_SwitchForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SWITCHFORM_H
