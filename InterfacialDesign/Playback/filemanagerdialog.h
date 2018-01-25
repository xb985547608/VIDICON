#ifndef FILEMANAGERDIALOG_H
#define FILEMANAGERDIALOG_H

#include <QDialog>
#include <QMap>
#include "filemanagerview.h"

class FileManagerDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FileManagerDialog(QWidget *parent = nullptr);

signals:
    void signalSetParameter(int type, void *param, QString SessionID = "R00001");
    void signalGetParameter(int type, int StreamType = 0, int Channel = 0, QString SessionID = "R00001");

public slots:
    void handlerReceiveData(int type, QByteArray data);

private:
    QStringList fileList;
    FileView *fileView;
};

#endif // FILEMANAGERDIALOG_H
