#ifndef DOWNLOADTHREAD_H
#define DOWNLOADTHREAD_H

#include <QThread>

class DownloadThread : public QThread
{
    Q_OBJECT
public:
    explicit DownloadThread(QObject *parent = nullptr);

protected:
    void run() override;
signals:

public slots:
};

#endif // DOWNLOADTHREAD_H
