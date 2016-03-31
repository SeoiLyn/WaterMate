#include "HttpBridger.h"
#include <QDirIterator>
#include <QTextCodec>
#include <QByteArray>
#include <QUrl>

#include "config.h"
#include "xlogger.h"
#include "template.h"
#include "configer.h"

QList<QString> HttpBridger::m_rootPath;
bool           HttpBridger::m_bRootPathInited = false;
int            HttpBridger::m_listenPort = 60602;

HttpBridger::HttpBridger(quint16 port, QObject* parent)
 : QTcpServer(parent), disabled(false)
{
    startBridger(port);
}

void HttpBridger::startBridger(quint16 port)
{
    if ( listen(QHostAddress::LocalHost, port) )
    {
        XINFOR("listen on port 60602 successfully!");
    } else {
        XERROR("something wrong on listening on 60602");
    }
}

void HttpBridger::incomingConnection(int socket)
{
    //保证只初始化一次
    if (!m_bRootPathInited)
    {
        Configer::LoadConfiger();
        Configer::getConfigerPath(m_rootPath);
        Configer::getConfigerPort(m_listenPort);
        m_bRootPathInited = true;
    }

    if (disabled)
        return;

    // When a new client connects, the server constructs a QTcpSocket and all
    // communication with the client is done over this QTcpSocket. QTcpSocket
    // works asynchronously, this means that all the communication is done
    // in the two slots readClient() and discardClient().
    QTcpSocket* s = new QTcpSocket(this);
    connect(s, SIGNAL(readyRead()), this, SLOT(readClient()));
    connect(s, SIGNAL(disconnected()), this, SLOT(discardClient()));
    s->setSocketDescriptor(socket);

    XINFOR("New Connection");
}

void HttpBridger::readClient()
{
    if (disabled)
        return;

    // This slot is called when the client sent data to the server. The
    // server looks if it was a get request and sends a very simple HTML
    // document back.
    QTcpSocket* socket = (QTcpSocket*)sender();
    if (socket->canReadLine()) {
        QString params = socket->readLine();
        QStringList tokens = params.split(QRegExp("[ \r\n][ \r\n]*"));

        if (tokens[0] == "GET") {
            QString result = "";
            switch (tokens.size())
            {
            case 4:
                {
                    result = processHtmlNodeRequest(tokens[1]);
                    break;
                }
            case 3:
                {
                    //root path
                    result = processHtmlRootRequest();
                    break;
                }
            default:
                break;
            }


            if (!result.isEmpty())
            {
#if 0
                QTextStream os(socket);
                QTextCodec *codec = QTextCodec::codecForLocale();
                os.setCodec(codec);
                //os.setCodec("UTF-8");
                os.setAutoDetectUnicode(true);
                os << result;
#else
                QTextStream os(socket);
                QTextCodec *codec = QTextCodec::codecForName("UTF-8");
                os.setAutoDetectUnicode(true);

                QByteArray another = result.toUtf8().data();
                socket->write(another.data());
#endif
            }

            socket->close();

            XINFOR("Wrote to client");

            if (socket->state() == QTcpSocket::UnconnectedState) {
                delete socket;
                XERROR("Connection closed");
            }
        }
    }
}

void HttpBridger::discardClient()
{
    QTcpSocket* socket = (QTcpSocket*)sender();
    socket->deleteLater();

    XINFOR("Connection closed");
}

//访问 html 根目录
QString HttpBridger::processHtmlRootRequest()
{
    QString htmlcontent = TEMPLATE_INDEX;

    for (int i = 0; i < m_rootPath.size(); ++i) {
        QString result;
        if ( checkSubFolder("", m_rootPath.at(i), result) )
        {
            htmlcontent += result;
        }
    }
    return htmlcontent;
}

/**
 * @param identation, 目录树的缩进、层次参数
 * @param path,需要进行目录树生成的目录
 * @param result, 生成的目录树结果
 * @return if has valid file under the folder, return true; else return false
 * @notice 目录，只认为包含 html 的目录，认为是有效目录；其它目录一概不认为是有效目录，从而不进行显示
 */
bool HttpBridger::checkSubFolder(QString identation, QString path, QString& result)
{
    QString newIdent = identation + "&nbsp;&nbsp;";
    QString subFolderInfo = "";
    bool isValidFolder = false;

    QDirIterator it(path);
    while (it.hasNext()) {
        //htmlcontent += it.fileName();
        QString filepath = it.next();
        QString filename = it.fileName();
        if (filename=="." || filename=="..")
        {
            continue;
        }

        if (QDir(filepath).exists())
        {
            //is a folder
            QString result;
            if ( checkSubFolder(newIdent, filepath, result) )
            {
                subFolderInfo = subFolderInfo + identation + "<B>" + filename + "</B><br>";
                subFolderInfo += result;
                isValidFolder = true;//has valid subfolder
            }
        } else if (filepath.contains(".html") ) {
            isValidFolder = true;//has valid html file
            subFolderInfo += identation + "<a href=\"http://127.0.0.1:" + QString("%1").arg(m_listenPort) + "/" + filepath + "\" target=blank>" + filename + "</a><br>";
        }
    }

    result = subFolderInfo;

    return isValidFolder;
}

