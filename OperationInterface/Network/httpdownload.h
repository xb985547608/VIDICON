#ifndef HTTP_H
#define HTTP_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QPixmap>
#include <QTime>
#include <QTimer>
#include "operationinterface_global.h"

#define CMD_GETIMAGE  1024
#define CMD_DOWNLOAD  1025

#define DOWNLOADDIR   "download"

enum DownloadState {
    Downloading,
    Waiting,
    Pause,
    Error,
    Finished,
};

class OPERATIONINTERFACESHARED_EXPORT HttpDownload : public QObject
{
    Q_OBJECT
public:
    struct FileStatus {
        qint64  bytesReceived;  //已下载大小
        qint64  bytesTotal;     //总大小
        QString fileName;       //文件名
        QString speed;          //下载速度
        int     percent;        //下载百分比
        int     state;          //文件状态
    };
    static HttpDownload *getInstance() {
        if(s_instance == NULL){
            s_instance = new HttpDownload();
        }
        return s_instance;
    }

    void init();
    QString getDownloadDir() const { return m_downloadDir; }
    Q_INVOKABLE void setDownloadDir(QString dirStr);

    Q_INVOKABLE void getImage(QString path = "/ISAPI/Snap/GetImg0.1");
    Q_INVOKABLE void downloadFile(QString fileName);

    inline bool isLeisure() { return (m_currentCmd == -1); }

    void setHost(QString host) { m_host = host; }
    void setPort(QString port) { m_port = port; }

signals:
    void signalImage(QPixmap *pixmap);
    void signalFileStatus(const FileStatus *m_fileStatus);

public slots:
    void finished(QNetworkReply *m_reply);
    void readyRead();
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void handleTimeout();
    void handleError(QNetworkReply::NetworkError error);
    Q_INVOKABLE void handleCancelDownload(QString file);

private:
    void reset();

private:
    static HttpDownload *s_instance;
    explicit HttpDownload(QObject *parent = nullptr);

    QString m_host;
    QString m_port;
    QNetworkAccessManager *m_manager;
    QNetworkReply *m_reply;

    int m_currentCmd;
    QString m_tempFileName;
    QString m_downloadDir;

    FileStatus m_fileStatus;

    QTime m_downloadTime;
    QTimer *m_timer;
    qint64 m_lastReceiveBytes;
};

#endif // HTTP_H
