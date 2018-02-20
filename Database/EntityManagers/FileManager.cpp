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
    qDebug() << "in saveFile : " << file->getFolder()->getPath() << endl;
    QString request = "insert into File values (NULL, "
            +QString::number(file->getFolder()->getId())+", "
            +QString::number(file->getType()->getId())+", "
            +QString::number(file->getCategory()->getId())+", '"
            +file->getCreatedAt().toString(Parameters::timeFormat)+"', '"+file->getUpdatedAt().toString(Parameters::timeFormat)+"', '"+file->getFileName()+"', '"+file->getPath()+"', "+QString::number(file->getSize())+", "+QString::number(((file->isSaved()) ? 1 : 0))+")";
    qDebug() << request << endl;
    query->exec(request);
    if (!query->isActive()){
        qDebug() << "Failed to save a file " << query->lastError().text() << endl;
        return false;
    }
    return true;
}

/**
 * @brief FileManager::updateFile
 * @param file
 * @return
 */
bool FileManager::updateFile(File *file)
{
    QString request = "update File set iddir = "+QString::number(file->getFolder()->getId())+" and idtype = "+file->getType()->getId()+" and idcategory = "+file->getCategory()->getId()+" and createdat = '"+file->getCreatedAt().toString(Parameters::timeFormat)+"' and updatedat = '"+file->getUpdatedAt().toString(Parameters::timeFormat)+"' and filename = '"+file->getFileName()+"' and path = '"+file->getPath()+"' and size = "+file->getSize()+" and saved = "+((file->isSaved()) ? 1 : 0)+" where id = "+file->getId();
    query->exec(request);
    if (!query->isActive()){
        qDebug() << "Failed to update a file " << query->lastError().text() << endl;
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
    QString request = "delete from File where id = "+file->getId();
    query->exec(request);
    if (!query->isActive()){
        qDebug() << "Failed to delete a file " << query->lastError().text() << endl;
        return false;
    }
    return true;
}

bool FileManager::cleanDirFile(QDir dir)
{
    QString request = "delete from File where iddirectory = (select id from Directory where path = '"+dir.absolutePath()+"')";
    query->exec(request);
    if(!query->isActive()){
        qDebug()<<"Cleaning dir's files : " + query->lastError().text();
        return false;
    }
    return true;
}

QList<File> FileManager::getAllFiles()
{
    QList<File> files;
    /*
    query->exec("select id,filename, createdat, size, saved, idtype, suffix, iddirectory from file where path = '"+path+"' ");
    if (!query->isActive())
        qDebug()<<"Error when selecting files : " + query->lastError().text();

    if (query->next())
    {
        Type * type = typeManager->findTypeById(query->value(5).toInt());
        Directory * directory = directoryManager->findDirectoryById(query->value(7).toInt());
        return new File(query->value(0).toInt(),query->value(1).toString(), query->value(2).toDateTime(), query->value(3).toInt(),path,query->value(4).toInt(),type, query->value(6).toString(), directory);
    }
    */
    return files;
}

QList<File> FileManager::getNotSavedFiles()
{
    QList<File> files;
    /*
    query->exec("select id,filename, createdat, size, saved, idtype, suffix, iddirectory from file where path = '"+path+"' ");
    if (!query->isActive())
        qDebug()<<"Error when selecting files : " + query->lastError().text();

    if (query->next())
    {
        Type * type = typeManager->findTypeById(query->value(5).toInt());
        Directory * directory = directoryManager->findDirectoryById(query->value(7).toInt());
        return new File(query->value(0).toInt(),query->value(1).toString(), query->value(2).toDateTime(), query->value(3).toInt(),path,query->value(4).toInt(),type, query->value(6).toString(), directory);
    }
    */
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
    File * file = new File(0, fileInfo.fileName(), fileInfo.absoluteFilePath(), fileInfo.created(), fileInfo.lastModified(), fileInfo.size(), 0, NULL, NULL, NULL);
    file->setCategory(getCategory(fileInfo));
    file->setType(getType(fileInfo));
    file->setFolder(getFolder(fileInfo));
    //qDebug() << "after type " << endl;
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

File * FileManager::getByFileName(QString fileName)
{
    QString request = "select id, filename, path, createdat, updatedat, size, saved from File where filename = '"+fileName+"'";
    query->exec(request);
    if (!query->isActive()){
        qDebug() << "Failed getting a file with "+fileName+" as filename "+query->lastError().text() << endl;
        return NULL;
    }
    if (query->next())
        return new File(query->value(0).toInt(), query->value(1).toString(), query->value(2).toString(), query->value(3).toDateTime(), query->value(4).toDateTime(), query->value(5).toInt(), query->value(6).toBool(), NULL, NULL, NULL);
    return NULL;
}



