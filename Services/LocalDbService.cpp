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

    // Setting necessary arguments to convert the fileInfo to a File object
    getFileManager()->setCategoryManager(getCategoryManager());
    getFileManager()->setTypeManager(getTypeManager());
}

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
    qDebug() << "inserting file " << endl;
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
 * @brief LocalDBService::startBackingUp
 */
void LocalDBService::startBackingUp()
{
    // Backing up all files
    QList<File*> * notSavedFiles = getFileManager()->getNotSavedFiles();
    emit filesToSend(notSavedFiles);
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
    if (getFileManager()->symDeleteFile(file) && getFileManager()->updateFile(parentDir)){
        return true;
    }
    return false;
}

/**
 * @brief LocalDBService::updateFile
 * @return
 */
bool LocalDBService::saveFileUpdate(QFileInfo fileInfo)
{
    if (fileInfo.isDir()){
        qDebug() << "saving dir update !" << fileInfo.fileName() << endl;
        getFileManager()->updateDirContent(QDir(fileInfo.absoluteFilePath()));
        getFileManager()->updateFile(getFileManager()->convertToFile(fileInfo));
    }
    else{
        File * file = getFileManager()->convertToFile(fileInfo);
        File * parentDir = getFileManager()->convertToFile(QFileInfo(fileInfo.absolutePath()));
        //qDebug() << "File updated in test! " << endl;
        //qDebug() << "File updated in db! " << parentDir->toString() << endl;
        if (!(getFileManager()->updateFile(file) && getFileManager()->updateFile(parentDir))){
            return false;
        }
    }
    return true;
}
