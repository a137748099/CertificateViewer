#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QMutex>

class Logger
{
public:
    Logger();

    static void installLog(QString logPath);

private:

    static void messageOutput(QtMsgType type,
                              const QMessageLogContext &context,
                              const QString &msg);
    static bool ignore(const QString& msg);

private:
    static QStringList m_ignore;
    static QMutex loggerMutex;
    static QString logFilePath;
    static QtMessageHandler m_preHandler;
};

#endif // LOGGER_H
