#include "localdbservice.h"

/***        Builders        ***/
LocalDBService::LocalDBService(QSqlDatabase db, QObject *parent) : QThread(parent)
{
    fileInfoList = new QFileInfoList();
    this->db = db;

    // Initializing entity managers
    categoryManager = new CategoryManager(db);
    typeManager = new TypeManager(db,categoryManager);
    dirManager = new DirectoryManager(db,typeManager);
    fileManager= new FileManager(db,typeManager,dirManager);
    userManager = new UserManager(db);
}

/***        Implementation of run function      ***/
void LocalDBService::run()
{
    /*
    if (fileInfoList->size() == 0)
        return;

    fileManager->resetDatabaseFiles();
    dirManager->resetDatabaseDirectories();

    qDebug() << "directories' update ";
    for (int i=0;i<fileInfoList->size();i++)
    {
        QFileInfo element = fileInfoList->at(i);

        if (element.isDir())
        {
            //qDebug() << element.filePath();
            Directory * dir = dirManager->findDirectoryByPath(element.filePath());
            if (dir == NULL)
            {
                dirManager->saveDirectoryLocally(&element);
            }
            else
            {
                dir->setStatus(1);
                dirManager->updateDirectoryLocally(dir);
            }
        }
    }

    qDebug() << "files' update ";
    for (int i=0;i<fileInfoList->size();i++)
    {
        QFileInfo element = fileInfoList->at(i);

        if (!element.isDir())
        {
            //qDebug() << element.filePath();
            File * file = fileManager->findFileByPath(element.filePath());

            if (file == NULL)
            {
                fileManager->saveFileLocally(&element);
            }
            else
            {
                file->setStatus(1);
                fileManager->updateFileLocally(file);
            }
        }
    }
    terminateLocalDatabaseUpdate();

    emit dataSavedLocally();
    */
}

void LocalDBService::terminateLocalDatabaseUpdate()
{
    // i check the integrity of the local database
    dirManager->checkDirsIntegrity();
    fileManager->checkFilesIntegrity();
}

/***        This function passes arguments from the parent object to this thread        ***/
void LocalDBService::setArguments(QFileInfoList *fileInfoList)
{
    this->fileInfoList = fileInfoList;
}

void LocalDBService::setEntitiesManagers(DirectoryManager *dirManager, FileManager *fileManager)
{
    this->fileManager = fileManager;
    this->dirManager = dirManager;
}

bool LocalDBService::isDbEmpty()
{
    User * user = userManager->getUser();
    if (user == NULL)
        return true;
    return false;
}

UserManager * LocalDBService::getUserManager()
{
    return userManager;
}
