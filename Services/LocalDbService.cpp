#include "localdbservice.h"

/***        Builders        ***/
LocalDBService::LocalDBService()
{
    userManager = NULL;
    categoryManager = NULL;
    typeManager = NULL;
    fileManager = NULL;
    directoryManager = NULL;
}

/***        Implementation of run function      ***/
//void LocalDBService::run()
//{
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
//}

bool LocalDBService::isDbEmpty()
{
    User * user = userManager->getUser();
    if (user == NULL)
        return true;
    return false;
}

AbstractManager * LocalDBService::getManager(QString manager)
{
    if (manager == LocalDBService::USER){
        if (userManager == NULL)
            userManager = new UserManager();
        return userManager;
    }

    if (manager == LocalDBService::CATEGORY){
        if (categoryManager == NULL)
            categoryManager = new CategoryManager();
        return categoryManager;
    }

    if (manager == LocalDBService::TYPE){
        if (typeManager == NULL)
            typeManager = new TypeManager();
        return typeManager;
    }

    if (manager == LocalDBService::DIR){
        if (directoryManager == NULL)
            directoryManager = new DirectoryManager();
        return directoryManager;
    }

    if (manager == LocalDBService::FILE){
        if (fileManager == NULL)
            fileManager = new FileManager();
        return fileManager;
    }

    return NULL;
}

bool LocalDBService::save(QFileInfo fileInfo)
{
    qDebug() << " in localDBservice save function " << endl;
    if (fileInfo.isDir()){
        Directory dir();
    }
    else if (fileInfo.isFile()){

    }
}
