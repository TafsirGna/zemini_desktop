#ifndef CLASSFILEMANAGER_H
#define CLASSFILEMANAGER_H

#include "Database\Entities\File.h"
#include "AbstractManager.h"
#include <QSqlError>
#include "Database\EntityManagers\TypeManager.h"
#include "Database\EntityManagers\DirectoryManager.h"

class FileManager : public AbstractManager
{
    Q_OBJECT
private :
    TypeManager * typeManager;
    DirectoryManager * directoryManager;
    QList<File> *  filestosave;
    File * currentfile;

protected:
    void saveUpdateToRemoteDB();
    File * convertToFile(QFileInfo);

public :
    FileManager();
    FileManager(QSqlDatabase &db);
    FileManager(QSqlDatabase &db, TypeManager * typeManager, DirectoryManager * directoryManager);

    void saveFile(File * file);
    bool saveFileLocally(File * file);
    bool saveFileLocally(QFileInfo * fileInfo);
    void saveFile(QList<File> listFiles);
    void saveFile(QFileInfoList * filelist);

    void updateFile(File * file);
    bool updateFileLocally(File * file);
    bool updateFileLocally(QFileInfo * fileInfo);

    void deleteFile(File * file);
    bool deleteFileLocally(File * file);
    //void deleteDBIrrelevantFiles();

    File * findFileByPath(QString path);    //As the path and the filename are unique, finding a file by its filepath return only one file object

    void showDatabaseFiles();
    void checkFilesIntegrity();
    QList<File> findFileByIdDirectory(int iddirectory);
    QList<File> findFileByStatus(int status);
    void saveUpdatesToRemoteDB();
    void resetDatabaseFiles();

public slots:
    void handleEndofRequest(QNetworkReply*);
};

#endif // CLASSFILEMANAGER_H
