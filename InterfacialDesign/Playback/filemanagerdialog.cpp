#include "filemanagerdialog.h"
#include "Protocol/vidiconprotocol.h"
#include "parsexml.h"
#include <QDebug>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QPushButton>
#include "Network/httpdownload.h"
#include <QKeyEvent>

FileManagerDialog::FileManagerDialog(QWidget *parent) :
    QDialog(parent),
    isVideo(true)
{
    connect(this, &FileManagerDialog::signalGetParameter, VidiconProtocol::getInstance(), &VidiconProtocol::handleGetParameter);
    connect(VidiconProtocol::getInstance(), &VidiconProtocol::signalReceiveData, this, &FileManagerDialog::handleReceiveData);

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
    connect(btn, &QPushButton::clicked, this, &FileManagerDialog::handleDownload);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addLayout(layout1);
    layout->addWidget(fileView);
    layout->addWidget(btn, 0, Qt::AlignCenter);
    resize(400, 600);

}

void FileManagerDialog::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape) {
        close();
    }
}

void FileManagerDialog::handleReceiveData(int type, QByteArray data)
{
    switch(type) {
    case QUERYVIDEONAMEDAY: {
        VidiconProtocol::BackUpQueryParameter param;
        param.Type = 1;
        if(ParseXML::getInstance()->parseBackUpQueryParameter(&param, data)) {
            videoList = param.fileList;
            qDebug() << "#FileManagerDialog# handleReceiveData, ParameterType:" << type << "parse data success...";
        }else {
            qDebug() << "#FileManagerDialog# handleReceiveData, ParameterType:" << type << "parse data error...";
        }
        break;
    }
    case QUERYPICTURENAMEDAY: {
        VidiconProtocol::BackUpQueryParameter param;
        param.Type = 1;
        if(ParseXML::getInstance()->parseBackUpQueryParameter(&param, data)) {
            pictureList = param.fileList;
            fileView->setDataSource(isVideo ? videoList : pictureList);
            qDebug() << "#FileManagerDialog# handleReceiveData, ParameterType:" << type << "parse data success...";
            exec();
        }else {
            qDebug() << "#FileManagerDialog# handleReceiveData, ParameterType:" << type << "parse data error...";
        }
        break;
    }
    default:
        break;
    }
}

void FileManagerDialog::handleDownload()
{
    QList<FileModel::FileInfo> list = fileView->dataSource();
    QStringList files;
    foreach (FileModel::FileInfo info, list) {
        if(info.CheckState) {
            files.append(info.fileName);
        }
    }
//    QMetaObject::invokeMethod(HttpDownload::getInstance(), "downloadFiles", Q_ARG(QStringList, files));
    emit signalAddDownloadTask(files);
}
