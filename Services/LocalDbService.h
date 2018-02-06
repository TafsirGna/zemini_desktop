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

    // Some getters
    UserManager * getUserManager();
    CategoryManager * getCategoryManager();
    FileManager * getFileManager();
    DirectoryManager * getDirectoryManager();
    TypeManager * getTypeManager();

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
    bool save(User *);
    bool save(QFileInfo);

};
#endif // LOCALDBSERVICE_H
