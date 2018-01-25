/********************************************************************************
** Form generated from reading UI file 'loginform.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINFORM_H
#define UI_LOGINFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginForm
{
public:
    QHBoxLayout *horizontalLayout;
    QFrame *frame;
    QPushButton *btnLogin;
    QPushButton *btnMinimize;
    QPushButton *btnClose;
    QLabel *lblHint;
    QLineEdit *password;
    QWidget *sbFactoryMode;
    QLabel *label;

    void setupUi(QWidget *LoginForm)
    {
        if (LoginForm->objectName().isEmpty())
            LoginForm->setObjectName(QStringLiteral("LoginForm"));
        LoginForm->resize(442, 400);
        LoginForm->setStyleSheet(QStringLiteral(""));
        horizontalLayout = new QHBoxLayout(LoginForm);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        frame = new QFrame(LoginForm);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setMinimumSize(QSize(442, 400));
        frame->setMaximumSize(QSize(442, 400));
        frame->setStyleSheet(QStringLiteral("border-image:url(:/images/login-Background.png);"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        btnLogin = new QPushButton(frame);
        btnLogin->setObjectName(QStringLiteral("btnLogin"));
        btnLogin->setGeometry(QRect(58, 262, 321, 41));
        QFont font;
        font.setPointSize(18);
        btnLogin->setFont(font);
        btnLogin->setStyleSheet(QLatin1String("\n"
"QPushButton{\n"
"color:white;\n"
"border-image:url();\n"
"border:1px solid white;\n"
"border-radius:20px\n"
"}\n"
"\n"
"QPushButton:hover{\n"
"color:#B1ACB0;\n"
"border:1px solid #B1ACB0;\n"
"border-radius:20px\n"
"}"));
        btnMinimize = new QPushButton(frame);
        btnMinimize->setObjectName(QStringLiteral("btnMinimize"));
        btnMinimize->setGeometry(QRect(390, 10, 21, 21));
        btnMinimize->setStyleSheet(QLatin1String("QPushButton{\n"
"border-image:url(:/images/loginBtn-Minimize-Normal.png);\n"
"}\n"
"QPushButton:hover{\n"
"border-image:url(:/images/loginBtn-Minimize-Hover.png);\n"
"}"));
        btnClose = new QPushButton(frame);
        btnClose->setObjectName(QStringLiteral("btnClose"));
        btnClose->setGeometry(QRect(415, 10, 21, 21));
        btnClose->setStyleSheet(QLatin1String("QPushButton{\n"
"border-image:url(:/images/loginBtn-Close-Normal.png);\n"
"}\n"
"QPushButton:hover{\n"
"border-image:url(:/images/loginBtn-Close-Hover.png);\n"
"}"));
        lblHint = new QLabel(frame);
        lblHint->setObjectName(QStringLiteral("lblHint"));
        lblHint->setGeometry(QRect(138, 226, 161, 31));
        QFont font1;
        font1.setPointSize(13);
        lblHint->setFont(font1);
        lblHint->setStyleSheet(QLatin1String("border-image:url();\n"
"background-color:transparent;\n"
"border:0px;\n"
"color:red\n"
""));
        lblHint->setAlignment(Qt::AlignCenter);
        password = new QLineEdit(frame);
        password->setObjectName(QStringLiteral("password"));
        password->setGeometry(QRect(145, 186, 231, 31));
        QFont font2;
        font2.setPointSize(16);
        password->setFont(font2);
        password->setStyleSheet(QLatin1String("border-image:url();\n"
"background-color:transparent;\n"
"border:0px;\n"
"color:white\n"
""));
        password->setEchoMode(QLineEdit::Normal);
        sbFactoryMode = new QWidget(frame);
        sbFactoryMode->setObjectName(QStringLiteral("sbFactoryMode"));
        sbFactoryMode->setGeometry(QRect(199, 131, 94, 35));
        sbFactoryMode->setMinimumSize(QSize(94, 35));
        sbFactoryMode->setMaximumSize(QSize(94, 35));
        sbFactoryMode->setStyleSheet(QStringLiteral("border-image:url()"));
        label = new QLabel(frame);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(112, 133, 72, 30));
        QFont font3;
        font3.setPointSize(13);
        font3.setBold(true);
        font3.setWeight(75);
        label->setFont(font3);
        label->setStyleSheet(QLatin1String("border-image:url();\n"
"color:white"));

        horizontalLayout->addWidget(frame);

        QWidget::setTabOrder(btnLogin, password);
        QWidget::setTabOrder(password, btnMinimize);
        QWidget::setTabOrder(btnMinimize, btnClose);

        retranslateUi(LoginForm);

        QMetaObject::connectSlotsByName(LoginForm);
    } // setupUi

    void retranslateUi(QWidget *LoginForm)
    {
        LoginForm->setWindowTitle(QApplication::translate("LoginForm", "Form", Q_NULLPTR));
        btnLogin->setText(QApplication::translate("LoginForm", "Login", Q_NULLPTR));
        btnMinimize->setText(QString());
        btnClose->setText(QString());
        lblHint->setText(QApplication::translate("LoginForm", "<html><head/><body><p><br/></p></body></html>", Q_NULLPTR));
        password->setText(QString());
        label->setText(QApplication::translate("LoginForm", "\345\267\245\345\216\202\346\250\241\345\274\217", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class LoginForm: public Ui_LoginForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINFORM_H
