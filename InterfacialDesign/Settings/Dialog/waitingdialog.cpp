#include "waitingdialog.h"
#include <QLabel>
#include <QMovie>
#include <QHBoxLayout>
#include <QVBoxLayout>

WaitingDialog::WaitingDialog(QWidget *parent) : QDialog(parent)
{
//    Qt::WindowFlags flags = Qt::Dialog;
//    flags &= ~Qt::WindowCloseButtonHint;
//    setWindowFlags(flags);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::Dialog | Qt::WindowCloseButtonHint);

    setWindowOpacity(0.5);

    QLabel *lbl1 = new QLabel("加载数据中......", this);
    QLabel *lbl2 = new QLabel(this);
    QMovie *movie = new QMovie(":/images/loading.gif");
    lbl2->setMovie(movie);
    movie->start();

    QVBoxLayout *layout1 = new QVBoxLayout;
    layout1->addStretch();
    layout1->addWidget(lbl2, 0, Qt::AlignCenter);
    layout1->addWidget(lbl1, 0, Qt::AlignCenter);
    layout1->addStretch();

    QHBoxLayout *layout2 = new QHBoxLayout(this);
    layout2->addStretch();
    layout2->addLayout(layout1);
    layout2->addStretch();

    resize(200, 100);
}
