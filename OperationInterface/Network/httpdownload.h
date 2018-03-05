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
    typedef struct FileStatus {
        qint64  bytesReceived;  //已下载大小
        qint64  bytesTotal;     //总大小
        QString fileName;       //文件名
        QString speed;          //下载速度
        int     percent;        //下载百分比
        int     state;          //文件状态
    }FileStatus;
    static HttpDownload *getInstance(QString host = "", int port = -1) {
        if(_instance == NULL){
            _instance = new HttpDownload(host, QString::number(port));
        }
        return _instance;
    }

    void init();
    QString getDownloadDir() const { return downloadDir; }

    Q_INVOKABLE void getImage(QString path = "/ISAPI/Snap/GetImg0.1");
    Q_INVOKABLE void downloadFile(QString fileName);

    inline bool isLeisure() { return (currentCmd == -1); }
signals:
    void signalImage(QPixmap *pixmap);
    void signalFileStatus(const FileStatus *fileStatus);

public slots:
    void finished(QNetworkReply *reply);
    void readyRead();
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void handleTimeout();
    Q_INVOKABLE void handleCancelDownload(QString file);

private:
    static HttpDownload *_instance;
    explicit HttpDownload(QString host, QString port, QObject *parent = nullptr);

    QString host;
    QString port;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;

    int currentCmd;
    QString tempFileName;
    QString downloadDir;

    FileStatus fileStatus;

    QTime downloadTime;
    QTimer *timer;
    qint64 lastReceiveBytes;
};

#endif // HTTP_H
