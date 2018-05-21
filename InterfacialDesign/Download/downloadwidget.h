#ifndef DOWNLOADWIDGET_H
#define DOWNLOADWIDGET_H

/**
 * @brief         布局下载界面，处理下载任务
 * @author        xiaobin <xiaobin@sunniwell.com>
 * @date          20180504
 */

#include <QWidget>
#include "downloadinfoview.h"
#include <QMap>
#include "Network/httpdownload.h"

namespace Ui {
class DownloadForm;
}

class DownloadWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DownloadWidget(QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event);

private:
    void refreshDownloadDirLabel(QString dirStr = QString());

signals:

public slots:
    //周期为一秒，检查是否需要开始新的下载
    void handleTimeout();
    void handleReceiveFileStatus(const HttpDownload::FileStatus *fileStatus);
    //将文件列表中的所有文件加入下载队列
    void enqueue(QStringList files);

private:
    Ui::DownloadForm *ui;
    DownloadInfoView *m_listView;
    QMap<int, QString> m_waitingQueue;
};

#endif // DOWNLOADWIDGET_H
