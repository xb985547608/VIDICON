#ifndef FILEMANAGERDIALOG_H
#define FILEMANAGERDIALOG_H

/**
 * @brief         弹出对话框，显示文件列表
 * @author        xiaobin <xiaobin@sunniwell.com>
 * @date          20180504
 */

#include <QDialog>
#include <QMap>
#include <QPointer>
#include <QEventLoop>
#include "filemanagerview.h"
#include "basewidget.h"

class FileManagerDialog : public BaseWidget
{
    Q_OBJECT
public:
    explicit FileManagerDialog(QWidget *parent = nullptr);


protected:
    void keyPressEvent(QKeyEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

signals:
    void signalAddDownloadTask(QStringList files);

public slots:
    void handleReceiveData(VidiconProtocol::Type type, QByteArray data);
    void handleDownload();
    int exec();

private:
    QStringList m_items;
    FileView *m_fileView;

    QStringList m_videoItems;
    QStringList m_pictureItems;
    bool m_isVideo;

    QPointer<QEventLoop> m_eventLoop;
};

#endif // FILEMANAGERDIALOG_H
