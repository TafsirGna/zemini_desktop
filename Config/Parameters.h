#ifndef ZEMINIPARAMETERS_H
#define ZEMINIPARAMETERS_H

#include <QString>
#include <QDir>
#include <QRegExpValidator>
#include <QRegExp>
#include <QSqlDatabase>
#include <Database/DatabaseManager.h>

class Parameters
{
public:

    const static QString appVersion;

    const static QString webSite;

    const static QString url;

    const static int networkTimerFrequency;

    const static QString storageDirectory;

    const static QString setupDirectory;

    const static QString iconLocation;

    const static QString devAppFolder;

    const static QString userIcon;

    const static QString timeFormat;

    const static int passwordMinLength;

    const static QRegExp * emailRegExpr;

    static QSqlDatabase localDb;

    /*
    static QString getTypeByPath(QString path)
    {
        QString result = "None";
        QStringList pathList = path.split('/');
        for (int i=0;i<pathList.size();i++)
        {
            if (pathList.at(i) == "Zemini")
            {
                if (i+1<pathList.size())
                    result = pathList.at(i+1);
                else
                    return result;
            }
        }
        //qDebug()<<result;
        return result;
    }

    static QString getParentDirPath(QString path)
    {
        QString result;
        QStringList pathList = path.split('/');
        if (pathList.size()>0) pathList.removeLast();
        result = pathList.join('/');
        qDebug()<<"--- "+result;
        return result;
    }

    // this creates the zemini directory if it doesn't exist yet
    static void make_directories(QString rootDir)
    {

    }
    */
};

#endif // ZEMINIPARAMETERS_H
