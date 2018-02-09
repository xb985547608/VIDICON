#ifndef HTTP_H
#define HTTP_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QPixmap>
#include "operationinterface_global.h"

#define CMD_GETIMAGE  1024
#define CMD_DOWNLOAD  1025

class OPERATIONINTERFACESHARED_EXPORT HttpDownload : public QObject
{
    Q_OBJECT
public:
    typedef struct FileStatus{
        QString bytesReceived;  //已下载大小
        QString bytesTotal;     //总大小
        QString fileName;       //文件名
        QString speed;          //下载速度
        bool    isWaiting;      //文件状态  等待下载or正在下载
        bool    isComplete;     //是否下载完整
        bool    isError;        //是否有错误
    }FileStatus;
    static HttpDownload *getInstance() {
        if(_instance == NULL){
            _instance = new HttpDownload;
        }
        return _instance;
    }

    void init();

    void getImage(QUrl url = QUrl("http://192.168.0.66/ISAPI/Snap/GetImg0.1"));
    void downloadFiles(QStringList list);

    inline bool isLeisure();
signals:
    void signalImage(QPixmap pixmap);
    void signalDownloadProgress();

public slots:
    void finished(QNetworkReply *reply);
    void readyRead();
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

private:
    static HttpDownload *_instance;
    explicit HttpDownload(QObject *parent = nullptr);

    QString host;
    QString port;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;

    QStringList fileList;
    int currentCmd;
};

#endif // HTTP_H
