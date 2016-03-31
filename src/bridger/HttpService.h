#include "HttpBridger.h"

class HttpService : public QtService<QCoreApplication>
{
public:
    HttpService(int argc, char **argv);

protected:
    virtual void start();
    virtual void pause();
    virtual void resume();
    virtual void stop();

private:
    HttpBridger *daemon;

private:
    int   m_listenPort;
};