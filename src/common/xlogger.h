#ifndef __X_LOGGER_H__
#define __X_LOGGER_H__

/**
 * 重定向日志到日志文件 %AppData%\WaterMate 以及系统服务日志里
 * waterlin.org@gmail.com
 */

#include <QDesktopServices>
#include <qdatetime.h>
#include <QFile>
#include <QTextStream>
#include <QDebug>

#define XINFOR(fmt, ...) \
    do { \
        XLogger::Info(fmt, __VA_ARGS__);\
    } while (0)

#define XALARM(fmt, ...) \
    do { \
        XLogger::Warning(fmt, __VA_ARGS__);\
    } while (0)

#define XDEBUG(fmt, ...) \
    do { \
        XLogger::Debug(fmt, __VA_ARGS__);\
    } while (0)

#define XERROR(fmt, ...) \
    do { \
        XLogger::Error(fmt, __VA_ARGS__);\
    } while (0)

#if 0
//暂时弃用
#define XINFOR(message) XLogger::instance()->info(message)
#define XALARM(message) XLogger::instance()->warn(message)
#define XDEBUG(message) XLogger::instance()->debug(message)
#define XERROR(message) XLogger::instance()->error(message)
#endif

class XLogger
{
public:
    XLogger(QString logfile);
    ~XLogger(void);

public:
    static void Info(const char *fmt, ...);
    static void Warning(const char *fmt, ...);
    static void Debug(const char *fmt, ...);
    static void Error(const char *fmt, ...);

private:
    void info(QString log);
    void warn(QString log);
    void debug(QString log);
    void error(QString log);

public:
    static void initLogger();
    static XLogger* instance();
    static void freeLogger();

private:
    QString generateLog(QtMsgType type, QString msg);

private:
    static XLogger* m_xLogger;

private:
    QString m_logFile;
    QFile*  m_logHandle;
    QTextStream* m_logStream;
};

#endif /* __X_LOGGER_H__ */
