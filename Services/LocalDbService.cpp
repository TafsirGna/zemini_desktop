#include "localdbservice.h"

const QString LocalDBService::USER = "User";
const QString LocalDBService::CATEGORY = "Category";
const QString LocalDBService::FILE = "File";
const QString LocalDBService::FILE_TYPE = "FileType";
const QString LocalDBService::APP_DATA = "App_data";
const QString LocalDBService::DRIVE_TYPE = "drive_type";
const QString LocalDBService::DRIVE = "Drive";
const QString LocalDBService::FILE_FORMAT = "FileFormat";
const QStringList LocalDBService::INIT_DATA_LIST = (QStringList() << LocalDBService::CATEGORY << LocalDBService::FILE_FORMAT << LocalDBService::FILE_TYPE );

UserManager * LocalDBService::userManager = NULL;
CategoryManager *LocalDBService::categoryManager = NULL;
FileTypeManager * LocalDBService::fileTypeManager = NULL;
FileManager *LocalDBService::fileManager = NULL;
AppDataManager * LocalDBService::appDataManager = NULL;
DriveTypeManager * LocalDBService::driveTypeManager = NULL;
DriveManager * LocalDBService::driveManager = NULL;
FileFormatManager * LocalDBService::fileFormatManager = NULL;

/***        Builders        ***/
LocalDBService::LocalDBService()
{
    dbTables2Init = INIT_DATA_LIST;
}

bool LocalDBService::isDbEmpty()
{
    User * user = getUserManager()->getUser();
    if (user == NULL)
        return true;
    return false;
}

const AbstractManager * LocalDBService::getManager(QString manager)
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
    QMap<QString, QString> parameters;
    parameters.insert("tableName", LocalDBService::FILE);
    return save(parameters, file);
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
CategoryManager *LocalDBService::getCategoryManager()
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

DbEntity *LocalDBService::save(QMap<QString, QString> parameters, DbEntity * entity)
{
    QString tableName = parameters["tableName"];
    if (tableName == LocalDBService::APP_DATA){
        return (DbEntity *) getAppDataManager()->add((AppData*) entity);
    }

    if (tableName == LocalDBService::FILE){
        return (DbEntity *)getFileManager()->add((File *) entity);
    }

    if (tableName == LocalDBService::FILE_TYPE){
        return (DbEntity *)getFileTypeManager()->add((FileType *) entity);
    }

    if (tableName == LocalDBService::FILE_FORMAT){
        return (DbEntity *)getFileFormatManager()->add((FileFormat *)entity);
    }

    if (tableName == LocalDBService::CATEGORY){
        return (DbEntity *)getCategoryManager()->add((Category *)entity);
    }
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
    emit filesToSend(LocalDBService::FILE, notSavedFiles);
}

void LocalDBService::onDbInit(QMap<QString, QString> metaData, QList<DbEntity*> * data)
{
    qDebug() << "in onDbInit function " << endl;
    QString entityName = metaData["objectType"];
    for (int i = 0; i < data->size(); i++){
        if (entityName == LocalDBService::CATEGORY){
            Category *category = (Category *) data->at(i);

            QMap<QString, QString> parameters;
            parameters.insert("tableName", LocalDBService::CATEGORY);
            if (save(parameters, category))
                qDebug() << "category " << category->getName() <<" not inserted : " << endl;
        }
        if (entityName == LocalDBService::FILE_FORMAT){
            FileFormat *format = (FileFormat *) data->at(i);

            QMap<QString, QString> parameters;
            parameters.insert("tableName", LocalDBService::FILE_FORMAT);
            if (save(parameters, format))
                qDebug() << "format " << format->getName() <<" not inserted : " << endl;
        }
        if (entityName == LocalDBService::FILE_TYPE){
            FileType *type = (FileType *)data->at(i);

            QMap<QString, QString> parameters;
            parameters.insert("tableName", LocalDBService::FILE_TYPE);
            if (save(parameters, type))
                qDebug() << "type " << type->getName() <<" not inserted : " << endl;
        }
    }
    dbTables2Init.removeOne(metaData["objectType"]);
    //qDebug()  << metaData["objectType"] << dbTables2Init << endl;
    if (dbTables2Init.isEmpty()){
        completeDbInit();
        emit dbInitialized();
    }
}

void LocalDBService::completeDbInit()
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

    //DriveManager::displayAll();
}

DbEntity *LocalDBService::getOneBy(QMap<QString, QString> parameters)
{
    if (parameters["tableName"] == LocalDBService::APP_DATA){
        return getAppDataManager()->getByKey(parameters["id"]);
    }
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

void LocalDBService::onRequestReplyReceived(QMap<QString, QString> metaData, QList<DbEntity*> *data)
{
    int requestCode = metaData["code"].toInt();
    bool success = ((metaData["success"] == "0") ? false : true);

    if (requestCode == NetworkService::CODE_REGISTER_USER){
        qDebug() << "User saved : localdbservice" << endl;
        User* user = (User*) data->first();
        this->userManager->add(*user);
        emit userSaved();
    }

    if (requestCode == NetworkService::CODE_FILE_SAVE){
        if (success){
            File * file = (File *) &(data->first());
            getFileManager()->setFileSaved(file->getId());
        }
    }

    if (requestCode == NetworkService::CODE_DB_INIT){
        onDbInit(metaData, data);
    }
}
