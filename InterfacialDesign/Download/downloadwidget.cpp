#include "downloadwidget.h"
#include "ui/ui_downloadform.h"
#include <QTimer>
#include <QMetaObject>
#include <QDebug>
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>
#include <QDesktopServices>
#include "soundeffect.h"
#include "statustip.h"

DownloadWidget::DownloadWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DownloadForm)
{
    ui->setupUi(this);

    listView = new DownloadInfoView(ui->listWidget);

    QHBoxLayout *hl = new QHBoxLayout(ui->listWidget);
    hl->addWidget(listView);

    //开始周期500ms的巡检
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
    connect(ui->changeDirBtn, &QPushButton::clicked, this, [this](){
        QString dirStr = QFileDialog::getExistingDirectory(this, "选择存放下载文件的目录", QDir::currentPath());
        QMetaObject::invokeMethod(HttpDownload::getInstance(), "setDownloadDir", Q_ARG(QString, dirStr));

        refreshDownloadDirLabel(dirStr);
    });
}

void DownloadWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    refreshDownloadDirLabel();
}

void DownloadWidget::refreshDownloadDirLabel(QString dirStr)
{
    //显示省略的下载目录
    if (dirStr.isNull())
        dirStr = HttpDownload::getInstance()->getDownloadDir();
    dirStr = QString("存储目录：%1").arg(dirStr);
    ui->lblDir->setToolTip(dirStr);
//    dirStr = ui->lblDir->fontMetrics().elidedText(dirStr, Qt::ElideRight, ui->lblDir->width());
    ui->lblDir->setText(dirStr);
}

void DownloadWidget::handleTimeout()
{
    HttpDownload *h = HttpDownload::getInstance();
    if(h->isLeisure()) {
        QAbstractItemModel *model = listView->model();
        //如果列表中第一行的文件正在等待下载，则开始下载该文件
        if(listView->data(model->index(0, 1)).toInt() == Waiting) {
            QString file = listView->data(model->index(0, 3)).toString();
            listView->setData(model->index(0, 1), Downloading);
            QMetaObject::invokeMethod(h, "downloadFile", Q_ARG(QString, file));
            qDebug() << "#DownloadWidget# handleTimeout(), Start Download file --> " << file;

            StatusTip::getInstance()->showStatusTip(QString("开始下载文件：%1").arg(file));
        }
    }
}

//处理当前下载文件的信息
void DownloadWidget::handleReceiveFileStatus(const HttpDownload::FileStatus *fileStatus)
{
    QAbstractItemModel *model = listView->model();
    ui->lblSpeed->setText(fileStatus->speed);
    //根据文件状态的不同做不同的处理
    if(fileStatus->state == Downloading) {
        listView->setData(model->index(0, 2), fileStatus->percent);
    }else if(fileStatus->state == Finished) {
        listView->setData(model->index(0, 2), 100);
        SoundEffect::getInstance()->triggerSoundEffect(SoundEffect::Success);
        ui->lblSpeed->setText("");
        StatusTip::getInstance()->showStatusTip(QString("文件下载完成：%1").arg(fileStatus->fileName));
    }else if(fileStatus->state == Error) {
        SoundEffect::getInstance()->triggerSoundEffect(SoundEffect::Error);
        ui->lblSpeed->setText("");
        StatusTip::getInstance()->showStatusTip(QString("文件下载出错：%1").arg(fileStatus->fileName));
    }

    //更新文件状态
    listView->setData(model->index(0, 1), fileStatus->state);
}

void DownloadWidget::enqueue(QStringList files)
{
    foreach (QString file, files) {
        listView->addData(file);
    }
}
