#ifndef PREPARETHREAD_H
#define PREPARETHREAD_H

#include <QThread>

class PrepareThread : public QThread
{
    Q_OBJECT
public:
    explicit PrepareThread(QObject *parent = nullptr);

signals:
private:
    virtual void run();

public slots:
};

#endif // PREPARETHREAD_H
