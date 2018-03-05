#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QStringList>
#include "operationinterface_global.h"

//打开调试日志输出
#define LOG_DEBUG       0
//日志文件个数和文件大小限制
#define LOG_MAX_SIZE            52428800  //50M
#define LOG_FILE_MAX_COUNT      4
//日志目录和文件名称
#define LOG_DIR_NAME           "log"
#define LOG_FILE_NAME           "VIDICON"

class OPERATIONINTERFACESHARED_EXPORT Logger : public QThread
{
    Q_OBJECT
private:
    static Logger* _instance;
    Logger();
    void run();
    int getLogIndex();
    static QMutex *lock;

    bool checkLogDir();

public:
    ~Logger();
    static Logger * getInstance()
    {
        if(NULL == _instance)
        {
            QMutexLocker locker(lock);
            _instance = new Logger();
            _instance->start();
        }
        return _instance;
    }
    void log(QtMsgType type, const char *msg);    
    void log(QtMsgType type, const QMessageLogContext &context, const QString &msg);

private:
    QMutex lockLog;
    QMutex lockWrite;
    QStringList msgBuffer1;
    QStringList msgBuffer2;
    QStringList *currentLog;
    QStringList *currentWrite;
    bool isRun;

signals:
    
public slots:
    
};

#endif // LOGGER_H
