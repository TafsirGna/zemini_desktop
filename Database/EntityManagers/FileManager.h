#ifndef CLASSFILEMANAGER_H
#define CLASSFILEMANAGER_H

#include "Database\Entities\File.h"
#include "Database\EntityManagers\TypeManager.h"

class FileManager : public AbstractManager
{
    Q_OBJECT
private :
    TypeManager * typeManager;

protected:
    File * convertToFile(QFileInfo);

public :
    FileManager();
    FileManager(TypeManager * typeManager);

    bool saveFile(File * file);
    void updateFile(File * file);
    void deleteFile(File * file);
    bool cleanDirFile(QDir);
    QList<File> getAllFiles();
    QList<File> getNotSavedFiles();

    /*
    File * findFileByPath(QString path);    //As the path and the filename are unique, finding a file by its filepath return only one file object
    void showDatabaseFiles();
    void checkFilesIntegrity();
    QList<File> findFileByIdDirectory(int iddirectory);
    QList<File> findFileByStatus(int status);
    void saveUpdatesToRemoteDB();
    void resetDatabaseFiles();
    */

public slots:
    //void handleEndofRequest(QNetworkReply*);
};

#endif // CLASSFILEMANAGER_H
