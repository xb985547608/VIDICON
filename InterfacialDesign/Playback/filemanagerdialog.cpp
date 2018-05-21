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
    BaseWidget(parent),
    m_isVideo(true)
{
    Qt::WindowFlags flags = windowFlags();
    setWindowFlags(flags | Qt::Dialog);
    setAttribute(Qt::WA_DeleteOnClose, false);

    QRadioButton *rBtn1 = new QRadioButton("视频", this);
    rBtn1->setChecked(true);
    QRadioButton *rBtn2 = new QRadioButton("图片", this);
    connect(rBtn1, &QRadioButton::toggled, this, [this](bool checked) {
        m_fileView->setDataSource(checked ? m_videoItems : m_pictureItems);
        m_isVideo = checked;

        if (m_isVideo)
            m_fileView->horizontalHeader()->hideSection(2);
        else
            m_fileView->horizontalHeader()->showSection(2);
    });
    QHBoxLayout *layout1 = new QHBoxLayout;
    layout1->addWidget(rBtn1);
    layout1->addWidget(rBtn2);
    layout1->addStretch();

    m_fileView = new FileView(this);

    QPushButton *btn = new QPushButton("开始下载", this);
    connect(btn, &QPushButton::clicked, this, &FileManagerDialog::handleDownload);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addLayout(layout1);
    layout->addWidget(m_fileView);
    layout->addWidget(btn, 0, Qt::AlignCenter);
    resize(400, 600);

    hide();
}

void FileManagerDialog::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape) {
        close();
    }
}

void FileManagerDialog::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    if (!m_eventLoop.isNull()) {
        if (m_eventLoop.data()->isRunning())
            m_eventLoop.data()->quit();
    }
}

void FileManagerDialog::handleReceiveData(VidiconProtocol::Type type, QByteArray data)
{
    bool isOK = false;

    switch(type) {
    case VidiconProtocol::QUERYVIDEONAMEDAY: {
        BackUpQueryParameter param;
        param.Type = 1;
        isOK = ParseXML::getInstance()->parseBackUpQueryParameter(&param, data);
        if (isOK) {
            m_videoItems = param.fileList;
        }
        break;
    }
    case VidiconProtocol::QUERYPICTURENAMEDAY: {
        BackUpQueryParameter param;
        param.Type = 1;
        isOK = ParseXML::getInstance()->parseBackUpQueryParameter(&param, data);
        if (isOK) {
            m_pictureItems = param.fileList;
            m_fileView->setDataSource(m_videoItems);
            m_fileView->horizontalHeader()->hideSection(2);
            exec();
        }
        break;
    }
    default:
        return;
    }

    if (isOK)
        qDebug() << "#FileManagerDialog# handleReceiveData, ParameterType:" << type << "parse data success...";
    else
        qDebug() << "#FileManagerDialog# handleReceiveData, ParameterType:" << type << "parse data error...";

}

void FileManagerDialog::handleDownload()
{
    QList<FileModel::FileInfo> list = m_fileView->dataSource();
    QStringList files;
    foreach (FileModel::FileInfo info, list) {
        if(info.CheckState) {
            files.append(info.fileName);
        }
    }
//    QMetaObject::invokeMethod(HttpDownload::getInstance(), "downloadFiles", Q_ARG(QStringList, files));
    emit signalAddDownloadTask(files);
}

int FileManagerDialog::exec()
{
    if (!m_eventLoop.isNull()) {
        qWarning("FileManagerDialog::exec: Recursive call detected");
        return -1;
    }

    //接收CloseEvent时不删除自身
    bool deleteOnClose = testAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_DeleteOnClose, false);

    bool wasShowModal = testAttribute(Qt::WA_ShowModal);
    setAttribute(Qt::WA_ShowModal, true);
//    setWindowModality(Qt::WindowModal);
    show();

    QPointer<FileManagerDialog> guard = this;

    QEventLoop loop;
    m_eventLoop = &loop;
    loop.exec(QEventLoop::DialogExec);

    if (guard.isNull())
        return QDialog::Rejected;

    m_eventLoop = 0;

    setAttribute(Qt::WA_ShowModal, wasShowModal);

    if (deleteOnClose)
        delete this;
    return QDialog::Accepted;
}
