#include "logger.h"
#include <QTime>
#include <QDate>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QApplication>
#include <QTextStream>
#include <stdio.h>
#include <QDebug>

QMutex* Logger::lock = new QMutex();
Logger* Logger::_instance = NULL;
Logger::Logger()
{
    isRun = true;
    currentLog = &msgBuffer1;
    currentWrite = &msgBuffer2;
    checkLogDir();
}

Logger::~Logger()
{
    isRun = false;
    this->wait();

    _instance = NULL;
}

void Logger::run()
{
    printf("#Logger# logger thread start!\n");
    QString logFileName;
    //获取记录日志的文件名
    if (logFileName.isNull() || logFileName.length() == 0) {
        QString logName = QString("%1/%2/%3_%4.log")
                .arg(QCoreApplication::applicationDirPath())
                .arg(LOG_DIR_NAME)
                .arg(LOG_FILE_NAME)
                .arg(getLogIndex());
        logFileName = logName;
    }
    QFile outFile(logFileName);
    //判断输出文件的大小是否超过限制
    if (outFile.size() > LOG_MAX_SIZE) {
        QString logName = QString("%1/%2/%3_%4.log")
                .arg(QCoreApplication::applicationDirPath())
                .arg(LOG_DIR_NAME)
                .arg(LOG_FILE_NAME)
                .arg(getLogIndex());
        logFileName = logName;
        outFile.setFileName(logFileName);
    }
    //已追加写入的方式打开输出文件
    if (!(outFile.open(QIODevice::WriteOnly | QIODevice::Append))) {
        printf("#Logger# init logger error! \n");
        isRun = false;
        return;
    }
    qint64 fileLen = outFile.size();

    bool isOpenLogFile = true;
    while(true) {
        lockWrite.lock();
        //有数据等待写入
        while (currentWrite->length() > 0) {
            //如果日志文件不存在或无法写入
            if (!(outFile.exists()) || !(outFile.isWritable())) {
                //获取可用的日志文件名
                outFile.close();
                QString logName = QString("%1/%2/%3_%4.log")
                        .arg(QCoreApplication::applicationDirPath())
                        .arg(LOG_DIR_NAME)
                        .arg(LOG_FILE_NAME)
                        .arg(getLogIndex());
                logFileName = logName;
                outFile.setFileName(logFileName);

                if (!(outFile.open(QIODevice::WriteOnly | QIODevice::Append))) {
                    isOpenLogFile = false;
                    break;
                } else {
                    isOpenLogFile = true;
                    fileLen = outFile.size();
                }
            }
            //写入日志文件
            QString text = currentWrite->first();
            fileLen += outFile.write(text.toStdString().data(), text.length());
            outFile.flush();

            if (fileLen > LOG_MAX_SIZE) {
                outFile.close();
                QString logName = QString("%1/%2/%3_%4.log")
                        .arg(QCoreApplication::applicationDirPath())
                        .arg(LOG_DIR_NAME)
                        .arg(LOG_FILE_NAME)
                        .arg(getLogIndex());
                logFileName = logName;
                outFile.setFileName(logFileName);

                if (!(outFile.open(QIODevice::WriteOnly | QIODevice::Append))) {
                    isOpenLogFile = false;
                    break;
                } else {
                    isOpenLogFile = true;
                    fileLen = outFile.size();
                }
            }
            currentWrite->removeFirst();
        }
        lockWrite.unlock();

        //打开日志文件失败，退出循环
        if (!isOpenLogFile)
            break;

        if (!isRun) {
            //日志线程结束前将所有缓存日志输出到日志文件
            if (currentLog->length() > 0 && currentWrite->length() == 0) {
                lockWrite.lock();
                if (currentLog->length() > 0 && currentWrite->isEmpty()) {
                    QStringList *temp = currentWrite;

                    currentWrite = currentLog;
                    currentLog = temp;
                }
                lockWrite.unlock();
            } else if (currentLog->isEmpty() && currentWrite->isEmpty()) {
                break;
            }
        } else {
            this->sleep(1);
        }
    }

    isRun = false;
    outFile.close();
    printf("#Logger# logger thread over!\n");
}

