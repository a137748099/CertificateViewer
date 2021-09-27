#include "logger.h"
#include <QDateTime>
#include <QDir>
//#include <QApplication>
#include <QTextStream>

QMutex Logger::loggerMutex;
QString Logger::logFilePath;
QStringList Logger::m_ignore;//需要过滤的字符串
QtMessageHandler Logger::m_preHandler;
Logger::Logger()
= default;

void Logger::installLog(QString logPath)
{
    QDateTime datetime(QDateTime::currentDateTime());
    logPath += datetime.toString("yyyy-MM-dd");
    if (!QDir(logPath).exists()) {
        QDir().mkpath(logPath);
    }
    if(!logPath.endsWith(QDir::separator())){
        logPath += QDir::separator();
    }
    logPath += QString("%1.log")
            .arg (datetime.toString ("hh-mm-ss"));
    Logger::logFilePath = logPath;
    m_preHandler = qInstallMessageHandler(Logger::messageOutput);//设置回调
}

void Logger::messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if(ignore(msg)){
        return;
    }

    QMutexLocker locker(&Logger::loggerMutex);//加锁

    QString levelValue;
    switch (type) {
    case QtDebugMsg:
        levelValue = "Debug";
        break;
    case QtInfoMsg:
        levelValue = "Info";
        break;
    case QtWarningMsg:
        levelValue = "Warning";
        break;
    case QtCriticalMsg:
        levelValue = "Critical";
        break;
    case QtFatalMsg:
        levelValue = "Fatal";
        break;
    }

#if 0 //重新格式调试信息，丢到stderr。可以显示行号等
    fprintf(stderr, "Debug: %s (%s:%u, %s)\n", msg.toLocal8Bit().constData(), context.file, context.line, context.function);
#endif

    QDateTime dateTime(QDateTime::currentDateTime());
    QString timeValue(dateTime.toString("dd-MM-yyyy HH:mm:ss:zzz"));

    QByteArray localMsg = msg.toUtf8();
    QString contextString;
    if(type>QtDebugMsg){
        contextString = QString("%1 %2: %3---file:%4,line:%5,function:%6\n").arg(timeValue).arg(levelValue)
                .arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function);
    }else{
        contextString = QString("%1 %2: %3\n").arg(timeValue).arg(levelValue)
                .arg(localMsg.constData())/*.arg(context.file).arg(context.line).arg(context.function)*/;
    }

    QFile outFile(Logger::logFilePath);
    if(outFile.open(QIODevice::WriteOnly | QIODevice::Append)){
        QTextStream stream(&outFile);
        stream << contextString << endl;

        if (QtFatalMsg == type) {
            abort();
        }
    }else{
        printf("open failed");
    }
//    if(m_preHandler){
//        m_preHandler(type,context,msg);
//    }
}

bool Logger::ignore(const QString &msg)
{
    foreach (QString s, m_ignore) {
        if (-1 != msg.indexOf(s)) {
            return true;
        }
    }
    return false;
}
