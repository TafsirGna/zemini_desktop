#include "localdbservice.h"

const QString LocalDBService::USER = "user";
const QString LocalDBService::CATEGORY = "category";
const QString LocalDBService::DIR = "directory";
const QString LocalDBService::FILE = "file";
const QString LocalDBService::TYPE = "type";

/***        Builders        ***/
LocalDBService::LocalDBService()
{
    userManager = NULL;
    categoryManager = NULL;
    typeManager = NULL;
    fileManager = NULL;
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
    User * user = getUserManager()->getUser();
    if (user == NULL)
        return true;
    return false;
}

AbstractManager * LocalDBService::getManager(QString manager)
{
    if (manager == LocalDBService::USER){
        return getUserManager();
    }

    if (manager == LocalDBService::CATEGORY){
        return getCategoryManager();
    }

    if (manager == LocalDBService::TYPE){
        return getTypeManager();
    }

    if (manager == LocalDBService::FILE){
        return getFileManager();
    }

    return NULL;
}

/**
 * @brief LocalDBService::save
 * @param fileInfo
 * @return
 */
bool LocalDBService::save(QFileInfo fileInfo)
{
    // Setting necessary arguments to convert the fileInfo to a File object
    getFileManager()->setCategoryManager(getCategoryManager());
    getFileManager()->setTypeManager(getTypeManager());
    // Converting ...
    File *file = getFileManager()->convertToFile(fileInfo);
    return save(file);
}

/**
 * @brief LocalDBService::save
 * @param user
 * @return
 */
bool LocalDBService::save(User * user)
{
    this->userManager->insertUser(*user);
    emit userLoggedIn();
}

/**
 * @brief LocalDBService::getUserManager
 * @return
 */
UserManager * LocalDBService::getUserManager()
{
    if (userManager == NULL)
        userManager = new UserManager();
    return userManager;
}

/**
 * @brief LocalDBService::getCategoryManager
 * @return
 */
CategoryManager * LocalDBService::getCategoryManager()
{
    if (categoryManager == NULL)
        categoryManager = new CategoryManager();
    return categoryManager;
}

/**
 * @brief LocalDBService::getTypeManager
 * @return
 */
TypeManager * LocalDBService::getTypeManager()
{
    if (typeManager == NULL)
        typeManager = new TypeManager();
    return typeManager;
}

/**
 * @brief LocalDBService::getFileManager
 * @return
 */
FileManager * LocalDBService::getFileManager()
{
    if (fileManager == NULL)
        fileManager = new FileManager();
    return fileManager;
}

bool LocalDBService::save(File *file)
{
    return getFileManager()->saveFile(file);
}

void LocalDBService::updateDirContent(QDir dir)
{
    getFileManager()->updateDbDir(dir);
}

bool LocalDBService::deleteDir(QDir dir)
{
    return false;//getDirectoryManager()->delete();
}

/**
 * @brief LocalDBService::startBackingUp
 */
void LocalDBService::startBackingUp()
{
    // Backing up all files
    QList<File*> * notSavedFiles = getFileManager()->getNotSavedFiles();
    int size = notSavedFiles->size();
    for(int i = 0; i < size; i++){
        emit fileToSend(notSavedFiles->at(i));
    }
}

/**
 * @brief LocalDBService::markFileSaved
 * @param fileId
 */
void LocalDBService::markFileSaved(int fileId)
{
    getFileManager()->setFileSaved(fileId);
}

void LocalDBService::initDb(QList<Category> * categories)
{
    // inserting the data received from the remote server
    if (categories != NULL){
        for (int i = 0; i < categories->size(); i++){
            Category category = categories->at(i);
            if (!getCategoryManager()->addCategory(category))
                qDebug() << "category " << category.getName() <<" not inserted : " << endl;
        }
    }

    // inserting other necessary data
    getTypeManager()->insertType(new Type(0, "directory", ""));
}

/**
 * @brief LocalDBService::refreshDb
 */
void LocalDBService::refreshDb()
{

}
