#include "httpdownload.h"
#include <QApplication>
#include <QFileInfo>
#include <QFile>
#include <QDir>

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

void HttpDownload::downloadFiles(QStringList list)
{
    if(!list.length())
        return;

    foreach (QString file, list) {
        FileStatus status;
        status.fileName = file;
        status.isWaiting = true;
        status.isComplete = false;
        status.isError = false;
        fileStatusList.append(status);
    }
    emit signalFileStatusList(fileStatusList);

    do {
        while(!isLeisure()) {
            qApp->processEvents();
        }
        QString date = QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss-zzz");
        tempFileName = QString("%1/%2.tmp").arg(downloadDir).arg(date);
        fileStatusList[0].isWaiting = false;
        currentCmd = CMD_DOWNLOAD;
        reply = manager->get(QNetworkRequest(QUrl(QString("http://%1:%2/record/%3")
                                                  .arg(host)
                                                  .arg(port)
                                                  .arg(fileStatusList[0].fileName))));
        connect(reply, &QNetworkReply::readyRead, this, &HttpDownload::readyRead);
        connect(reply, &QNetworkReply::downloadProgress, this, &HttpDownload::downloadProgress);
    }while(fileStatusList.length() != 1);
}

void HttpDownload::finished(QNetworkReply */*reply*/)
{
    if(reply->error() != QNetworkReply::NoError) {
        qDebug() << "#HttpDownload# finished,"
                 << "StatusCode:" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()
                 << "ErrorType:" << reply->error();
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
            QFileInfo newFileInfo = fileInfo.absolutePath() + "/" + fileStatusList[0].fileName;
            QDir dir;
            if(dir.exists(fileInfo.absolutePath())) {
                if(newFileInfo.exists()) {
                    newFileInfo.dir().remove(newFileInfo.fileName());
                }
                QFile::rename(tempFileName, newFileInfo.absoluteFilePath());
                qDebug() << "#HttpDownload# finished(), Download file" << newFileInfo.fileName() << "Success !!";
            }
            break;
        }
        default:
            break;
        }
    }
    if(currentCmd == CMD_DOWNLOAD)
        fileStatusList.removeAt(0);
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
        qDebug() << bytesReceived << bytesTotal;
    }
}
