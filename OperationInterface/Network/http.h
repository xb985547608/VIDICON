#ifndef HTTP_H
#define HTTP_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QPixmap>
#include "operationinterface_global.h"

#define CMD_GETIMAGE 0x101

class OPERATIONINTERFACESHARED_EXPORT Http : public QObject
{
    Q_OBJECT
public:
    static Http *getInstance(){
        if(_instance == NULL){
            _instance = new Http;
        }
        return _instance;
    }

    void getImage(QUrl url = QUrl("http://192.168.0.66/ISAPI/Snap/GetImg0.1"));

    bool isLeisure();
signals:
    void signalImage(QPixmap pixmap);
public slots:
    void handlerReply(QNetworkReply *reply);

private:
    static Http *_instance;
    explicit Http(QObject *parent = nullptr);

    QNetworkAccessManager *manager;
    int currentCmd;
};

#endif // HTTP_H
