#include "httpdownload.h"
#include <QApplication>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include "util.h"

HttpDownload *HttpDownload::_instance = NULL;
HttpDownload::HttpDownload(QString host, QString port, QObject *parent) : QObject(parent),
    host(host), port(port), currentCmd(-1)
{

}

void HttpDownload::init()
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &HttpDownload::finished);
    downloadDir = QString("%1/%2").arg(qApp->applicationDirPath()).arg(DOWNLOADDIR);
    //确保download目录的存在
    QDir dir(downloadDir);
    if(!dir.exists()) {
        dir.cdUp();
        dir.mkdir(DOWNLOADDIR);
    }
    //清除临时文件
    dir.cd(DOWNLOADDIR);
    QStringList filter;
    filter << "*.tmp";
    QFileInfoList list = dir.entryInfoList(filter, QDir::Files | QDir::NoDotAndDotDot);
    foreach (QFileInfo info, list) {
        QFile::remove(info.absoluteFilePath());
    }

    //该定时器用来计算每秒平均下载速率
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &HttpDownload::handleTimeout);
}

void HttpDownload::setDownloadDir(QString dirStr)
{
    downloadDir = dirStr;
    QDir dir(downloadDir);
    if(!dir.exists()) {
        dir.cdUp();
        dir.mkdir(DOWNLOADDIR);
    }
}

void HttpDownload::getImage(QString path)
{
    while(!isLeisure()) {
        qApp->processEvents();
    }
    reply = manager->get(QNetworkRequest(QUrl(QString("http://%1:%2/%3")
                                         .arg(host)
                                         .arg(port)
                                         .arg(path))));
    currentCmd = CMD_GETIMAGE;
}

void HttpDownload::downloadFile(QString fileName)
{
    if(fileName.isNull())
        return;

    while(!isLeisure()) {
        qApp->processEvents();
    }
    QStringList list = fileName.split('/');
    fileStatus.fileName = list.at(list.length() - 1);
    fileStatus.state = Downloading;

    QString date = QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss-zzz");
    tempFileName = QString("%1/%2.tmp").arg(downloadDir).arg(date);
    currentCmd = CMD_DOWNLOAD;

    reply = manager->get(QNetworkRequest(QUrl(QString("http://%1:%2/record/%3")
                                              .arg(host)
                                              .arg(port)
                                              .arg(fileName))));
    //测试链接
    //reply = manager->get(QNetworkRequest(QUrl("http://sw.bos.baidu.com/sw-search-sp/software/06da2b30f1c74/BaiduNetdisk_5.7.3.1.exe")));
    connect(reply, &QNetworkReply::readyRead, this, &HttpDownload::readyRead);
    connect(reply, &QNetworkReply::downloadProgress, this, &HttpDownload::downloadProgress);

    downloadTime.start();
    timer->start(1000);
}

void HttpDownload::finished(QNetworkReply */*reply*/)
{
    if(reply->error() != QNetworkReply::NoError) {
        qDebug() << "#HttpDownload# finished,"
                 << "StatusCode:" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()
                 << "ErrorType:" << reply->error();
        if(currentCmd == CMD_DOWNLOAD) {
            fileStatus.state = Error;
            emit signalFileStatus(&fileStatus);
        }
    }else {
        switch (currentCmd) {
        case CMD_GETIMAGE: {
            QPixmap *pixmap = new QPixmap;
            if(pixmap->loadFromData(reply->readAll())) {
                emit signalImage(pixmap);
            }
            break;
        }
        case CMD_DOWNLOAD: {
            QFileInfo fileInfo(tempFileName);
            QFileInfo newFileInfo = fileInfo.absolutePath() + "/" + fileStatus.fileName;
            QDir dir;
            if(dir.exists(fileInfo.absolutePath())) {
                if(newFileInfo.exists()) {
                    newFileInfo.dir().remove(newFileInfo.fileName());
                }
                QFile::rename(tempFileName, newFileInfo.absoluteFilePath());
                qDebug() << "#HttpDownload# finished(), Download file" << newFileInfo.fileName() << "Success !!";
                fileStatus.state = Finished;
                emit signalFileStatus(&fileStatus);
            }
            break;
        }
        default:
            break;
        }
    }
    if(currentCmd == CMD_DOWNLOAD) {
        lastReceiveBytes = 0;
        if(timer->isActive()) {
            timer->stop();
        }
    }
    currentCmd = -1;
    reply = NULL;
}

void HttpDownload::readyRead()
{
    if(currentCmd == CMD_DOWNLOAD) {
        QFile file(tempFileName);
        if(file.open(QIODevice::Append)) {
            file.write(reply->readAll());
            file.close();
        }else {
            qDebug() << "#HttpDownload# readyRead(), Open file error" << tempFileName;
        }
    }
}

void HttpDownload::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    if(currentCmd == CMD_DOWNLOAD) {
        fileStatus.bytesReceived = bytesReceived;
        fileStatus.bytesTotal = bytesTotal;
        fileStatus.percent = bytesReceived * 100 / bytesTotal;
    }
}

void HttpDownload::handleTimeout()
{
    //下载的每秒瞬时速度
    qreal s = fileStatus.bytesReceived - lastReceiveBytes;
    //记录本次字节数
    lastReceiveBytes = fileStatus.bytesReceived;
    //将速度转为字符串
    fileStatus.speed = speed(s);
    emit signalFileStatus(&fileStatus);
}

void HttpDownload::handleCancelDownload(QString file)
{
    if(file.isNull())
        return;

    QStringList list = file.split('/');
    if(fileStatus.fileName == list.at(list.length() - 1)) {
        reply->abort();
    }
}
