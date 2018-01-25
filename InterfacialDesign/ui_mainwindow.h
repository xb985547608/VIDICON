/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionSave;
    QAction *actionOpen;
    QAction *actionClose;
    QAction *actionExit;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QFrame *frameSwitch;
    QFrame *frameContent;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(589, 473);
        MainWindow->setStyleSheet(QString::fromUtf8("QMenu {\n"
"font-size:14px;\n"
"background-color: #f0f0f0; /* sets background of the menu \350\256\276\347\275\256\346\225\264\344\270\252\350\217\234\345\215\225\345\214\272\345\237\237\347\232\204\350\203\214\346\231\257\350\211\262\357\274\214\346\210\221\347\224\250\347\232\204\346\230\257\347\231\275\350\211\262\357\274\232white*/\n"
"}\n"
"  \n"
"QMenu::item {\n"
"background-color: transparent;\n"
"padding:2px 20px 2px 8px;/*\350\256\276\347\275\256\350\217\234\345\215\225\351\241\271\346\226\207\345\255\227\344\270\212\344\270\213\345\222\214\345\267\246\345\217\263\347\232\204\345\206\205\350\276\271\350\267\235\357\274\214\346\225\210\346\236\234\345\260\261\346\230\257\350\217\234\345\215\225\344\270\255\347\232\204\346\235\241\347\233\256\345\267\246\345\217\263\344\270\212\344\270\213\346\234\211\344\272\206\351\227\264\351\232\224*/\n"
"margin:0px 10px 0px 20px;/*\350\256\276\347\275\256\350\217\234\345\215\225\351\241\271\347\232\204\345\244\226\350\276\271\350\267\235*/\n"
"border-left:1px solid"
                        " #DBDBDB;/*\344\270\272\350\217\234\345\215\225\351\241\271\344\271\213\351\227\264\346\267\273\345\212\240\346\250\252\347\272\277\351\227\264\351\232\224*/\n"
"}\n"
"  \n"
"QMenu::item:selected { /* when user selects item using mouse or keyboard */\n"
"border:1px solid #aecff7;\n"
"border-radius:3px;\n"
"background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #f1f3f6, stop:1 #eef3f9);/*\350\277\231\344\270\200\345\217\245\346\230\257\350\256\276\347\275\256\350\217\234\345\215\225\351\241\271\351\274\240\346\240\207\347\273\217\350\277\207\351\200\211\344\270\255\347\232\204\346\240\267\345\274\217*/\n"
"}\n"
"QMenuBar{\n"
"color:black;\n"
"background-color:qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #DDDDDE, stop:1 #C7C7C8);\n"
"}\n"
"QMenuBar::item:selected{\n"
"background-color:#595B5C\n"
"}\n"
"QMenuBar::item:pressed{\n"
"background-color:#595B5C\n"
"}"));
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionClose = new QAction(MainWindow);
        actionClose->setObjectName(QStringLiteral("actionClose"));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        frameSwitch = new QFrame(centralWidget);
        frameSwitch->setObjectName(QStringLiteral("frameSwitch"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frameSwitch->sizePolicy().hasHeightForWidth());
        frameSwitch->setSizePolicy(sizePolicy);
        frameSwitch->setMinimumSize(QSize(60, 0));
        frameSwitch->setMaximumSize(QSize(60, 16777215));
        frameSwitch->setStyleSheet(QStringLiteral(""));

        horizontalLayout->addWidget(frameSwitch);

        frameContent = new QFrame(centralWidget);
        frameContent->setObjectName(QStringLiteral("frameContent"));
        frameContent->setFrameShape(QFrame::StyledPanel);
        frameContent->setFrameShadow(QFrame::Raised);

        horizontalLayout->addWidget(frameContent);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        actionSave->setText(QApplication::translate("MainWindow", "Save", Q_NULLPTR));
        actionOpen->setText(QApplication::translate("MainWindow", "Open", Q_NULLPTR));
        actionClose->setText(QApplication::translate("MainWindow", "Close", Q_NULLPTR));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
