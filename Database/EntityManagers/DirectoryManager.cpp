#include "Database\EntityManagers\DirectoryManager.h"

/***        Builders        ***/
DirectoryManager::DirectoryManager() : AbstractManager()
{
    this->typeManager = new TypeManager();
}

DirectoryManager::DirectoryManager(QSqlDatabase &db,TypeManager * typeManager) : AbstractManager(db)
{
    str_Url = Parameters::url()+"/FileDbClasses/TestDirectoryManager.php";
    this->typeManager = typeManager;

    connect(this->accessManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(handleEndofRequest(QNetworkReply*)));

    showDatabaseDirectories();
}

DirectoryManager::DirectoryManager(QSqlDatabase &db) : AbstractManager(db)
{
    str_Url = Parameters::url()+"/FileDbClasses/TestDirectoryManager.php";
    this->typeManager = new TypeManager(db);

    connect(this->accessManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(handleEndofRequest(QNetworkReply*)));

    showDatabaseDirectories();
}

void DirectoryManager::saveDirectory(Directory *directory)
{

    QUrl params(str_Url+"?action=1"+    //action (1) shows we want to save updates
                "&filename=" + directory->getName() +
                "&size="+ QString::number(directory->getSize())+
                "&datecreation="+ directory->getDatecreation().toString(Parameters::timeFormat())
                );

    //request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    this->accessManager->get(QNetworkRequest(params));

}

/***        functions to save directories into the database     ***/
bool DirectoryManager::saveDirectoryLocally(Directory * directory)
{

    int max_id = -1;
    // retreiving the maximum id from the table file
    query->exec("select max(id) from directory");
    if(!query->isActive())
    {
        qDebug()<<"Error when selecting max id directory : " + query->lastError().text();
        return false;
    }

    if (query->next())
    {
        max_id = query->value(0).toInt();
        max_id++;
    }

    QString requete = "INSERT INTO directory(id,idtype,datecreation, name, size,status, path, iddirectory) VALUES("+QString::number(max_id)+","+QString::number(directory->getType()->getId())+",'"+directory->getDatecreation().toString(Parameters::timeFormat())+"','"+directory->getName()+"',"+QString::number(directory->getSize())+","+QString::number(directory->getStatus())+",'"+directory->getPath()+"',"+((directory->getDirectory() == NULL) ? QString::number(0) : QString::number(directory->getDirectory()->getId()))+")";
    //qDebug() << requete;
    query->exec(requete);
    if(!query->isActive())
    {
        qDebug()<<"Erreur insertion directory : " + query->lastError().text();
        return false;
    }
    return true;
}

bool DirectoryManager::saveDirectoryLocally(QFileInfo *fileInfo)
{
    //qDebug() << "Saving directory";
    return saveDirectoryLocally(convertToDirectory(fileInfo));
}

Directory * DirectoryManager::convertToDirectory(QFileInfo *fileInfo)
{
    Directory *parentDirectory = findDirectoryByPath(fileInfo->path());
    Type * type = typeManager->findTypeByName(Parameters::getTypeByPath(fileInfo->path()));
    return new Directory(fileInfo->baseName(),fileInfo->created(),fileInfo->size(),fileInfo->filePath(),2,type,parentDirectory);
}

Directory * DirectoryManager::findDirectoryByPath(QString path)
{
    // then if there's a directory with these specs, we retrieve the values that we need
    query->exec("select id,name,datecreation,size,status,idtype,iddirectory from directory where path = '"+path+"' ");
    if (!query->isActive())
    {
        qDebug()<<"Error when selecting directory : " + query->lastError().text();
        return NULL;
    }

    if (query->next())
    {
        Type * type = typeManager->findTypeById(query->value(5).toInt());
        Directory * parentDirectory = findDirectoryById(query->value(6).toInt());
        return new Directory(query->value(0).toInt(),query->value(1).toString(), query->value(2).toDateTime(), query->value(3).toInt(),path,query->value(4).toInt(),type, parentDirectory);
    }
    else
        return NULL;
}

