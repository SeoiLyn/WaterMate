#include "configer.h"
#include <stdlib.h>
#include <shlobj.h>
#include <QSettings>
#include <QDir>
#include <QFile>

QList<QString> Configer::m_rootPath;
int Configer::m_port = 60602;
bool Configer::m_inited = false;
bool Configer::m_firstTime = true;

Configer::Configer()
{
}

Configer::~Configer()
{
}

bool Configer::LoadConfiger()
{
    if (m_inited)
    {
        return true;
    }

    m_inited = true;

    QString appdata = _configerFile();

    QFile appfile(appdata);
    if (!appfile.exists())
    {
        return false;
    }

    QSettings settings(appdata, QSettings::IniFormat);
    m_port = settings.value("basic/port").toInt();
    m_firstTime = settings.value("basic/FirstTime").toBool();
    int total = settings.value("path/total").toInt();
    for (int i = 0; i < total; ++i) {
        QString path = settings.value(QString("path/idx%1").arg(i)).toString();
        m_rootPath.push_back(path);
    }

    return true;
}

bool Configer::SaveConfiger()
{
    QString appdata = _configerFile();
    QSettings settings(appdata, QSettings::IniFormat);


    settings.setValue("basic/port", m_port);
    settings.setValue("basic/FirstTime", m_firstTime);
    settings.setValue("path/total", m_rootPath.size());
    for (int i = 0; i < m_rootPath.size(); ++i) {
        settings.setValue(QString("path/idx%1").arg(i), m_rootPath.at(i));
    }

    return true;
}

void Configer::getConfigerPath(QList<QString>& path)
{
    path = m_rootPath;
}

void Configer::getConfigerPort(int& port)
{
    port = m_port;
}

void Configer::addConfigerPath(QString path)
{
    m_rootPath.push_back(path);
    SaveConfiger();
}

void Configer::delConfigerPath(QString path)
{
    for (int i = 0; i < m_rootPath.size(); ++i) {
        if ( m_rootPath.at(i) == path )
        {
            m_rootPath.removeAt(i);
        }
    }

    SaveConfiger();
}

QString Configer::_configerFile()
{
	char szAppData[MAX_PATH] = {0};
	SHGetSpecialFolderPathA(NULL, szAppData, CSIDL_APPDATA, TRUE);
    QString appfolder = szAppData;

    appfolder += "\\WaterMate";

    if (!QDir(appfolder).exists())
    {
        QDir().mkdir(appfolder);
    }

    QString appdata = appfolder + "\\config.ini";
    return appdata;
}

bool Configer::getFirsttime()
{
    return m_firstTime;
}

void Configer::setFirstTime(bool status)
{
    m_firstTime = status;
    SaveConfiger();
}