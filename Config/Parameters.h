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

    const static QSqlDatabase * localDb;

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

const QString Parameters::appVersion = "1.0";
const QString Parameters::webSite = "Groovieandroid.com";
const QString Parameters::url = "http://192.168.56.1/Symfony/web/desktop";
const int Parameters::networkTimerFrequency = 10000;
const QString Parameters::storageDirectory = QDir().homePath()+"/Zemini";
const QString Parameters::setupDirectory = QDir().homePath()+"/ZeminiSetup";
const QString Parameters::iconLocation = Parameters::setupDirectory+"/zemini.ico";
const QString Parameters::devAppFolder = "C:\\Users\\Tafsir\\Documents\\QtProjects\\Zemini_desktop";
const QString Parameters::userIcon = Parameters::devAppFolder + "\\Resources\\Images\\user_icon.png";
const QString Parameters::timeFormat = "yyyy-MM-dd hh:mm:ss";
const int Parameters::passwordMinLength = 6;
const QRegExp * Parameters::emailRegExpr = new QRegExp("^[a-z0-9._-]+@[a-z0-9._-]{2,}\\.[a-z]{2,4}$");
DatabaseManager dbManager;
const QSqlDatabase * Parameters::localDb = dbManager.connectToDatabase("ZeminiDB", "root","localhost", 3306, "","QSQLITE", "db");


#endif // ZEMINIPARAMETERS_H
