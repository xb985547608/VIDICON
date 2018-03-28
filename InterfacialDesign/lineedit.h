#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <QLineEdit>

class LineEdit : public QLineEdit
{
public:
    explicit LineEdit(QWidget *parent = Q_NULLPTR) :
        QLineEdit(parent)
    {
        setAttribute(Qt::WA_InputMethodEnabled, false);
    }
    explicit LineEdit(const QString &text, QWidget *parent = Q_NULLPTR) :
        QLineEdit(text, parent)
    {
        setAttribute(Qt::WA_InputMethodEnabled, false);
    }
};

#endif // LINEEDIT_H
