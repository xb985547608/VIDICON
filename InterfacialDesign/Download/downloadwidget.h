#ifndef DOWNLOADWIDGET_H
#define DOWNLOADWIDGET_H

#include <QWidget>
#include "downloadinfoview.h"
#include <QMap>
#include "Network/httpdownload.h"
#include <QSound>
namespace Ui {
class DownloadForm;
}

class DownloadWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DownloadWidget(QWidget *parent = nullptr);

signals:

public slots:
    void handleTimeout();
    void handleReceiveFileStatus(const HttpDownload::FileStatus *fileStatus);
    void enqueue(QStringList files);

private:
    Ui::DownloadForm *ui;
    DownloadInfoView *listView;
    QMap<int, QString> waitingQueue;
    QSound *successHintVoice;
    QSound *errorHintVoice;
};

#endif // DOWNLOADWIDGET_H
