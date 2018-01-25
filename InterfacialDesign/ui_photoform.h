/********************************************************************************
** Form generated from reading UI file 'photoform.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PHOTOFORM_H
#define UI_PHOTOFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PhotoForm
{
public:
    QLabel *label;

    void setupUi(QWidget *PhotoForm)
    {
        if (PhotoForm->objectName().isEmpty())
            PhotoForm->setObjectName(QStringLiteral("PhotoForm"));
        PhotoForm->resize(400, 300);
        label = new QLabel(PhotoForm);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(160, 90, 101, 81));

        retranslateUi(PhotoForm);

        QMetaObject::connectSlotsByName(PhotoForm);
    } // setupUi

    void retranslateUi(QWidget *PhotoForm)
    {
        PhotoForm->setWindowTitle(QApplication::translate("PhotoForm", "Form", Q_NULLPTR));
        label->setText(QApplication::translate("PhotoForm", "PhotoForm", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class PhotoForm: public Ui_PhotoForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PHOTOFORM_H
