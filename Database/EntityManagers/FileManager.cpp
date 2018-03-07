#include "Database/EntityManagers/FileManager.h"

/***            Default constructor         ***/
FileManager::FileManager() : AbstractManager()
{
    this->typeManager = new TypeManager();
    this->categoryManager = new CategoryManager();
}

/**
 * @brief FileManager::saveFile
 * @param file
 * @return
 */
bool FileManager::saveFile(File * file)
{
    qDebug() << "yes" << endl;
    QString request = "insert into File(id, iddir, idtype, idcategory, createdat, updatedat, filename, path, size, saved, thumbnail) select NULL, "
            +((file->getFolder() == NULL) ? "NULL" : QString::number(file->getFolder()->getId()))+", "
            +QString::number(file->getType()->getId())+", "
            +QString::number(file->getCategory()->getId())+", '"
            +file->getCreatedAt().toString(Parameters::timeFormat)
            +"', '"+file->getUpdatedAt().toString(Parameters::timeFormat)
            +"', '"+file->getFileName()+"', '"+file->getPath()
            +"', "+QString::number(file->getSize())+", "+QString::number(((file->isSaved()) ? 1 : 0))
            +", "+ ((file->getThumbnail() == NULL) ? "NULL": "'"+Functions::getRelativePath(file->getThumbnail()->absoluteFilePath())+"'")
            +" where not exists (select 1 from File where path = '"+file->getPath()+"' and filename = '"+file->getFileName()+"');";
    //qDebug() << request << endl;
    query->exec(request);
    if (!query->isActive()){
        qDebug() << "Failed to save a file " << query->lastError().text() << endl;
        return false;
    }
    printAllFiles();
    return true;
}

/**
 * @brief FileManager::updateFile
 * @param file
 * @return
 */
bool FileManager::updateFile(File *file)
{
    QString request = "update File set iddir = "+((file->getFolder() == NULL) ? "NULL" : QString::number(file->getFolder()->getId()))
            +" and idtype = "+QString::number(file->getType()->getId())+" and idcategory = "+QString::number(file->getCategory()->getId())+" and createdat = '"+file->getCreatedAt().toString(Parameters::timeFormat)+"' and updatedat = '"+file->getUpdatedAt().toString(Parameters::timeFormat)+"' and filename = '"+file->getFileName()+"' and path = '"+file->getPath()+"' and size = "+QString::number(file->getSize())+" and saved = "+QString::number(((file->isSaved()) ? 1 : 0))+" and thumbnail = "
            +((file->getThumbnail() == NULL) ? "NULL": "'"+Functions::getRelativePath(file->getThumbnail()->absoluteFilePath())+"'")
            +" where id = "+QString::number(file->getId());
    //qDebug() << "File updated in db! " << request << endl;
    QSqlQuery sqlQuery(*query);
    sqlQuery.exec(request);
    if (!sqlQuery.isActive()){
        qDebug() << "Failed to update a file " << sqlQuery.lastError().text() << endl;
        return false;
    }
    return true;
}

/**
 * @brief FileManager::deleteFile
 * @param file
 * @return
 */
bool FileManager::deleteFile(File *file)
{
    QString request = "delete from File where id = "+QString::number(file->getId());
    qDebug() << "deleting : " << request << endl;
    query->exec(request);
    if (!query->isActive()){
        qDebug() << "Failed to delete a file " << query->lastError().text() << endl;
        return false;
    }
    //printAllFiles();
    qDebug() << "Succeed in deleting a file " << query->lastError().text() << endl;
    return true;
}

