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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginForm
{
public:
    QPushButton *btnLogin;
    QPushButton *btnMinimize;
    QPushButton *btnClose;
    QLabel *lblHint;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label_3;
    QLineEdit *user;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *password;
    QLabel *label;
    QWidget *sbFactoryMode;

    void setupUi(QWidget *LoginForm)
    {
        if (LoginForm->objectName().isEmpty())
            LoginForm->setObjectName(QStringLiteral("LoginForm"));
        LoginForm->resize(445, 364);
        LoginForm->setMinimumSize(QSize(445, 364));
        LoginForm->setMaximumSize(QSize(445, 364));
        LoginForm->setStyleSheet(QStringLiteral(""));
        btnLogin = new QPushButton(LoginForm);
        btnLogin->setObjectName(QStringLiteral("btnLogin"));
        btnLogin->setGeometry(QRect(72, 275, 321, 41));
        QFont font;
        font.setPointSize(18);
        btnLogin->setFont(font);
        btnLogin->setStyleSheet(QLatin1String("\n"
"QPushButton{\n"
"color:black;\n"
"border-image:url();\n"
"border:1px solid black;\n"
"border-radius:20px\n"
"}\n"
"\n"
"QPushButton:hover{\n"
"color:#B1ACB0;\n"
"border:1px solid #B1ACB0;\n"
"border-radius:20px\n"
"}"));
        btnMinimize = new QPushButton(LoginForm);
        btnMinimize->setObjectName(QStringLiteral("btnMinimize"));
        btnMinimize->setGeometry(QRect(385, 10, 21, 21));
        btnMinimize->setStyleSheet(QLatin1String("QPushButton{\n"
"border-image:url(:/images/loginBtn-Minimize-Normal.png);\n"
"}\n"
"QPushButton:hover{\n"
"border-image:url(:/images/loginBtn-Minimize-Hover.png);\n"
"}"));
        btnClose = new QPushButton(LoginForm);
        btnClose->setObjectName(QStringLiteral("btnClose"));
        btnClose->setGeometry(QRect(415, 10, 21, 21));
        btnClose->setStyleSheet(QLatin1String("QPushButton{\n"
"border-image:url(:/images/loginBtn-Close-Normal.png);\n"
"}\n"
"QPushButton:hover{\n"
"border-image:url(:/images/loginBtn-Close-Hover.png);\n"
"}"));
        lblHint = new QLabel(LoginForm);
        lblHint->setObjectName(QStringLiteral("lblHint"));
        lblHint->setGeometry(QRect(100, 236, 261, 31));
        QFont font1;
        font1.setPointSize(16);
        lblHint->setFont(font1);
        lblHint->setStyleSheet(QStringLiteral("color:red"));
        lblHint->setAlignment(Qt::AlignCenter);
        layoutWidget = new QWidget(LoginForm);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(80, 140, 302, 85));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setSpacing(17);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setMinimumSize(QSize(61, 31));
        label_3->setMaximumSize(QSize(61, 31));
        label_3->setFont(font1);
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(label_3);

        user = new QLineEdit(layoutWidget);
        user->setObjectName(QStringLiteral("user"));
        user->setMinimumSize(QSize(231, 31));
        user->setMaximumSize(QSize(231, 31));
        user->setFont(font1);
        user->setStyleSheet(QStringLiteral(""));
        user->setEchoMode(QLineEdit::Normal);

        horizontalLayout->addWidget(user);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setMinimumSize(QSize(61, 31));
        label_2->setMaximumSize(QSize(61, 31));
        label_2->setFont(font1);
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_2->addWidget(label_2);

        password = new QLineEdit(layoutWidget);
        password->setObjectName(QStringLiteral("password"));
        password->setMinimumSize(QSize(231, 31));
        password->setMaximumSize(QSize(231, 31));
        password->setFont(font1);
        password->setStyleSheet(QStringLiteral(""));
        password->setEchoMode(QLineEdit::Normal);

        horizontalLayout_2->addWidget(password);


        verticalLayout->addLayout(horizontalLayout_2);

        label = new QLabel(LoginForm);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(52, 93, 101, 30));
        QFont font2;
        font2.setPointSize(16);
        font2.setBold(true);
        font2.setWeight(75);
        label->setFont(font2);
        label->setStyleSheet(QLatin1String("border-image:url();\n"
"color:black"));
        sbFactoryMode = new QWidget(LoginForm);
        sbFactoryMode->setObjectName(QStringLiteral("sbFactoryMode"));
        sbFactoryMode->setGeometry(QRect(159, 91, 94, 35));
        sbFactoryMode->setMinimumSize(QSize(94, 35));
        sbFactoryMode->setMaximumSize(QSize(94, 35));
        sbFactoryMode->setStyleSheet(QStringLiteral("border-image:url()"));
        QWidget::setTabOrder(user, password);
        QWidget::setTabOrder(password, btnLogin);
        QWidget::setTabOrder(btnLogin, btnMinimize);
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
        lblHint->setText(QString());
        label_3->setText(QApplication::translate("LoginForm", "\350\264\246\345\217\267:", Q_NULLPTR));
        user->setText(QString());
        label_2->setText(QApplication::translate("LoginForm", "\345\257\206\347\240\201:", Q_NULLPTR));
        password->setText(QString());
        label->setText(QApplication::translate("LoginForm", "\345\267\245\345\216\202\346\250\241\345\274\217", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class LoginForm: public Ui_LoginForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINFORM_H
