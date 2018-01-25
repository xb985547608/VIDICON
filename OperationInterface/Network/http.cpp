#include "http.h"

Http *Http::_instance = NULL;
Http::Http(QObject *parent) : QObject(parent),
    currentCmd(-1)
{
    manager = new QNetworkAccessManager(this);

    connect(manager, &QNetworkAccessManager::finished, this, &Http::handlerReply);
}

void Http::getImage(QUrl url)
{
    manager->get(QNetworkRequest(url));
    currentCmd = CMD_GETIMAGE;
}

bool Http::isLeisure()
{
    if(currentCmd != -1) {
        return false;
    }else{
        return true;
    }
}

void Http::handlerReply(QNetworkReply *reply)
{

    switch (currentCmd) {
    case CMD_GETIMAGE:
        if(reply->error() == QNetworkReply::NoError){
            QPixmap pixmap;
            if(pixmap.loadFromData(reply->readAll())){
                emit signalImage(pixmap);
            }
        }
        break;
    default:
        break;
    }
    currentCmd = -1;
}
