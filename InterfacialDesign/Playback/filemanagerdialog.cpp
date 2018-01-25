#include "filemanagerdialog.h"
#include "Protocol/vidiconprotocol.h"
#include "parsexml.h"
#include <QDebug>
#include <QVBoxLayout>

FileManagerDialog::FileManagerDialog(QWidget *parent) : QDialog(parent)
{
    connect(this, &FileManagerDialog::signalSetParameter, VidiconProtocol::getInstance(), &VidiconProtocol::handlerSetParameter);
    connect(VidiconProtocol::getInstance(), &VidiconProtocol::signalSendData, this, &FileManagerDialog::handlerReceiveData);

    fileView = new FileView(this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(fileView);
    setFixedWidth(400);
}

void FileManagerDialog::handlerReceiveData(int type, QByteArray data)
{
    switch(type) {
    case QUERYVIDEONAMEDAY: {
        VidiconProtocol::BackUpQueryParameter param;
        param.Type = 1;
        if(ParseXML::getInstance()->parseBackUpQueryParameter(&param, data)) {
            fileView->setDataSource(param.fileList);
            exec();
            qDebug() << "#FileManagerDialog# handlerReceiveData, ParameterType:" << type << "parse data success...";
        }else {
            qDebug() << "#FileManagerDialog# handlerReceiveData, ParameterType:" << type << "parse data error...";
        }
        break;
    }
    default:
        break;
    }
}
