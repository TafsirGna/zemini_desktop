#ifndef LOCALDBSERVICE_H
#define LOCALDBSERVICE_H

#include <QDebug>
#include <QFileInfoList>
#include "Database\EntityManagers\DirectoryManager.h"
#include "Database\EntityManagers\UserManager.h"
#include "Database\EntityManagers\FileManager.h"
#include <Services/ZeminiService.h>

class LocalDBService: public ZeminiService
{
    Q_OBJECT
private :
    //All entity managers
    DirectoryManager * directoryManager;
    FileManager * fileManager;
    CategoryManager * categoryManager;
    UserManager * userManager;
    TypeManager * typeManager;

protected:

public:

    //Constants
    const static QString USER;
    const static QString CATEGORY;
    const static QString TYPE;
    const static QString FILE;
    const static QString DIR;

    //Builder
    LocalDBService();

    // Getters
    bool isDbEmpty();
    AbstractManager * getManager(QString);

public slots:
    bool save(QFileInfo);

};

const QString LocalDBService::USER = "user";
const QString LocalDBService::CATEGORY = "category";
const QString LocalDBService::DIR = "directory";
const QString LocalDBService::FILE = "file";
const QString LocalDBService::TYPE = "type";

#endif // LOCALDBSERVICE_H
