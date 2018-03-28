#ifndef FILEMANAGERDIALOG_H
#define FILEMANAGERDIALOG_H

#include <QDialog>
#include <QMap>
#include <QPointer>
#include <QEventLoop>
#include "filemanagerview.h"
#include "basicwidget.h"

class FileManagerDialog : public BasicWidget
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
