/********************************************************************************
** Form generated from reading UI file 'settingsform.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGSFORM_H
#define UI_SETTINGSFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SettingsForm
{
public:
    QHBoxLayout *horizontalLayout;
    QWidget *sideBar;
    QWidget *displayArea;

    void setupUi(QWidget *SettingsForm)
    {
        if (SettingsForm->objectName().isEmpty())
            SettingsForm->setObjectName(QStringLiteral("SettingsForm"));
        SettingsForm->resize(847, 576);
        SettingsForm->setStyleSheet(QLatin1String("QScrollBar:vertical\n"
"{\n"
"    width:8px;\n"
"    background:rgba(0,0,0,0%);\n"
"    margin:0px,0px,0px,0px;\n"
"    padding-top:9px;   \n"
"    padding-bottom:9px;\n"
"}\n"
"QScrollBar::handle:vertical\n"
"{\n"
"    width:8px;\n"
"    background:rgba(0,0,0,25%);\n"
"    border-radius:4px;  \n"
"    min-height:20;\n"
"}\n"
"QScrollBar::handle:vertical:hover\n"
"{\n"
"    width:8px;\n"
"    background:rgba(0,0,0,50%);  \n"
"    border-radius:4px;\n"
"    min-height:20;\n"
"}\n"
"QScrollBar::add-line:vertical  \n"
"{\n"
"    height:9px;width:8px;\n"
"    border-image:url(:/images/a/3.png);\n"
"    subcontrol-position:bottom;\n"
"}\n"
"QScrollBar::sub-line:vertical \n"
"{\n"
"    height:9px;width:8px;\n"
"    border-image:url(:/images/a/1.png);\n"
"    subcontrol-position:top;\n"
"}\n"
"QScrollBar::add-line:vertical:hover\n"
"{\n"
"    height:9px;width:8px;\n"
"    border-image:url(:/images/a/4.png);\n"
"    subcontrol-position:bottom;\n"
"}\n"
"QScrollBar::sub-line:vertical:hover \n"
"{\n"
"    height:9px;wid"
                        "th:8px;\n"
"    border-image:url(:/images/a/2.png);\n"
"    subcontrol-position:top;\n"
"}\n"
"QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical \n"
"{\n"
"    background:rgba(0,0,0,10%);\n"
"    border-radius:4px;\n"
"}\n"
"QTabBar::tab {\n"
"    background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                stop: 0 #E1E1E1, stop: 0.4 #DDDDDD,\n"
"                                stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3);\n"
"    border: 0px solid #C4C4C3;\n"
"    border-bottom-color: #C2C7CB; /* same as the pane color */\n"
"    border-top-left-radius: 4px;\n"
"    border-top-right-radius: 4px;\n"
"    min-width: 8ex;\n"
"    padding:5px 10px;\n"
"margin-right:3px;\n"
"}\n"
"\n"
"QTabBar::tab:selected, QTabBar::tab:hover {\n"
"    background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                stop: 0 #fafafa, stop: 0.4 #f4f4f4,\n"
"                                stop: 0.5 #e7e7e7, stop: 1.0 #fafafa);\n"
"    \n"
"}\n"
"\n"
"QTabBar::tab:selected"
                        " {\n"
"    border-bottom:2px solid #ffa02f\n"
"}\n"
"\n"
"QTabBar::tab:!selected {\n"
"    margin-top: 2px; /* make non-selected tabs look smaller */\n"
"}"));
        horizontalLayout = new QHBoxLayout(SettingsForm);
        horizontalLayout->setSpacing(2);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        sideBar = new QWidget(SettingsForm);
        sideBar->setObjectName(QStringLiteral("sideBar"));
        sideBar->setMinimumSize(QSize(141, 0));
        sideBar->setMaximumSize(QSize(141, 16777215));

        horizontalLayout->addWidget(sideBar);

        displayArea = new QWidget(SettingsForm);
        displayArea->setObjectName(QStringLiteral("displayArea"));

        horizontalLayout->addWidget(displayArea);


        retranslateUi(SettingsForm);

        QMetaObject::connectSlotsByName(SettingsForm);
    } // setupUi

    void retranslateUi(QWidget *SettingsForm)
    {
        SettingsForm->setWindowTitle(QApplication::translate("SettingsForm", "Form", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class SettingsForm: public Ui_SettingsForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGSFORM_H
