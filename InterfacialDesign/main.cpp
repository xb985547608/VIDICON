#include "mainwindow.h"
#include <QApplication>
#include "../OperationInterface/operationinterface.h"
#include "Login/loginwidget.h"
#include "customstyle.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    a.setStyle(new CustomStyle);

    OperationInterface o;

    LoginWidget::getInstance()->show();

    MainWindow w;

    return a.exec();
}
