#include "httpdownload.h"
#include <QApplication>

HttpDownload *HttpDownload::_instance = NULL;
HttpDownload::HttpDownload(QObject *parent) : QObject(parent),
    currentCmd(-1)
{

}

void HttpDownload::init()
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &HttpDownload::handlerReply);
}

void HttpDownload::getImage(QUrl url)
{
    manager->get(QNetworkRequest(url));
    currentCmd = CMD_GETIMAGE;
}

void HttpDownload::downloadFiles(QStringList list)
{
    foreach (QString file, list) {
        while(!isLeisure()) {
            qApp->processEvents();
        }
        currentCmd = CMD_DOWNLOAD;

        manager->get(QNetworkRequest(QUrl(QString("http://%1:%2/record/%3")
                                          .arg(host)
                                          .arg(port)
                                          .arg(file))));
    }
}

bool HttpDownload::isLeisure()
{
    return (currentCmd == -1);
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
            QPixmap pixmap;
            if(pixmap.loadFromData(reply->readAll())) {
                emit signalImage(pixmap);
            }
            break;
        }
        case CMD_DOWNLOAD: {

            break;
        }
        default:
            break;
        }
    }

    currentCmd = -1;
}

void HttpDownload::readyRead()
{

}

void HttpDownload::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{

}
