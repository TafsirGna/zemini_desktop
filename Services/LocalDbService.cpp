#include "localdbservice.h"

const QString LocalDBService::USER = "user";
const QString LocalDBService::CATEGORY = "Category";
const QString LocalDBService::FILE = "file";
const QString LocalDBService::FILE_TYPE = "FileType";
const QString LocalDBService::APP_DATA = "app_data";
const QString LocalDBService::DRIVE_TYPE = "drive_type";
const QString LocalDBService::DRIVE = "drive";
const QString LocalDBService::FILE_FORMAT = "FileFormat";
const QStringList LocalDBService::INIT_DATA_LIST = (QStringList() << LocalDBService::CATEGORY << LocalDBService::FILE_FORMAT << LocalDBService::FILE_TYPE );

/***        Builders        ***/
LocalDBService::LocalDBService()
{
    userManager = NULL;
    categoryManager = NULL;
    fileTypeManager = NULL;
    fileManager = NULL;
    appDataManager = NULL;
    driveTypeManager = NULL;
    driveManager = NULL;
    fileFormatManager = NULL;
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

    if (manager == LocalDBService::FILE_TYPE){
        return getFileTypeManager();
    }

    if (manager == LocalDBService::FILE){
        return getFileManager();
    }

    if (manager == LocalDBService::APP_DATA){
        return getAppDataManager();
    }

    if (manager == LocalDBService::DRIVE_TYPE){
        return getDriveTypeManager();
    }

    if (manager == LocalDBService::DRIVE){
        return getDriveManager();
    }

    if (manager == LocalDBService::FILE_FORMAT){
        return getFileFormatManager();
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
    //qDebug() << "inserting file " << endl;
    // Converting ...
    File *file = getFileManager()->convertToFile(fileInfo);
    //qDebug() << "file converted" << endl;
    return save(file);
}

/**
 * @brief LocalDBService::save
 * @param user
 * @return
 */
bool LocalDBService::save(User * user)
{
    qDebug() << "User saved : localdbservice" << endl;
    this->userManager->add(*user);
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

DriveTypeManager *LocalDBService::getDriveTypeManager()
{
    if (driveTypeManager == NULL)
        driveTypeManager = new DriveTypeManager();
    return driveTypeManager;
}

DriveManager *LocalDBService::getDriveManager()
{
    if (driveManager == NULL)
        driveManager = new DriveManager();
    return driveManager;
}

FileFormatManager *LocalDBService::getFileFormatManager()
{
    if (fileFormatManager == NULL)
        fileFormatManager = new FileFormatManager();
    return fileFormatManager;
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
 * @brief LocalDBService::getFileTypeManager
 * @return
 */
FileTypeManager * LocalDBService::getFileTypeManager()
{
    if (fileTypeManager == NULL)
        fileTypeManager = new FileTypeManager();
    return fileTypeManager;
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

DbEntity *LocalDBService::save(File *file)
{
    return (DbEntity *)getFileManager()->add(file);
}

DbEntity * LocalDBService::save(FileType *type)
{
    return (DbEntity *)getFileTypeManager()->add(type);
}

DbEntity * LocalDBService::save(FileFormat *format)
{
    return (DbEntity *)getFileFormatManager()->add(format);
}

DbEntity *LocalDBService::save(Category *category)
{
    return (DbEntity *)getCategoryManager()->add(category);
}

bool LocalDBService::update(AppData* appData)
{
    return getAppDataManager()->update(appData);
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

void LocalDBService::initDb(QList<DbEntity> * entities, QString entityName)
{
    // inserting the data received from the remote server
    if (entities == NULL){
        emit dbTableInitialized();
        return;
    }

    for (int i = 0; i < entities->size(); i++){
        if (entityName == LocalDBService::CATEGORY){
            Category *category = (Category *) &(entities->at(i));
            if (save(category))
                qDebug() << "category " << category->getName() <<" not inserted : " << endl;
        }
        if (entityName == LocalDBService::FILE_FORMAT){
            FileFormat *format = (FileFormat *) &(entities->at(i));
            if (save(format))
                qDebug() << "format " << format->getName() <<" not inserted : " << endl;
        }
        if (entityName == LocalDBService::FILE_TYPE){
            FileType *type = (FileType *) &(entities->at(i));
            if (save(type))
                qDebug() << "type " << type->getName() <<" not inserted : " << endl;
        }
    }
    qDebug() << "Emitting the entity name" << endl;
    emit dbTableInitialized();
}

void LocalDBService::initDb()
{
    //inserting all the drivetype objects
    getDriveTypeManager()->initDbTable();

    // inserting other necessary data
    getFileTypeManager()->add(new FileType(0, "directory", "", NULL));

    // initializing the datetime of the last exit from the app
    getAppDataManager()->add(new AppData(AppData::LAST_EXIT, "0000-00-00 00:00:00"));

    // Creating a drive object corresponding to the device on which the software is being installed
    QMap<QString, QString> properties;
    properties.insert("Name", DriveType::COMPUTER);
    DriveType * computerType = getDriveTypeManager()->getOneBy(properties);
    getDriveManager()->add(new Drive(0, QDir().homePath(), computerType));

    DriveManager::displayAll();
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
    if (getFileManager()->symDelete(file) && getFileManager()->update(parentDir)){
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
        getFileManager()->update(getFileManager()->convertToFile(fileInfo));
    }
    else{
        File * file = getFileManager()->convertToFile(fileInfo);
        File * parentDir = getFileManager()->convertToFile(QFileInfo(fileInfo.absolutePath()));
        if (!(getFileManager()->update(file) && getFileManager()->update(parentDir))){
            return false;
        }
    }
    return true;
}
