#include <QApplication>
#include "HttpService.h"
#include <QTextCodec>
#include "xlogger.h"

int main(int argc, char *argv[])
{
    XLogger::initLogger();

#if 0
    QTextCodec *codec = QTextCodec::codecForLocale();
    QTextCodec::setCodecForTr(codec);
#else
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);  
    QTextCodec::setCodecForCStrings(codec);   
    QTextCodec::setCodecForTr(codec);
#endif

#if !defined(Q_WS_WIN)
    // QtService stores service settings in SystemScope, which normally require root privileges.
    // To allow testing this example as non-root, we change the directory of the SystemScope settings file.
    QSettings::setPath(QSettings::NativeFormat, QSettings::SystemScope, QDir::tempPath());
    qWarning("(Example uses dummy settings file: %s/QtSoftware.conf)", QDir::tempPath().toLatin1().constData());
#endif

    HttpService service(argc, argv);
    service.exec();
    XLogger::freeLogger();
    return 0;
}