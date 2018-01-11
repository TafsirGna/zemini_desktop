#ifndef CLASSDIRECTORYMANAGER_H
#define CLASSDIRECTORYMANAGER_H

#include "AbstractManager.h"
#include <QSqlError>
#include "Database\Entities\Directory.h"
#include "Database\EntityManagers\TypeManager.h"

class DirectoryManager : public AbstractManager
{
private:
    TypeManager * typeManager;
    QList<Directory> * dirstosave;
    Directory * currentdir;

protected:
    void saveUpdateToRemoteDB();

public:
    //builders
    DirectoryManager();
    DirectoryManager(QSqlDatabase &db);
    DirectoryManager(QSqlDatabase &db,TypeManager * typeManager);

    //functions to save directories into databases
    bool saveDirectoryLocally(Directory * directory);
    bool saveDirectoryLocally(QFileInfo * fileInfo);
    void saveDirectory(Directory * directory);

    //functions to update directories in databases
    bool updateDirectoryLocally(Directory * directory);
    bool updateDirectoryLocally(QFileInfo * fileInfo);
    void updateDirectory(Directory * directory);

    //functions to delete directories from databases
    bool deleteDirectoryLocally(Directory * directory);
    bool deleteDirectoryLocally(QFileInfo fileInfo);
    void deleteDirectory(Directory * directory);
    //void deleteDBIrrelevantDirs();

    //this function converts a QFileInfo object to a Directory object
    Directory * convertToDirectory(QFileInfo * fileInfo);

    // functions used to select by a given critera
    Directory * findDirectoryByPath(QString path);
    Directory * findDirectoryById(int id);
    QList<Directory> findDirectoryByIdDirectory(int iddirectory);
    QList<Directory> findDirectoryByStatus(int status);
    Directory * findOtherDirectoryOnDisk();
    void saveUpdatesToRemoteDB();

    void showDatabaseDirectories();
    void checkDirsIntegrity();
    void countDatabaseDirectories();
    void resetDatabaseDirectories();
    QList<Directory> getAllDBDirs();

public slots:
    void handleEndofRequest(QNetworkReply*);
};

#endif // CLASSDIRECTORYMANAGER_H