Directory * DirectoryManager::findDirectoryById(int id)
{
    if (id == 0)
        return NULL;

    // then if there's a directory with these specs, we retrieve the values that we need
    query->exec("select id,name,datecreation,size,status,idtype,path,iddirectory from directory where id = '"+QString::number(id)+"' ");
    if (!query->isActive())
    {
        qDebug()<<"Error when selecting directory : " + query->lastError().text();
        return NULL;
    }

    if (query->next())
    {
        Type * type = typeManager->findTypeById(query->value(5).toInt());
        Directory * parentDirectory = findDirectoryById(query->value(7).toInt()); ;
        return new Directory(query->value(0).toInt(),query->value(1).toString(), query->value(2).toDateTime(), query->value(3).toInt(),query->value(6).toString(),query->value(4).toInt(),type, parentDirectory);
    }
    else
        return NULL;
}

QList<Directory> DirectoryManager::findDirectoryByIdDirectory(int iddirectory)
{
    QList<Directory> dirsList;
    // then if there's a directory with these specs, we retrieve the values that we need
    query->exec("select id,name,datecreation,size,status,idtype,iddirectory,path from directory where iddirectory = "+QString::number(iddirectory)+" ");
    if (!query->isActive())
    {
        qDebug()<<"Error when selecting directory : " + query->lastError().text();
        return dirsList;
    }

    while(query->next())
    {
        //qDebug() << " --- query->value(5).toInt() : "+ QString::number(query->value(5).toInt());
        //if (typeManager == NULL) qDebug() << "typeManager NULL"; else qDebug() << "typeManager not NULL";
        Type * type = typeManager->findTypeById(query->value(5).toInt());
        Directory * parentDirectory = NULL;
        Directory * directory= new Directory(query->value(0).toInt(),query->value(1).toString(), query->value(2).toDateTime(), query->value(3).toInt(),query->value(7).toString(),query->value(4).toInt(),type, parentDirectory);
        dirsList << *directory;
    }
    return dirsList;
}

bool DirectoryManager::updateDirectoryLocally(QFileInfo * fileInfo)
{
    Directory * directory = convertToDirectory(fileInfo);

    return updateDirectoryLocally(directory);
}

bool DirectoryManager::updateDirectoryLocally(Directory *directory)
{
    //qDebug() << "Updating directory";
    //query->exec("UPDATE directory set name='"+directory->getName()+"',datecreation = '"+directory->getDatecreation().toString(Parameters::timeFormat())+"', size = '"+QString::number(directory->getSize())+"',status = '"+QString::number(directory->getStatus())+"', path = '"+directory->getPath()+"',idtype = '"+QString::number(directory->getType()->getId())+"' where id = '"+QString::number(directory->getId())+"'");
    query->exec("UPDATE directory set status = '"+QString::number(directory->getStatus())+"' where id = '"+QString::number(directory->getId())+"'");
    if (!query->isActive())
    {
        qDebug()<<"Error when updating directory : " + query->lastError().text();
        return false;
    }
    return true;
}

void DirectoryManager::showDatabaseDirectories()
{
    // then if there's a directory with these specs, we retrieve the values that we need
    query->exec("select id,name,datecreation,size,status,idtype,path,iddirectory from directory ");
    if (!query->isActive())
    {
        qDebug()<<"Error when selecting directory : " + query->lastError().text();
        return;
    }

    while (query->next())
    {

        Type * type = typeManager->findTypeById(query->value(5).toInt());
        //qDebug() << " --- query->value(7).toInt() : " + QString::number(query->value(7).toInt());
        Directory * parentDirectory = NULL;//= findDirectoryById(query->value(7).toInt());
        Directory * directory = new Directory(query->value(0).toInt(),query->value(1).toString(), query->value(2).toDateTime(), query->value(3).toInt(),query->value(6).toString(),query->value(4).toInt(),type, parentDirectory);
        //directory->toString();
    }
}

/*
void DirectoryManager::deleteDBIrrelevantDirs()
{
    QString request = "delete from directory where status = 0";
    query->exec(request);
    if (!query->isActive())
    {
        qDebug()<<"Error when deleting irrelevant directories";
    }
}
*/

void DirectoryManager::checkDirsIntegrity()
{

    QList<Directory> dirsList = findDirectoryByIdDirectory(0);

    // if (dirsList == NULL) return;

    for (int i = 0;i<dirsList.size();i++)
    {
        Directory directory = dirsList.at(i);
        Directory * parentdir = findDirectoryByPath(Parameters::getParentDirPath(directory.getPath()));
        directory.setDirectory(parentdir);
        updateDirectoryLocally(&directory);
    }

}

