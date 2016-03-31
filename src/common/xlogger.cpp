#include "XLogger.h"
#include <QDir>
#include <stdlib.h>
#include <shlobj.h>
#include <QMutex>

XLogger* XLogger::m_xLogger = NULL;

XLogger::XLogger(QString logfile)
{
    m_logFile = logfile;
    m_logHandle = new QFile(m_logFile);
    m_logHandle->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
    
    m_logStream = new QTextStream(m_logHandle);
}

XLogger::~XLogger(void)
{
    m_logHandle->close();
    delete m_logHandle;
    delete m_logStream;
}

void XLogger::Info(const char *fmt, ...)
{
    XLogger::initLogger();
    if (m_xLogger)
    {
        char msg[2048];

        va_list vl;
        va_start(vl, fmt);
        vsprintf(msg,fmt,vl);
        va_end(vl);

        m_xLogger->info(QString(msg));
    }
}

void XLogger::Warning(const char *fmt, ...)
{
    XLogger::initLogger();
    if (m_xLogger)
    {
        char info[2048];

        va_list vl;
        va_start(vl, fmt);
        vsprintf(info,fmt,vl);
        va_end(vl);

        m_xLogger->warn(QString(info));
    }
}

void XLogger::Debug(const char *fmt, ...)
{
    XLogger::initLogger();
    if (m_xLogger)
    {
        char info[2048];

        va_list vl;
        va_start(vl, fmt);
        vsprintf(info,fmt,vl);
        va_end(vl);

        m_xLogger->debug(QString(info));
    }
}

void XLogger::Error(const char *fmt, ...)
{
    XLogger::initLogger();
    if (m_xLogger)
    {
        char info[2048];

        va_list vl;
        va_start(vl, fmt);
        vsprintf(info,fmt,vl);
        va_end(vl);

        m_xLogger->error(QString(info));
    }
}

void XLogger::info(QString log)
{
    *m_logStream << generateLog(QtDebugMsg, log);
    m_logStream->flush();
    qDebug() << log;
}

void XLogger::warn(QString log)
{
    *m_logStream << generateLog(QtWarningMsg, log);
    m_logStream->flush();
    qDebug() << log;
}

void XLogger::debug(QString log)
{
    *m_logStream << generateLog(QtDebugMsg, log);
    m_logStream->flush();
    qDebug() << log;
}

void XLogger::error(QString log)
{
    *m_logStream << generateLog(QtCriticalMsg, log);
    m_logStream->flush();
    qDebug() << log;
}

void XLogger::initLogger()
{
    if (m_xLogger)
    {
        return;
    }

#if 0
    QString appfolder = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
#else
	char szAppData[MAX_PATH] = {0};
	SHGetSpecialFolderPathA(NULL, szAppData, CSIDL_APPDATA, TRUE);
    QString appfolder = szAppData;
#endif
    appfolder += "\\WaterMate";

    if (!QDir(appfolder).exists())
    {
        QDir().mkdir(appfolder);
    }

    QString appdata = appfolder + "\\bridger.log";

    m_xLogger = new XLogger(appdata);
}

XLogger* XLogger::instance()
{
    return m_xLogger;
}

void XLogger::freeLogger()
{
    if (m_xLogger)
    {
        delete m_xLogger;
        m_xLogger = NULL;
    }
}

QString XLogger::generateLog(QtMsgType type, QString msg)
{
	QDateTime datatime = QDateTime::currentDateTime();
    QString log = datatime.toString("yyyy-MM-dd hh:mm:ss-zzz") + ": ";
	switch (type) {
	case QtDebugMsg:
		log += QString("DEBUG  : ");
        log += msg;
		break;
	case QtWarningMsg:
		log += QString("WARNING: %1");
        log += msg;
		break;
	case QtCriticalMsg:
		log += QString("ERROR  : %1");
        log += msg;
		break;
	default:
		log += QString("");
		break;
	}

    log += "\r\n";

    return log;
}