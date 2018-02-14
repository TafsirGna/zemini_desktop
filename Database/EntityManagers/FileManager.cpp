#include "Database/EntityManagers/FileManager.h"

/***            Default constructor         ***/
FileManager::FileManager() : AbstractManager()
{
    this->typeManager = new TypeManager();
    //this->filestosave = new QList<File>();
}

FileManager::FileManager(TypeManager * typeManager): AbstractManager()
{
    this->typeManager = typeManager;
    //this->filestosave = new QList<File>();
}

/**
 * @brief FileManager::saveFile
 * @param file
 * @return
 */
bool FileManager::saveFile(File * file)
{
    QString request = "insert into File values (NULL, "
            +QString::number(file->getFolder()->getId())+", "
            +file->getType()->getId()+", "
            +file->getCategory()->getId()+", '"
            +file->getCreationTime().toString(Parameters::timeFormat)+"', '"+file->getUpdateTime().toString(Parameters::timeFormat)+"', '"+file->getFileName()+"', '"+file->getPath()+"', "+file->getSize()+", "+((file->isSaved()) ? 1 : 0)+")";
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
    QString request = "update File set iddir = "+QString::number(file->getFolder()->getId())+" and idtype = "+file->getType()->getId()+" and idcategory = "+file->getCategory()->getId()+" and creationtime = '"+file->getCreationTime().toString(Parameters::timeFormat)+"' and updatetime = '"+file->getUpdateTime().toString(Parameters::timeFormat)+"' and filename = '"+file->getFileName()+"' and path = '"+file->getPath()+"' and size = "+file->getSize()+" and saved = "+((file->isSaved()) ? 1 : 0)+" where id = "+file->getId();
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
    query->exec("select id,filename, datecreation, size, saved, idtype, suffix, iddirectory from file where path = '"+path+"' ");
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
    query->exec("select id,filename, datecreation, size, saved, idtype, suffix, iddirectory from file where path = '"+path+"' ");
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

/***            here i wants to save the files into the local database          ***/
/*
bool FileManager::saveFileLocally(File *file)
{
    int max_id = -1;
    // retreiving the max id from the table file
    query->exec("select max(id) from file");
    if(!query->isActive())
    {
        qDebug()<<"Error when selecting max id file : " + query->lastError().text();
        return false;
    }

    if (query->next())
    {
        max_id = query->value(0).toInt();
        max_id++;
    }

    QString request = "INSERT INTO file VALUES('"+QString::number(max_id)+"',"+QString::number(file->getDirectory()->getId())+","+QString::number(file->getType()->getId())+",'"+file->getDatecreation().toString(Parameters::timeFormat)+"','"+file->getName()+"','"+file->getPath()+"','"+QString::number(file->getSize())+"','"+QString::number(file->getStatus())+"','"+file->getSuffix()+"')";
    //qDebug() << request;

    query->exec(request);
    if(!query->isActive())
    {
        qDebug()<<"Erreur insertion file : " + query->lastError().text();
        return false;
    }

    return true;
}
*/

/*
bool FileManager::updateFileLocally(File *file)
{
    //QString request = "update file set filename = '"+file->getName()+"', size = '"+QString::number(file->getSize())+"', status = '"+QString::number(file->getStatus())+"', path = '"+file->getPath()+"', datecreation = '"+file->getDatecreation().toString(ZeminiParameters::timeFormat())+"' where id = '"+QString::number(file->getId())+"'";
    QString request = "update file set status = '"+QString::number(file->getStatus())+"' where id = '"+QString::number(file->getId())+"'";
    //qDebug() << request;

    query->exec(request);
    if(!query->isActive())
    {
        qDebug()<<"Error when updating file : " + query->lastError().text();
        return false;
    }
    return true;
}

bool FileManager::updateFileLocally(QFileInfo *fileInfo)
{
    File * file = convertToFile(*fileInfo);
    return updateFileLocally(file);
}
*/
File * FileManager::convertToFile(QFileInfo fileInfo)
{
    /*
    Directory * directory = directoryManager->findDirectoryByPath(fileInfo.path());
    Type * type = typeManager->findTypeByName(Parameters::getTypeByPath(fileInfo.path()));
    type->getId();
    return new File(fileInfo.baseName(),fileInfo.created(),fileInfo.size(),fileInfo.filePath(),2,type,fileInfo.suffix(),directory);
    */
}



