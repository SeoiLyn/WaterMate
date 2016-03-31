/**
 * ��һЩ������Ϣ���л�Ӳ��
 */

#ifndef CONFIGER_FILE_H_
#define CONFIGER_FILE_H_

#include <QString>
#include <QList>

class Configer
{
public:
    Configer();
    ~Configer();

public:
    static bool LoadConfiger();
    static bool SaveConfiger();
    static void getConfigerPath(QList<QString>& path);
    static void getConfigerPort(int& port);
    static void addConfigerPath(QString path);
    static void delConfigerPath(QString path);

    static bool getFirsttime();
    static void setFirstTime(bool status);

private:
    static QString _configerFile();

private:
    static QList<QString> m_rootPath;
    static int            m_port;
    static bool           m_inited;
    static bool           m_firstTime;//�Ƿ�װ���һ������
};
#endif /* CONFIGER_FILE_H_ */