bool FileManager::updateDirContent(QDir dir)
{
    QFileInfoList children_dirs = dir.entryInfoList(QDir::AllDirs | QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    int size(children_dirs.size());
    for (int i(0); i < size ; i ++){
        QFileInfo child = children_dirs.at(i);

        // i get all the recently updated or created files
        QDateTime createdAt = child.created(),
                currentDateTime = QDateTime::currentDateTime();
        if (createdAt.date() == currentDateTime.date() and createdAt.time().hour() == currentDateTime.time().hour() and (currentDateTime.time().minute() - createdAt.time().minute()) < Parameters::MIN_CREAT_DELAY){
            if (!saveFile(convertToFile(child)))
                continue;
            qDebug() << "Updating Dir'content !" << endl;
            emit fileSaved(child);
            emit fileSaved(convertToFile(child));
        }
    }
    return true;
}

QList<File*>* FileManager::getAllFiles()
{
    QList<File*> *files = new QList<File *>();
    QString request = "SELECT id, iddir, idtype, idcategory, createdat, updatedat, filename, path, size, saved, thumbnail FROM File ";
    QSqlQuery sqlQuery(*query);
    sqlQuery.exec(request);
    if (!sqlQuery.isActive()){
        qDebug()<<"Error when selecting all files : " + sqlQuery.lastError().text();
        return NULL;
    }

    while (sqlQuery.next()){
        File * file = new File(sqlQuery.value(0).toInt(),sqlQuery.value(6).toString(), sqlQuery.value(7).toString(), sqlQuery.value(4).toDateTime(),sqlQuery.value(5).toDateTime(), sqlQuery.value(8).toInt(), sqlQuery.value(9).toBool(), new QFileInfo(sqlQuery.value(10).toString()),NULL, NULL, NULL);
        QFileInfo fileInfo(QDir().homePath()+file->getPath()+"/"+file->getFileName());
        file->setFolder(getFolder(fileInfo));
        file->setCategory(getCategory(fileInfo));
        file->setType(getType(fileInfo));
        files->append(file);
    }
    return files;
}

QList<File*> * FileManager::getNotSavedFiles()
{
    QList<File*> *files = new QList<File *>();

    query->exec("select id, iddir, idtype, idcategory, createdat, updatedat, filename, path, size, saved, thumbnail from File where saved = 0");
    if (!query->isActive()){
        qDebug()<<"Error when selecting not saved files : " + query->lastError().text();
        return NULL;
    }
    while (query->next()){
        File * file = new File(query->value(0).toInt(),query->value(6).toString(), query->value(7).toString(), query->value(4).toDateTime(),query->value(5).toDateTime(), query->value(8).toInt(), query->value(9).toBool(), new QFileInfo(query->value(10).toString()),NULL, NULL, NULL);
        QFileInfo fileInfo(QDir().homePath()+file->getPath()+"/"+file->getFileName());
        file->setFolder(getFolder(fileInfo));
        file->setCategory(getCategory(fileInfo));
        file->setType(getType(fileInfo));
        files->append(file);
    }
    return files;
}

bool FileManager::setFileSaved(int fileId)
{
    QString request = "update file set saved = '"+QString::number(File::FILE_SAVED)+"' where id = '"+QString::number(fileId)+"'";
    //qDebug() << request;
    query->exec(request);
    if(!query->isActive())
    {
        qDebug()<<"Error when changing file status : " + query->lastError().text();
        return false;
    }
    return true;
}

/**
 * @brief FileManager::getCategory
 * @param file
 * @return Category
 */
Category * FileManager::getCategory(QFileInfo fileInfo)
{
    QString path = fileInfo.absoluteFilePath();
    QStringList pathTab = path.split("/");
    int index  = pathTab.indexOf(Parameters::ROOT_DIR_NAME);
    return categoryManager->getByName(pathTab.at(index+1));
}

/**
 * @brief FileManager::getFolder
 * @param file
 * @return
 */
File * FileManager::getFolder(QFileInfo fileInfo)
{
    QStringList pathTab = fileInfo.absoluteFilePath().split("/");
    int index = pathTab.indexOf(fileInfo.fileName());
    if (index <= 1)
        return NULL;
    //qDebug() << "index : " << index << endl;
    return getByFileName(pathTab.at(index-1));
}

/**
 * @brief FileManager::getFileID
 * @return
 */
File * FileManager::getFileAt(QFileInfo fileInfo)
{
    QString request = "select id, iddir, idtype, idcategory, createdat, updatedat, filename, path, size, saved, thumbnail from File where path = '"+Functions::getRelativePath(fileInfo.absolutePath())+"' and filename = '"+fileInfo.fileName()+"'";
    QSqlQuery sqlQuery(*query);
    sqlQuery.exec(request);
    if (!sqlQuery.isActive()){
        qDebug()<<"Error when selecting the file : " + sqlQuery.lastError().text();
        return NULL;
    }

    if (sqlQuery.next()){
        File * file = new File(sqlQuery.value(0).toInt(),sqlQuery.value(6).toString(), sqlQuery.value(7).toString(), sqlQuery.value(4).toDateTime(),sqlQuery.value(5).toDateTime(), sqlQuery.value(8).toInt(), sqlQuery.value(9).toBool(), new QFileInfo(sqlQuery.value(10).toString()),NULL, NULL, NULL);
        QFileInfo fileInfo(QDir().homePath()+file->getPath()+"/"+file->getFileName());
        file->setFolder(getFolder(fileInfo));
        file->setCategory(getCategory(fileInfo));
        file->setType(getType(fileInfo));
        return file;
    }
    return NULL;
}

/**
 * @brief FileManager::getType
 * @param file
 * @return
 */
Type * FileManager::getType(QFileInfo fileInfo)
{
    if (fileInfo.isDir())
        return typeManager->getByName("directory");

    Type * type = typeManager->getBySuffix(fileInfo.suffix());
    if (type == NULL)
        return new Type(0, fileInfo.suffix(), fileInfo.suffix());
    return type;
}

/**
 * @brief FileManager::convertToFile
 * @param fileInfo
 * @return
 */
File *FileManager::convertToFile(QFileInfo fileInfo)
{
    File * savedFile = getFileAt(fileInfo);
    File * file = NULL;
    if (savedFile != NULL){
        file = savedFile;
        file->setSize(fileInfo.size());
        file->setUpdatedAt(fileInfo.lastModified());
        file->setSaved(false);
    }
    else{
        file = new File(0, fileInfo.fileName(), Functions::getRelativePath(fileInfo.absolutePath()), fileInfo.created(), fileInfo.lastModified(), fileInfo.size(), false, ((fileInfo.isDir()) ? NULL : Functions::generateThumbnails(fileInfo)), NULL, NULL, NULL);
        file->setCategory(getCategory(fileInfo));
        file->setType(getType(fileInfo));
        file->setFolder(getFolder(fileInfo));
    }

    /*
    if (file == NULL)
        qDebug() << "File null !" <<endl;
    else
        qDebug() << "File not null !" << file->toString() << endl;
        */


    return file;
}

void FileManager::setTypeManager(TypeManager * typeManager)
{
    this->typeManager = typeManager;
}

void FileManager::setCategoryManager(CategoryManager * categoryManager)
{
    this->categoryManager = categoryManager;
}

/**
 * @brief FileManager::getByFileName
 * @param fileName
 * @return
 */
File * FileManager::getByFileName(QString fileName)
{
    QString request = "select id, filename, path, createdat, updatedat, size, saved, thumbnail from File where filename = '"+fileName+"'";
    query->exec(request);
    if (!query->isActive()){
        qDebug() << "Failed getting a file with "+fileName+" as filename "+query->lastError().text() << endl;
        return NULL;
    }
    if (query->next())
        return new File(query->value(0).toInt(), query->value(1).toString(), query->value(2).toString(), query->value(3).toDateTime(), query->value(4).toDateTime(), query->value(5).toInt(), query->value(6).toBool(), new QFileInfo(query->value(7).toString()), NULL, NULL, NULL);
    return NULL;
}

/**
 * @brief FileManager::printAllFiles
 */
void FileManager::printAllFiles()
{
    QList<File *> * files = getAllFiles();
    int size = files->size();
    for (int i(0); i < size; i++){
        qDebug() << files->at(i)->toString() << endl;
    }
}



