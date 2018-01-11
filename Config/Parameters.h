#ifndef ZEMINIPARAMETERS_H
#define ZEMINIPARAMETERS_H

#include <QString>
#include <QDir>
#include <QRegExpValidator>

class Parameters
{
public:

    static QString appVersion()
    {
        return "1.0";
    }

    static QString webSite()
    {
        return "Groovieandroid.com";
    }

    static QString url ()
    {
        return "http://192.168.56.1";
    }

    static int networkTimerFrequency()
    {
        return 10000;
    }

    static QString storageDirectory()
    {
        return QDir().homePath()+"/Zemini";
    }

    static QString moviesDirectory()
    {
        return storageDirectory()+"/Movies";
    }

    static QString seriesDirectory()
    {
        return storageDirectory()+"/Series";
    }

    static QString softwaresDirectory()
    {
        return storageDirectory()+"/Softwares";
    }

    static QString setupDirectory()
    {
        return QDir().homePath()+"/ZeminiSetup";
    }

    static QString iconLocation()
    {
        return Parameters::setupDirectory()+"/zemini_logo.png";
    }

    static QString booksDirectory()
    {
        return storageDirectory()+"/Books";
    }

    static int getDirServiceSleepTime()
    {
        return 60;
    }

    static QString timeFormat()
    {
        QString format = "yyyy-MM-dd hh:mm:ss";
        return format;
    }

    static int passwordMinLength()
    {
        return 6;
    }

    static bool isEmailValid(QString email)
    {
        // here , i check if the email entered is valid or not after a certain regular expression

        QRegExp rx("^[a-z0-9._-]+@[a-z0-9._-]{2,}\\.[a-z]{2,4}$");
        //email = "gnatafsir@gmail.com";
        return rx.exactMatch(email);

    }

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

    /***    this creates the zemini directory if it doesn't exist yet    ***/
    static void make_directories(QString rootDir)
    {
        rootDir.replace("\\","/");
        //QMessageBox::information(0,"path",directoryPath);
        QDir zeminiDirectory(rootDir);
        if (!zeminiDirectory.exists())
        {
            if (!zeminiDirectory.mkdir("."))
                qDebug()<<"Error creating the main folder"<<endl;
        }

        QString programsDirPath = rootDir+"/Softwares";
        programsDirPath.replace("\\","/");
        QDir programsDir(programsDirPath);
        if (!programsDir.exists())
        {
            if (!programsDir.mkdir("."))
                qDebug()<<"Error creating the programs folder"<<endl;
        }

        QString gameDirPath = rootDir+"/Games";
        gameDirPath.replace("\\","/");
        QDir gameDir(gameDirPath);
        if (!gameDir.exists())
        {
            if (!gameDir.mkdir("."))
                qDebug()<<"Error creating the game folder"<<endl;
        }

        QString documentsDirPath = rootDir+"/Books";
        documentsDirPath.replace("\\","/");
        QDir documentsDir(documentsDirPath);
        if (!documentsDir.exists())
        {
            if (!documentsDir.mkdir("."))
                qDebug()<<"Error creating the books folder"<<endl;
        }

        QString moviesDirPath = rootDir+"/Movies";
        moviesDirPath.replace("\\","/");
        QDir moviesDir(moviesDirPath);
        if (!moviesDir.exists())
        {
            if (!moviesDir.mkdir("."))
                qDebug()<<"Error creating the movies folder"<<endl;
        }

        QString seriesDirPath = rootDir+"/Series";
        seriesDirPath.replace("\\","/");
        QDir seriesDir(seriesDirPath);
        if (!seriesDir.exists())
        {
            if (!seriesDir.mkdir("."))
                qDebug()<<"Error creating the series folder"<<endl;
        }
    }

    static QString devAppFolder()
    {
        return "C:\\Users\\Tafsir\\Documents\\QtProjects\\Zemini_desktop";
    }
};

#endif // ZEMINIPARAMETERS_H
