#include "downloadwidget.h"
#include "ui/ui_downloadform.h"
#include <QTimer>
#include <QMetaObject>
#include <QDebug>

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
}

void DownloadWidget::handleTimeout()
{
    HttpDownload *h = HttpDownload::getInstance();
    if(h->isLeisure()) {
        QAbstractItemModel *model = listView->model();
        if(model->data(model->index(0, 1)).toInt() == Waiting) {
            QString file = model->data(model->index(0, 3)).toString();
            model->setData(model->index(0, 1), Downloading);
            QMetaObject::invokeMethod(h, "downloadFile", Q_ARG(QString, file));
            qDebug() << "#DownloadWidget# handleTimeout(), Start Download file --> " << file;
        }
    }
}

//处理当前下载文件的信息
void DownloadWidget::handleReceiveFileStatus(const HttpDownload::FileStatus *fileStatus)
{
    QAbstractItemModel *model = listView->model();

    if(fileStatus->state == Downloading) {
        model->setData(model->index(0, 2), fileStatus->percent);
    }else if(fileStatus->state == Finished) {
        model->setData(model->index(0, 2), 100);
    }
    model->setData(model->index(0, 1), fileStatus->state);
}

void DownloadWidget::enqueue(QStringList files)
{
    foreach (QString file, files) {
        listView->addData(file);
    }
}