//访问普通的 html 节点
QString HttpBridger::processHtmlNodeRequest(QString htmlpath)
{
    if (htmlpath == "/")
    {
        return processHtmlRootRequest();
    }

    if (htmlpath.contains("/search?"))
    {
        QString keyword = htmlpath.section("=",1,1);
        return processSearchRequest(keyword);
    }

    if (htmlpath.contains("/readme"))
    {
        return TEMPLATE_HEADER + TEMPLATE_README;
    }

    if (htmlpath.contains("/config"))
    {
        return processConfigRequest(htmlpath);
    }

    return processHtmlFileRequest(htmlpath);
}

QString HttpBridger::processHtmlFileRequest(QString htmlpath)
{
    //read feedback
    QString filename = htmlpath.right(htmlpath.size()-1);
    filename = QUrl::fromPercentEncoding(QByteArray(filename.toStdString().c_str()));
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return TEMPLATE_HEADER + 
        tr("File doesn't exits, 404 Error");
    }

    QTextStream stream(&file);
    QString htmlcontent;
    htmlcontent = stream.readAll();
    htmlcontent = TEMPLATE_HEADER + htmlcontent;

    file.close( );

    return htmlcontent;
}

QString HttpBridger::processSearchRequest(QString keyword)
{
    QString keydecoded = QUrl::fromPercentEncoding(QByteArray(keyword.toStdString().c_str()));
    
    QString htmlcontent = TEMPLATE_HEADER;

    for (int i = 0; i < m_rootPath.size(); ++i) {
        QString searchResult;
        if ( searchEngine(m_rootPath.at(i), keydecoded, searchResult) )
        {
            htmlcontent += searchResult;
        } else {
            htmlcontent += "No Search Result!";
        }
    }

    return htmlcontent;
}

QString HttpBridger::processConfigRequest(QString parameters)
{

    QString addon = "";
    if (parameters != "/config")
    {
        //提交或删除操作
        QString keyword = parameters.section("=",1,1);
        QString keydecoded = QUrl::fromPercentEncoding(QByteArray(keyword.toStdString().c_str()));

        QRegExp sep("[?=]");
        QString operate = parameters.section(sep,1,1);

        //@todo 检查该目录是否已经在列表里了
        if (operate == "addpath")
        {
            QDir addpath(keydecoded);
            if (addpath.exists())
            {
                Configer::addConfigerPath(keydecoded);
                Configer::getConfigerPath(m_rootPath);
                addon = "<B>Add the path successfully</B><br><hr>";
            } else {
                addon = "<B>Can't add the path, because the path is invalid!</B><br><hr>";
            }
        } else if (operate == "delpath") {
            Configer::delConfigerPath(keydecoded);
            Configer::getConfigerPath(m_rootPath);
        }
    }

    //redirect to /config and refresh the page
    QString htmlcontent = TEMPLATE_HEADER + addon;

    for (int i = 0; i < m_rootPath.size(); ++i) {
        htmlcontent += m_rootPath.at(i);
        htmlcontent += "<br>";
    }

    htmlcontent += TEMPLATE_ADDPATH;

    return htmlcontent;
}

/**
 * 在指定目录 rootPath 里进行关键词 keyword 搜索工作
 * 如找到匹配的结果，则返回 true，否则返回 false
 * 并把结果保存于参数 result 里
 */
bool HttpBridger::searchEngine(QString rootPath, QString keyword, QString& result)
{
    bool bSearchResult = false;
    QString resultstyle = "<font color=#ff0000>"+keyword+"</font>";

    QDirIterator it(rootPath, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString filepath = it.next();
        QString filename = it.fileName();
        if (filename=="." || filename=="..")
        {
            continue;
        }

        XINFOR(filepath.toStdString().c_str());

        if (!QDir(filepath).exists() && filepath.contains(".html"))
        {
            //do the search job

            QFile file(filepath);
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                //can't open file and search
                return false;
            }

            QString resultInfo;
            QTextStream stream(&file);

            while (!stream.atEnd())
            {
                QString linecontent;
                linecontent = stream.readLine();

                if (linecontent.contains(keyword))
                {
                    resultInfo += linecontent.replace(QRegExp(keyword), resultstyle);
                    resultInfo += "<br>";
                    bSearchResult = true;
                }
            }

            if (!resultInfo.isEmpty())
            {
                result += "<h2><a href=\"http://127.0.0.1:" + QString("%1").arg(m_listenPort) + "/" + filepath + "\" target=blank>" + filename + "</a></h2><br>";
                result += resultInfo;
                result += "<hr>";
            }
        }
    }

    return bSearchResult;
}