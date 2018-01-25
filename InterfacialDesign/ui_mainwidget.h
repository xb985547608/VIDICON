/********************************************************************************
** Form generated from reading UI file 'mainwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWIDGET_H
#define UI_MAINWIDGET_H

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

class Ui_Form
{
public:
    QHBoxLayout *horizontalLayout;
    QWidget *widgetMenu;
    QVBoxLayout *verticalLayout;
    QPushButton *btnWelcome;
    QPushButton *btnPreview;
    QPushButton *btnPlayback;
    QPushButton *btnPhoto;
    QPushButton *btnSetup;
    QSpacerItem *verticalSpacer;
    QPushButton *btnLogout;
    QWidget *widgetMain;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QStringLiteral("Form"));
        Form->resize(983, 672);
        Form->setMinimumSize(QSize(660, 400));
        horizontalLayout = new QHBoxLayout(Form);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        widgetMenu = new QWidget(Form);
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
"	border-image:url(:/image-assets/welcome.png)\n"
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
"	border-image:url(:/image-assets/preview.png)\n"
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
"	border-image:url(:/image-assets/playback.png)\n"
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
"	border-image:url(:/image-assets/photo.png)\n"
"}\n"
"QPushButton[choice=\"true\"]{\n"
"	background-color:#EDF1F6;\n"
"}\n"
"QPushButton:hover[choice=\"false\"]{\n"
"	background-color:#595B5D;\n"
"}"));

        verticalLayout->addWidget(btnPhoto);

        btnSetup = new QPushButton(widgetMenu);
        btnSetup->setObjectName(QStringLiteral("btnSetup"));
        btnSetup->setMinimumSize(QSize(60, 50));
        btnSetup->setMaximumSize(QSize(60, 50));
        btnSetup->setFocusPolicy(Qt::NoFocus);
        btnSetup->setStyleSheet(QLatin1String("QPushButton{\n"
"	border-image:url(:/image-assets/setup.png)\n"
"}\n"
"QPushButton[choice=\"true\"]{\n"
"	background-color:#EDF1F6;\n"
"}\n"
"QPushButton:hover[choice=\"false\"]{\n"
"	background-color:#595B5D;\n"
"}"));

        verticalLayout->addWidget(btnSetup);

        verticalSpacer = new QSpacerItem(20, 17, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        btnLogout = new QPushButton(widgetMenu);
        btnLogout->setObjectName(QStringLiteral("btnLogout"));
        btnLogout->setMinimumSize(QSize(60, 50));
        btnLogout->setMaximumSize(QSize(60, 50));
        btnLogout->setFocusPolicy(Qt::NoFocus);
        btnLogout->setStyleSheet(QLatin1String("QPushButton{\n"
"	border-image:url(:/image-assets/logout.png)\n"
"}\n"
"QPushButton:hover{\n"
"	background-color:#595B5D;\n"
"}"));

        verticalLayout->addWidget(btnLogout);


        horizontalLayout->addWidget(widgetMenu);

        widgetMain = new QWidget(Form);
        widgetMain->setObjectName(QStringLiteral("widgetMain"));
        widgetMain->setStyleSheet(QStringLiteral("background-color:#EDF1F6"));

        horizontalLayout->addWidget(widgetMain);

        QWidget::setTabOrder(btnWelcome, btnPreview);
        QWidget::setTabOrder(btnPreview, btnPlayback);
        QWidget::setTabOrder(btnPlayback, btnPhoto);
        QWidget::setTabOrder(btnPhoto, btnSetup);
        QWidget::setTabOrder(btnSetup, btnLogout);

        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "Form", Q_NULLPTR));
        btnWelcome->setText(QString());
        btnPreview->setText(QString());
        btnPlayback->setText(QString());
        btnPhoto->setText(QString());
        btnSetup->setText(QString());
        btnLogout->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWIDGET_H