void Logger::log(QtMsgType type, const char *msg)
{
    //日志线程已经标记为退出，不记录新的日志
    if (!isRun)
        return;

    QMutexLocker locker(&lockLog);
    QString time = QTime::currentTime().toString("hh:mm:ss");
    QString date = QDate::currentDate().toString("yyyy-MM-dd");
    QString txt;

    switch (type) {
        //调试信息
        case QtDebugMsg:
        {
            txt = QString("   #DEBUG# [%1 %2] %3\n").arg(date, time, msg);
            break;
        }
        //提示信息
        case QtInfoMsg:
        {
            txt = QString("    #INFO# [%1 %2] %3\n").arg(date, time, msg);
            break;
        }
        //警告信息
        case QtWarningMsg:
        {
            txt = QString(" #WARNING# [%1 %2] %3\n").arg(date, time, msg);
            break;
        }
        //严重错误
        case QtCriticalMsg:
        {
            txt = QString("#CRITICAL# [%1 %2] %3\n").arg(date, time, msg);
            break;
        }
        //致命错误
        case QtFatalMsg:
        {
            txt = QString("   #FATAL# [%1 %2] %3\n").arg(date, time, msg);
            break;
        }
        default:
            break;
    }

    if (type != QtDebugMsg || LOG_DEBUG)
        printf("%s", txt.toStdString().data());

    currentLog->append(txt);

    //判断更换日志缓冲区
    if (currentLog->length() > 10 && currentWrite->isEmpty())
    {
        lockWrite.lock();
        if (currentLog->length() > 10 && currentWrite->isEmpty())
        {
            QStringList *temp = currentWrite;
            currentWrite = currentLog;
            currentLog = temp;
        }
        lockWrite.unlock();
    }
}

void Logger::log(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(context);
    log(type, msg.toStdString().data());
}

int Logger::getLogIndex()
{
    QString name = "";
    QStringList strList;
    bool isOk = false;
    //获取日志目录的日志文件
    QDir dir(LOG_DIR_NAME);
    QStringList filters;
    filters << QString("%1_*.log").arg(LOG_FILE_NAME);
    QFileInfoList list = dir.entryInfoList(filters, QDir::Files);

    //日志文件与索引的一个映射表
    QMap<int, QFileInfo> infoMap;

    for (int i = 0; i < list.size(); i++) {
        if (list.at(i).fileName() == "." ||
                list.at(i).fileName() == "..")
            continue;
        //判断后缀是否为log
        if (list.at(i).completeSuffix() != "log") {
            QFile::remove(list.at(i).absoluteFilePath());
            continue;
        }
        //获取前缀
        name = list.at(i).baseName();
        strList = name.split("_");
        if (strList.size() != 2) {
            QFile::remove(list.at(i).absoluteFilePath());
            continue;
        }
        //获取日志文件的索引号
        int temp = strList.at(1).toInt(&isOk);
        if (isOk) {
            infoMap.insert(temp, list.at(i));
        } else {
            QFile::remove(list.at(i).absoluteFilePath());
            continue;
        }
    }

    //对日志文件列表的索引值按升序排列
    QList<int> logIndexList;
    foreach (int index, infoMap.keys()) {
        int i;
        for (i=0; i<logIndexList.size(); i++) {
            if (index < logIndexList.at(i)) {
                logIndexList.insert(i, index);
                break;
            }
        }
        if (i == logIndexList.size()) {
            logIndexList.append(index);
        }
    }
    //确定日志文件的有效个数
    int count = 0;
    if (infoMap.size() >= LOG_FILE_MAX_COUNT) {
        //删除最老的日志文件相关信息
        QFile::remove(infoMap.value(logIndexList.at(0)).absoluteFilePath());
        infoMap.remove(logIndexList.at(0));
        logIndexList.removeFirst();

        count = LOG_FILE_MAX_COUNT - 1;
    } else {
        count = infoMap.size();
    }
    //对这些日志文件按升序重命名
    for (int i=0; i<count; i++) {
        QString oldLogName = infoMap.value(logIndexList.at(i)).absoluteFilePath();
        QString newLogName = QString("%1/%2/%3_%4.log")
                .arg(QCoreApplication::applicationDirPath())
                .arg(LOG_DIR_NAME)
                .arg(LOG_FILE_NAME)
                .arg(i);
        QFile::rename(oldLogName, newLogName);
    }

    int index = count-1;
    index = (index >= 0) ? index : 0;
    QString logName = QString("%1/%2/%3_%4.log")
            .arg(QCoreApplication::applicationDirPath())
            .arg(LOG_DIR_NAME)
            .arg(LOG_FILE_NAME)
            .arg(index);

    if (QFileInfo(logName).exists() && QFileInfo(logName).size() > LOG_MAX_SIZE)
        index++;

    return index;
}

bool Logger::checkLogDir()
{
    QDir dir(QCoreApplication::applicationDirPath() + "/" + LOG_DIR_NAME);
    bool isOK = false;
    if (dir.exists()) {
        isOK = true;
    } else {
        dir.cdUp();
        isOK = dir.mkdir(LOG_DIR_NAME);
        if (!isOK) {
            isOK = dir.mkdir(LOG_DIR_NAME);
            if (!isOK) {
                printf("#Logger# checkLogDir(), make dir error!!\n");
                isRun = false;
            }
        }
    }

    return isOK;
}