QList<Directory> DirectoryManager::findDirectoryByStatus(int status)
{
    QList<Directory> dirsList;
    // then if there's a directory with these specs, we retrieve the values that we need
    query->exec("select id,name,datecreation,size,status,idtype,iddirectory,path from directory where status = "+QString::number(status)+" ");
    if (!query->isActive())
    {
        qDebug()<<"Error when selecting directory : " + query->lastError().text();
    }

    while(query->next())
    {
        Type * type = typeManager->findTypeById(query->value(5).toInt());
        Directory * parentDirectory = NULL;
        Directory * directory= new Directory(query->value(0).toInt(),query->value(1).toString(), query->value(2).toDateTime(), query->value(3).toInt(),query->value(7).toString(),query->value(4).toInt(),type, parentDirectory);
        dirsList << *directory;
    }
    return dirsList;
}

void DirectoryManager::saveUpdatesToRemoteDB()
{
    if (busy) return;

    QList<Directory> dirsList = findDirectoryByStatus(0);

    if (dirsList.size() == 0) return;

    busy = true;
    dirstosave = &dirsList;

    saveUpdateToRemoteDB();
}

void DirectoryManager::saveUpdateToRemoteDB()
{
    currentdir = (Directory *) &(dirstosave->back());
    saveDirectory(currentdir);
}

void DirectoryManager::countDatabaseDirectories()
{
    // then if there's a directory with these specs, we retrieve the values that we need
    query->exec("select count(*) from directory ");
    if (!query->isActive())
    {
        qDebug()<<"Error when counting directory : " + query->lastError().text();
        return;
    }

    if (query->next())
    {
        qDebug() << "Number of Directories in the local database is : " + query->value(0).toString();
    }
}

void DirectoryManager::resetDatabaseDirectories()
{
    query->exec("update directory set status = 0");
    if(!query->isActive())
        qDebug()<<"Error when reseting the local database directories : " + query->lastError().text();
}

void DirectoryManager::handleEndofRequest(QNetworkReply * reply)
{
    if (reply->error() != QNetworkReply::NoError)
        deleteDirectoryLocally(currentdir);

    dirstosave->pop_back();

    if (dirstosave->size() == 0)
    {
        busy = false;
        emit remoteUpdatesDone(3);
        return;
    }

    saveUpdateToRemoteDB();
}

bool DirectoryManager::deleteDirectoryLocally(Directory *directory)
{
    QString request = "delete from directory where directory.path = '"+directory->getPath()+"'";
    //qDebug() << request;
    query->exec(request);
    if(!query->isActive())
    {
        qDebug()<<"Error when deleting directory : " + query->lastError().text();
        return false;
    }
    return true;
}

QList<Directory>  DirectoryManager::getAllDBDirs()
{
    QList<Directory> dirsList;
    // then if there's a directory with these specs, we retrieve the values that we need
    query->exec("select id,name,datecreation,size,status,idtype,iddirectory,path from directory ");
    if (!query->isActive())
    {
        qDebug()<<"Error when selecting directory : " + query->lastError().text();
    }

    while(query->next())
    {
        Type * type = typeManager->findTypeById(query->value(5).toInt());
        Directory * parentDirectory = NULL;
        Directory * directory= new Directory(query->value(0).toInt(),query->value(1).toString(), query->value(2).toDateTime(), query->value(3).toInt(),query->value(7).toString(),query->value(4).toInt(),type, parentDirectory);
        dirsList << *directory;
    }
    return dirsList;
}

Directory * DirectoryManager::findOtherDirectoryOnDisk()
{
    Directory * dir = NULL;
    // then if there's a directory with these specs, we retrieve the values that we need
    query->exec("select id,name,datecreation,size,status,idtype,iddirectory,path from directory where iddirectory = 0 and path <> '"+Parameters::storageDirectory()+"'");
    if (!query->isActive())
    {
        qDebug()<<"Error when selecting directory : " + query->lastError().text();
        return dir;
    }

    if (query->next())
    {
        Type * type = typeManager->findTypeById(query->value(5).toInt());
        Directory * parentDirectory = NULL;
        dir= new Directory(query->value(0).toInt(),query->value(1).toString(), query->value(2).toDateTime(), query->value(3).toInt(),query->value(7).toString(),query->value(4).toInt(),type, parentDirectory);
    }
    return dir;
}
