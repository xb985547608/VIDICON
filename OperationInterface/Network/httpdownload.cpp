#include "httpdownload.h"
#include <QApplication>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include "util.h"

HttpDownload *HttpDownload::s_instance = NULL;
HttpDownload::HttpDownload(QObject *parent) : QObject(parent),
    m_host("192.168.0.66"), m_port("80"), m_currentCmd(-1)
{

}

void HttpDownload::init()
{
    m_manager = new QNetworkAccessManager(this);
    connect(m_manager, &QNetworkAccessManager::finished, this, &HttpDownload::finished);

    //确保download目录的存在
    m_downloadDir = QString("%1/%2").arg(qApp->applicationDirPath()).arg(DOWNLOADDIR);
    QDir dir(m_downloadDir);
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
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &HttpDownload::handleTimeout);
}

void HttpDownload::setDownloadDir(QString dirStr)
{
    m_downloadDir = dirStr;
    QDir dir(m_downloadDir);
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
    m_reply = m_manager->get(QNetworkRequest(QUrl(QString("http://%1:%2%3")
                                         .arg(m_host)
                                         .arg(m_port)
                                         .arg(path))));
    m_currentCmd = CMD_GETIMAGE;
}

void HttpDownload::downloadFile(QString fileName)
{
    if(fileName.isNull())
        return;

    while(!isLeisure()) {
        qApp->processEvents();
    }
    QStringList list = fileName.split('/');
    m_fileStatus.fileName = list.at(list.length() - 1);
    m_fileStatus.state = Downloading;
    m_fileStatus.bytesReceived = 0;
    m_fileStatus.bytesTotal = 0;
    m_fileStatus.percent = 0;
    m_fileStatus.speed = speed(0);

    QString date = QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss-zzz");
    m_tempFileName = QString("%1/%2.tmp").arg(m_downloadDir).arg(date);
    m_currentCmd = CMD_DOWNLOAD;

    if (fileName.right(4).compare(".jpg") == 0){
        m_reply = m_manager->get(QNetworkRequest(QUrl(QString("http://%1:%2/record/image/%3")
                                                      .arg(m_host)
                                                      .arg(m_port)
                                                      .arg(fileName))));
    } else {
        m_reply = m_manager->get(QNetworkRequest(QUrl(QString("http://%1:%2/record/%3")
                                                      .arg(m_host)
                                                      .arg(m_port)
                                                      .arg(fileName))));
    }


    //测试链接
//    reply = manager->get(QNetworkRequest(QUrl("http://sw.bos.baidu.com/sw-search-sp/software/06da2b30f1c74/BaiduNetdisk_5.7.3.1.exe")));
    connect(m_reply, &QNetworkReply::readyRead, this, &HttpDownload::readyRead);
    connect(m_reply, &QNetworkReply::downloadProgress, this, &HttpDownload::downloadProgress);
    connect(m_reply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), this, &HttpDownload::handleError);

    m_downloadTime.start();
    m_timer->start(1000);
}

void HttpDownload::finished(QNetworkReply */*reply*/)
{
    if (m_reply != NULL) {
        switch (m_currentCmd) {
            case CMD_GETIMAGE: {
                QPixmap *pixmap = new QPixmap;
                if(pixmap->loadFromData(m_reply->readAll())) {
                    emit signalImage(pixmap);
                }
                break;
            }
            case CMD_DOWNLOAD: {
                QFileInfo fileInfo(m_tempFileName);
                QFileInfo newFileInfo = fileInfo.absolutePath() + "/" + m_fileStatus.fileName;
                QDir dir;
                if(dir.exists(fileInfo.absolutePath())) {
                    if(newFileInfo.exists()) {
                        newFileInfo.dir().remove(newFileInfo.fileName());
                    }
                    QFile::rename(m_tempFileName, newFileInfo.absoluteFilePath());
                    qDebug() << "#HttpDownload# finished(), Download file" << newFileInfo.fileName() << "Success !!";
                    m_fileStatus.state = Finished;
                    emit signalFileStatus(&m_fileStatus);
                }
                break;
            }
            default:
                break;
        }
        reset();
    }
}

void HttpDownload::readyRead()
{
    if(m_currentCmd == CMD_DOWNLOAD) {
        QFile file(m_tempFileName);
        if(file.open(QIODevice::Append)) {
            file.write(m_reply->readAll());
            file.close();
        }else {
            qDebug() << "#HttpDownload# readyRead(), Open file error" << m_tempFileName;
        }
    }
}

void HttpDownload::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    if(m_currentCmd == CMD_DOWNLOAD) {
        m_fileStatus.bytesReceived = bytesReceived;
        m_fileStatus.bytesTotal = bytesTotal;
        m_fileStatus.percent = bytesReceived * 100 / bytesTotal;
    }
}

void HttpDownload::handleTimeout()
{
    //下载的每秒瞬时速度
    qreal s = m_fileStatus.bytesReceived - m_lastReceiveBytes;
    //记录本次字节数
    m_lastReceiveBytes = m_fileStatus.bytesReceived;
    //将速度转为字符串
    m_fileStatus.speed = speed(s);
    emit signalFileStatus(&m_fileStatus);
}

void HttpDownload::handleError(QNetworkReply::NetworkError error)
{
    qDebug() << "#HttpDownload# finished,"
             << "StatusCode:" << m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()
             << "ErrorType:" << error;
    if (m_currentCmd == CMD_DOWNLOAD) {
        m_fileStatus.state = Error;
        emit signalFileStatus(&m_fileStatus);
    }
    reset();
}

void HttpDownload::handleCancelDownload(QString file)
{
    if(file.isNull())
        return;

    QStringList list = file.split('/');
    if(m_fileStatus.fileName == list.at(list.length() - 1)) {
        m_reply->abort();
    }
}

void HttpDownload::reset()
{
    if(m_currentCmd == CMD_DOWNLOAD) {
        m_lastReceiveBytes = 0;
        if(m_timer->isActive()) {
            m_timer->stop();
        }
    }
    m_currentCmd = -1;
    m_reply = NULL;
}
