#include "localdbservice.h"

const QString LocalDBService::USER = "user";
const QString LocalDBService::CATEGORY = "category";
const QString LocalDBService::FILE = "file";
const QString LocalDBService::TYPE = "type";
const QString LocalDBService::APP_DATA = "app_data";

/***        Builders        ***/
LocalDBService::LocalDBService()
{
    userManager = NULL;
    categoryManager = NULL;
    typeManager = NULL;
    fileManager = NULL;
    appDataManager = NULL;

    //getFileManager()->printAllFiles();
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

    if (manager == LocalDBService::APP_DATA){
        return getAppDataManager();
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

AppDataManager * LocalDBService::getAppDataManager()
{
    if (appDataManager == NULL)
        appDataManager = new AppDataManager();
    return appDataManager;
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

bool LocalDBService::update(AppData* appData)
{
    return getAppDataManager()->updateAppData(appData);
}

/**
 * @brief LocalDBService::saveDirChange
 * @param fileInfo
 */
void LocalDBService::saveDirChange(QFileInfo fileInfo)
{
    qDebug() << "saving dir'change !" << endl;
    QDir dir(fileInfo.absoluteFilePath());
    getFileManager()->updateDirContent(dir);
    getFileManager()->updateFile(getFileManager()->convertToFile(fileInfo));
}

bool LocalDBService::saveDirDeletion(QFileInfo dir)
{
    File * file = getFileManager()->convertToFile(dir);
    QFileInfo parentDir(dir.absolutePath());
    if (getFileManager()->deleteFile(file) && getFileManager()->updateFile(getFileManager()->convertToFile(parentDir))){
        emit fileToSend(file);
        return true;
    }
    return false;
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

    // initializing the datetime of the last exit from the app
    getAppDataManager()->addAppData(new AppData(AppData::LAST_EXIT, "0000-00-00 00:00:00"));
}

/**
 * @brief LocalDBService::refreshDb
 */
void LocalDBService::refreshDb()
{

}

/**
 * @brief LocalDBService::deleteFile
 * @return
 */
bool LocalDBService::saveFileDeletion(QFileInfo fileInfo)
{
    File * file = getFileManager()->convertToFile(fileInfo);
    File * parentDir = getFileManager()->convertToFile(QFileInfo(fileInfo.absolutePath()));
    if (getFileManager()->deleteFile(file) && getFileManager()->updateFile(parentDir)){
        emit fileToSend(file);
        return true;
    }
    return false;
}

/**
 * @brief LocalDBService::updateFile
 * @return
 */
bool LocalDBService::saveFileChange(QFileInfo fileInfo)
{
    File * file = getFileManager()->convertToFile(fileInfo);
    File * parentDir = getFileManager()->convertToFile(QFileInfo(fileInfo.absolutePath()));
    //qDebug() << "File updated in test! " << endl;
    //qDebug() << "File updated in db! " << parentDir->toString() << endl;
    if (getFileManager()->updateFile(file) && getFileManager()->updateFile(parentDir)){
        emit fileToSend(file);
        return true;
    }
    return false;
}
