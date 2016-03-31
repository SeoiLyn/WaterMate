#ifndef __HTTPBRIDGER_H__
#define __HTTPBRIDGER_H__
#include <QTcpSocket>
#include <QTcpServer>
#include <QCoreApplication>
#include <QDateTime>
#include <QHostAddress>
#include <QStringList>
#include <QFile>
#include "QtService.h"

class HttpBridger : public QTcpServer
{
    Q_OBJECT
public:
    HttpBridger(quint16 port, QObject* parent = 0);

    void incomingConnection(int socket);
    void pause(){ disabled = true; }
    void resume(){ disabled = false; }

private slots:
    void readClient();
    void discardClient();

private:
    void    startBridger(quint16 port);
    QString processHtmlRootRequest();
    QString processHtmlNodeRequest(QString htmlpath);
    QString processHtmlFileRequest(QString htmlpath);
    QString processConfigRequest(QString parameters);
    QString processSearchRequest(QString keyword);
    bool    searchEngine(QString rootPath, QString keyword, QString& result);

private:
    bool checkSubFolder(QString identation, QString path, QString& result);

private:
    bool disabled;

private:
    static QList<QString> m_rootPath;
    static bool           m_bRootPathInited;
    static int            m_listenPort;
};
#endif /* __HTTPBRIDGER_H__ */