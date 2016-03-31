#include "HttpService.h"
#include "config.h"
#include "xlogger.h"
#include "configer.h"

HttpService::HttpService(int argc, char **argv)
: QtService<QCoreApplication>(argc, argv, g_serviceName)
{
    setServiceDescription("WaterMate, a local search engine");
    setServiceFlags(QtServiceBase::CanBeSuspended);

    Configer::LoadConfiger();
    Configer::getConfigerPort(m_listenPort);
}

void HttpService::start()
{
    QCoreApplication *app = application();

    quint16 port = (app->argc() > 1) ?
        QString::fromLocal8Bit(app->argv()[1]).toUShort() : m_listenPort;
    daemon = new HttpBridger(port, app);

    if (!daemon->isListening()) {
        XERROR("Failed to bind to port %d", daemon->serverPort());
        app->quit();
    }

    if (Configer::getFirsttime())
    {
        //默认设置为自动开启
        setStartupType(QtServiceController::AutoStartup);
        Configer::setFirstTime(false);
    }
}

void HttpService::pause()
{
    daemon->pause();
}

void HttpService::resume()
{
    daemon->resume();
}

void HttpService::stop()
{
    daemon->close();
    delete daemon;
}