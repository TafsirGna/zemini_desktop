#include "localdbservice.h"

bool LocalDBService::dbInitStatus = false;

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
    dbTables2Init = Parameters::DB_INIT_DATA_LIST;
}

bool LocalDBService::isDbEmpty()
{
    User * user = getUserManager()->getUser();
    if (user == NULL)
        return true;
    return false;
}

bool LocalDBService::isDbInitialized()
{
    return LocalDBService::dbInitStatus;
}

const AbstractManager * LocalDBService::getManager(QString manager)
{
    if (manager == Parameters::DB_USER){
        return getUserManager();
    }

    if (manager == Parameters::DB_CATEGORY){
        return getCategoryManager();
    }

    if (manager == Parameters::DB_FILE_TYPE){
        return getFileTypeManager();
    }

    if (manager == Parameters::DB_FILE){
        return getFileManager();
    }

    if (manager == Parameters::DB_APP_DATA){
        return getAppDataManager();
    }

    if (manager == Parameters::DB_DRIVE_TYPE){
        return getDriveTypeManager();
    }

    if (manager == Parameters::DB_DRIVE){
        return getDriveManager();
    }

    if (manager == Parameters::DB_FILE_FORMAT){
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
    qDebug() << "file converted" << endl;
    QMap<QString, QString> parameters;
    parameters.insert("tableName", Parameters::DB_FILE);
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
    if (tableName == Parameters::DB_APP_DATA){
        return (DbEntity *) getAppDataManager()->add((AppData*) entity);
    }

    if (tableName == Parameters::DB_FILE){
        return (DbEntity *)getFileManager()->add((File *) entity);
    }

    if (tableName == Parameters::DB_FILE_TYPE){
        return (DbEntity *)getFileTypeManager()->add((FileType *) entity);
    }

    if (tableName == Parameters::DB_FILE_FORMAT){
        return (DbEntity *)getFileFormatManager()->add((FileFormat *)entity);
    }

    if (tableName == Parameters::DB_CATEGORY){
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
    if (notSavedFiles->size() == 0){
        return;
    }
    nbFiles2Send = notSavedFiles->size();
    emit filesToSend(Parameters::DB_FILE, (QList<DbEntity*>*)notSavedFiles);
}

void LocalDBService::onDbInit(QMap<QString, QString> metaData, QList<DbEntity*> * data)
{
    qDebug() << "in onDbInit function " << endl;
    QString entityName = metaData["objectType"];
    for (int i = 0; i < data->size(); i++){
        if (entityName == Parameters::DB_CATEGORY){
            Category *category = (Category *) data->at(i);

            QMap<QString, QString> parameters;
            parameters.insert("tableName", Parameters::DB_CATEGORY);
            if (!save(parameters, category))
                qDebug() << "category " << category->getName() <<" not inserted : " << endl;
        }
        if (entityName == Parameters::DB_FILE_FORMAT){
            FileFormat *format = (FileFormat *) data->at(i);

            QMap<QString, QString> parameters;
            parameters.insert("tableName", Parameters::DB_FILE_FORMAT);
            if (!save(parameters, format))
                qDebug() << "format " << format->getName() <<" not inserted : " << endl;
        }
        if (entityName == Parameters::DB_FILE_TYPE){
            FileType *type = (FileType *)data->at(i);

            QMap<QString, QString> parameters;
            parameters.insert("tableName", Parameters::DB_FILE_TYPE);
            if (!save(parameters, type))
                qDebug() << "type " << type->getName() <<" not inserted : " << endl;
        }
    }
    dbTables2Init.removeOne(metaData["objectType"]);
    //qDebug()  << metaData["objectType"] << dbTables2Init << endl;
    if (dbTables2Init.isEmpty()){
        completeDbInit();
        dbInitStatus = true;
        emit dbInitialized();
    }

    // storing the parameters in the data
    if (AppDataManager::add(new AppData("pullInDataFrequency", QString::number(5*60))) != NULL){
            qDebug() << "Succeeded in inserting the pull in frequency " << endl;
    }

    if (AppDataManager::add(new AppData("sendOutDataFrequency", QString::number(15))) != NULL){
        qDebug() << "Succeeded in inserting the send out frequency " << endl;
    }

    if (AppDataManager::add(new AppData("thumbsNumber", QString::number(4))) != NULL){
        qDebug() << "Succeeded in inserting the thumbs number setting " << endl;
    }

}

void LocalDBService::completeDbInit()
{
    //inserting all the drivetype objects
    getDriveTypeManager()->initDbTable();

    //CategoryManager::add(new Category(0, "Uncategorized"));

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
    if (parameters["tableName"] == Parameters::DB_APP_DATA){
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

    if (requestCode == Parameters::CODE_REGISTER_USER){
        qDebug() << "User saved : localdbservice" << endl;
        User* user = (User*) data->first();
        this->userManager->add(*user);
        emit userSaved(Parameters::CODE_REGISTER_USER);
        return;
    }

    if (requestCode == Parameters::CODE_USER_LOGIN){
        User* user = (User*) data->first();
        this->userManager->add(*user);
        emit userSaved(Parameters::CODE_USER_LOGIN);
        return;
    }

    if (requestCode == Parameters::CODE_FILE_SAVE){
        if (success){
            File * file = (File *) (data->first());
            QMap<QString, QString> map;
            map.insert("id", QString::number(file->getId()));
            file = FileManager::getOneBy(map);
            //qDebug() << "Finished" << endl;
            if (file->getThumbnail() == NULL){
                getFileManager()->setFileSaved(file->getId());
                nbFiles2Send--;
                emit fileBackedUp(file);
            }
            if (nbFiles2Send == 0)
                QTimer::singleShot(Parameters::CHECK_CON_TIME_OUT, this, SLOT(startBackingUp()));
        }
    }

    if (requestCode == Parameters::CODE_DB_INIT){
        onDbInit(metaData, data);
        return;
    }

    if (requestCode == Parameters::CODE_ACCOUNT_CHECKING){
        User * user = (User *) data->first();
        if (!user->isActivated()){
            emit userEnabled(false);
        }
        else{
            // we save it in the db
            User * m_user = UserManager::getUser();
            m_user->setActivated(true);
            UserManager::update(*m_user);
            //qDebug() << "popo : " << m_user->isActivated() << UserManager::getUser()->isActivated() <<  endl;
            emit userEnabled(true);
        }
    }

    if (requestCode == Parameters::CODE_SAVE_THUMBS){
        if (success){
            File * file = (File *) (data->first());
            QMap<QString, QString> map;
            map.insert("id", QString::number(file->getId()));
            qDebug() << "Thumbs saved !" << (file == NULL) << file->getId() << endl;
            file = FileManager::getOneBy(map);
            if (file->getThumbnail() != NULL){
                getFileManager()->setFileSaved(file->getId());
                nbFiles2Send--;
                emit fileBackedUp(file);
            }

            if (nbFiles2Send == 0)
                QTimer::singleShot(Parameters::CHECK_CON_TIME_OUT, this, SLOT(startBackingUp()));
        }
    }
}

QStringList LocalDBService::getSubDirNames()
{
    QList<Category> * categories = CategoryManager::getAll();
    int size = categories->size();
    QStringList subDirNames;
    for (int i(0); i < size; i++){
        Category category = categories->at(i);
        // No need to create a folder the "Uncategorized" category
        if (category.getName() != "Uncategorized")
            subDirNames.append(category.getName());
    }
    qDebug() << "SUBDIR " << subDirNames << endl;
    return subDirNames;
}
