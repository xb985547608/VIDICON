#include "filemanagerdialog.h"
#include "Protocol/vidiconprotocol.h"
#include "parsexml.h"
#include <QDebug>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QPushButton>

FileManagerDialog::FileManagerDialog(QWidget *parent) :
    QDialog(parent),
    isVideo(true)
{
    connect(this, &FileManagerDialog::signalGetParameter, VidiconProtocol::getInstance(), &VidiconProtocol::handlerGetParameter);
    connect(VidiconProtocol::getInstance(), &VidiconProtocol::signalSendData, this, &FileManagerDialog::handlerReceiveData);

    QRadioButton *rBtn1 = new QRadioButton("视频", this);
    rBtn1->setChecked(true);
    QRadioButton *rBtn2 = new QRadioButton("图片", this);
    connect(rBtn1, &QRadioButton::toggled, this, [this](bool checked) {
        fileView->setDataSource(checked ? videoList : pictureList);
        isVideo = checked;
    });
    QHBoxLayout *layout1 = new QHBoxLayout;
    layout1->addWidget(rBtn1);
    layout1->addWidget(rBtn2);
    layout1->addStretch();

    fileView = new FileView(this);

    QPushButton *btn = new QPushButton("开始下载", this);
    connect(btn, &QPushButton::clicked, this, &FileManagerDialog::handlerDownload);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addLayout(layout1);
    layout->addWidget(fileView);
    layout->addWidget(btn, 0, Qt::AlignCenter);
    setFixedWidth(400);

}

void FileManagerDialog::handlerReceiveData(int type, QByteArray data)
{
    switch(type) {
    case QUERYVIDEONAMEDAY: {
        VidiconProtocol::BackUpQueryParameter param;
        param.Type = 1;
        if(ParseXML::getInstance()->parseBackUpQueryParameter(&param, data)) {
            videoList = param.fileList;
            qDebug() << "#FileManagerDialog# handlerReceiveData, ParameterType:" << type << "parse data success...";
        }else {
            qDebug() << "#FileManagerDialog# handlerReceiveData, ParameterType:" << type << "parse data error...";
        }
        break;
    }
    case QUERYPICTURENAMEDAY: {
        VidiconProtocol::BackUpQueryParameter param;
        param.Type = 1;
        if(ParseXML::getInstance()->parseBackUpQueryParameter(&param, data)) {
            pictureList = param.fileList;
            fileView->setDataSource(isVideo ? videoList : pictureList);
            qDebug() << "#FileManagerDialog# handlerReceiveData, ParameterType:" << type << "parse data success...";
            exec();
        }else {
            qDebug() << "#FileManagerDialog# handlerReceiveData, ParameterType:" << type << "parse data error...";
        }
        break;
    }
    default:
        break;
    }
}

void FileManagerDialog::handlerDownload()
{
    QList<FileModel::FileInfo> list = fileView->dataSource();
    foreach (FileModel::FileInfo info, list) {
        if(info.CheckState) {
            qDebug() << info.fileName;
//            QMetaObject::invokeMethod(VidiconProtocol::getInstance(), "downloadFile", Q_ARG(QString, info.fileName));
        }
    }
}
