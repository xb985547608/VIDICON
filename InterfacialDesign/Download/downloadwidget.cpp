#include "downloadwidget.h"
#include "ui/ui_downloadform.h"
#include <QTimer>
#include <QMetaObject>
#include <QDebug>
#include <QMessageBox>
#include <QDir>
#include <QDesktopServices>
#include "soundeffect.h"

DownloadWidget::DownloadWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DownloadForm)
{
    ui->setupUi(this);

    listView = new DownloadInfoView(ui->listWidget);

    QHBoxLayout *hl = new QHBoxLayout(ui->listWidget);
    hl->addWidget(listView);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &DownloadWidget::handleTimeout);
    timer->start(500);

    connect(HttpDownload::getInstance(), &HttpDownload::signalFileStatus, this, &DownloadWidget::handleReceiveFileStatus);
    connect(listView, &DownloadInfoView::signalCancelDownload, HttpDownload::getInstance(), &HttpDownload::handleCancelDownload);

    connect(ui->allDeleteBtn, &QPushButton::clicked, this, [this](){
        QMetaObject::invokeMethod(HttpDownload::getInstance(), "handleCancelDownload", Q_ARG(QString, listView->data(0, 3).toString()));
        if(QMessageBox::warning(this, "警告", "是否删除所有下载任务", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
            listView->model()->removeRows(0, listView->model()->rowCount());
        }
    });
    connect(ui->allPauseBtn, &QPushButton::clicked, this, [this](){
        int rowCount = listView->model()->rowCount();
        for(int i=1; i< rowCount; i++) {
            if(listView->data(1, 1) > Waiting)
                break;
            listView->setData(listView->model()->index(1, 1), Pause);
        }
    });
    connect(ui->allStartBtn, &QPushButton::clicked, this, [this](){
        int rowCount = listView->model()->rowCount();
        if(listView->data(0, 1) != Downloading) {
            listView->setData(listView->model()->index(0, 1), Waiting);
        }
        for(int i=1; i< rowCount; i++) {
            if(listView->data(i, 1) > Pause)
                break;
            listView->setData(listView->model()->index(i, 1), Waiting);
        }
    });
    connect(ui->openFolderBtn, &QPushButton::clicked, this, [this](){
        QDesktopServices::openUrl(QUrl(HttpDownload::getInstance()->getDownloadDir()));
    });
}

void DownloadWidget::handleTimeout()
{
    HttpDownload *h = HttpDownload::getInstance();
    if(h->isLeisure()) {
        QAbstractItemModel *model = listView->model();
        if(listView->data(model->index(0, 1)).toInt() == Waiting) {
            QString file = listView->data(model->index(0, 3)).toString();
            listView->setData(model->index(0, 1), Downloading);
            QMetaObject::invokeMethod(h, "downloadFile", Q_ARG(QString, file));
            qDebug() << "#DownloadWidget# handleTimeout(), Start Download file --> " << file;
        }
    }
}

//处理当前下载文件的信息
void DownloadWidget::handleReceiveFileStatus(const HttpDownload::FileStatus *fileStatus)
{
    QAbstractItemModel *model = listView->model();
    ui->lblSpeed->setText(fileStatus->speed);
    if(fileStatus->state == Downloading) {
        listView->setData(model->index(0, 2), fileStatus->percent);
    }else if(fileStatus->state == Finished) {
        listView->setData(model->index(0, 2), 100);
        SoundEffect::getInstance()->triggerSoundEffect(SoundEffect::Success);
        ui->lblSpeed->setText("");
    }else if(fileStatus->state == Error) {
        SoundEffect::getInstance()->triggerSoundEffect(SoundEffect::Error);
        ui->lblSpeed->setText("");
    }
    listView->setData(model->index(0, 1), fileStatus->state);
}

void DownloadWidget::enqueue(QStringList files)
{
    foreach (QString file, files) {
        listView->addData(file);
    }
}
