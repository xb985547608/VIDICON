/********************************************************************************
** Form generated from reading UI file 'settings.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGS_H
#define UI_SETTINGS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SetupForm
{
public:
    QLabel *label;

    void setupUi(QWidget *SetupForm)
    {
        if (SetupForm->objectName().isEmpty())
            SetupForm->setObjectName(QStringLiteral("SetupForm"));
        SetupForm->resize(400, 300);
        label = new QLabel(SetupForm);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(130, 90, 131, 101));

        retranslateUi(SetupForm);

        QMetaObject::connectSlotsByName(SetupForm);
    } // setupUi

    void retranslateUi(QWidget *SetupForm)
    {
        SetupForm->setWindowTitle(QApplication::translate("SetupForm", "Form", Q_NULLPTR));
        label->setText(QApplication::translate("SetupForm", "SetupForm", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class SetupForm: public Ui_SetupForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGS_H
