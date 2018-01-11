#ifndef LOCALDBSERVICE_H
#define LOCALDBSERVICE_H

#include <QThread>
#include <QDebug>
#include <QFileInfoList>
#include "Database\Entities\Directory.h"
#include "Database\EntityManagers\DirectoryManager.h"
#include "Database\Entities\File.h"
#include "Database\EntityManagers\UserManager.h"
#include "Database\EntityManagers\FileManager.h"

class LocalDBService : public QThread
{
    Q_OBJECT
private :
    QFileInfoList * fileInfoList;

    //All entity managers
    DirectoryManager * dirManager;
    FileManager * fileManager;
    CategoryManager * categoryManager;
    UserManager * userManager;
    TypeManager * typeManager;

    // the local database
    QSqlDatabase db;
protected:
    void run();
    void terminateLocalDatabaseUpdate();

public:
    LocalDBService(QSqlDatabase db,QObject * parent = 0);
    void setArguments(QFileInfoList * fileInfoList);
    void setEntitiesManagers(DirectoryManager * dirManager, FileManager * fileManager);

    // Getters
    bool isDbEmpty();
    UserManager * getUserManager();

signals:
    void dataSavedLocally();

};

#endif // LOCALDBSERVICE_H
