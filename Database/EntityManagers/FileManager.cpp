#include "Database\EntityManagers\FileManager.h"
#include "Services\DirectoryService.h"

/***            Default constructor         ***/
FileManager::FileManager() : AbstractManager()
{
    this->typeManager = new TypeManager();
    this->directoryManager = new DirectoryManager();
    //this->filestosave = new QList<File>();
}

FileManager::FileManager(TypeManager * typeManager, DirectoryManager * directoryManager): AbstractManager()
{
    this->typeManager = typeManager;
    this->directoryManager = directoryManager;
    //this->filestosave = new QList<File>();
}

/***            This function creates or if you want, saves a given instance of contenu into the database          ***/
void FileManager::saveFile(File * file)
{
    /*QUrl params(str_Url+"?action=1"+    //action (1) shows we want to save updates
                "&filename=" + file->getName() +
                "&suffix=" + file->getSuffix() +
                "&size="+ QString::number(file->getSize())+
                "&datecreation="+ file->getDatecreation().toString(Parameters::timeFormat())
                );
    */
    //request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    //this->accessManager->get(QNetworkRequest(params));

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

/*
File * FileManager::findFileByPath(QString path)
{
    // then if there's a directory with these specs, we retrieve the values that we need
    query->exec("select id,filename, datecreation, size, status, idtype, suffix, iddirectory from file where file.path = '"+path+"' ");
    if (!query->isActive())
    {
        qDebug()<<"Error when selecting a file : " + query->lastError().text();
        return NULL;
    }

    if (query->next())
    {
        //qDebug() << query->value(5).toInt();
        Type * type = typeManager->findTypeById(query->value(5).toInt());
        Directory * directory = directoryManager->findDirectoryById(query->value(7).toInt());
        return new File(query->value(0).toInt(),query->value(1).toString(), query->value(2).toDateTime(), query->value(3).toInt(),path,query->value(4).toInt(),type, query->value(6).toString(), directory);
    }
    else
        return NULL;
}

void FileManager::showDatabaseFiles()
{
    // then if there's a directory with these specs, we retrieve the values that we need
    query->exec("select id,filename, datecreation, size, status, idtype, suffix, iddirectory,path from file");
    if (!query->isActive())
    {
        qDebug()<<"Error when selecting a file : " + query->lastError().text();
    }
    //qDebug() << "yes";
    while (query->next())
    {

        Type * type = typeManager->findTypeById(query->value(5).toInt());
        Directory * directory = directoryManager->findDirectoryById(query->value(7).toInt());
        File * file = new File(query->value(0).toInt(),query->value(1).toString(), query->value(2).toDateTime(), query->value(3).toInt(),query->value(8).toString(),query->value(4).toInt(),type, query->value(6).toString(), directory);
        file->toString();
    }
}

void FileManager::handleEndofRequest(QNetworkReply* reply)
{
    if (reply->error() != QNetworkReply::NoError)
        deleteFileLocally(currentfile);

    filestosave->pop_back();

    if (filestosave->size() == 0)
    {

        busy = false;
        emit remoteUpdatesDone(4);
        return;
    }
    saveUpdateToRemoteDB();

}


void FileManager::deleteDBIrrelevantFiles()
{
    QString request = "delete from file where status = 0";
    query->exec(request);
    if (!query->isActive())
    {
        qDebug()<<"Error when deleting irrelevant files";
    }
}


void FileManager::checkFilesIntegrity()
{

    QList<File> filesList = findFileByIdDirectory(0);
    //if (filesList == NULL) return;
    for (int i=0;i<filesList.size();i++)
    {
        File file = filesList.at(i);
        Directory * directory = directoryManager->findDirectoryByPath(Parameters::getParentDirPath(file.getPath()));
        file.setDirectory(directory);
        updateFileLocally(&file);
    }

}


QList<File> FileManager::findFileByIdDirectory(int iddirectory)
{
    QList<File> filesList;
    // then if there's a directory with these specs, we retrieve the values that we need
    query->exec("select id,filename, datecreation, size, status, idtype, suffix, iddirectory,path from file where iddirectory = "+QString::number(iddirectory)+" ");
    if (!query->isActive())
    {
        qDebug()<<"Error when selecting a file : " + query->lastError().text();
        return filesList;
    }

    while(query->next())
    {
        Type * type = typeManager->findTypeById(query->value(5).toInt());
        Directory * directory = directoryManager->findDirectoryById(query->value(7).toInt());
        filesList << *(new File(query->value(0).toInt(),query->value(1).toString(), query->value(2).toDateTime(), query->value(3).toInt(),query->value(8).toString(),query->value(4).toInt(),type, query->value(6).toString(), directory));
    }
    return filesList;
}

QList<File> FileManager::findFileByStatus(int status)
{
    QList<File> filesList;
    // then if there's a directory with these specs, we retrieve the values that we need
    query->exec("select id,filename, datecreation, size, status, idtype, suffix, iddirectory,path from file where status = "+QString::number(status)+" ");
    if (!query->isActive())
    {
        qDebug()<<"Error when selecting a file : " + query->lastError().text();
        //return NULL;
    }

    while(query->next())
    {
        Type * type = typeManager->findTypeById(query->value(5).toInt());
        Directory * directory = directoryManager->findDirectoryById(query->value(7).toInt());
        filesList << *(new File(query->value(0).toInt(),query->value(1).toString(), query->value(2).toDateTime(), query->value(3).toInt(),query->value(8).toString(),query->value(4).toInt(),type, query->value(6).toString(), directory));
    }
    return filesList;
}

void FileManager::saveUpdatesToRemoteDB()
{
    if (busy) return;

    QList<File> filesList = findFileByStatus(0);

    //if (filesList == NULL) return ;
    if (filesList.size() == 0) return;


    this->busy = true;

    filestosave = &filesList;

    saveUpdateToRemoteDB();

}

void FileManager::saveUpdateToRemoteDB()
{
    //currentfile = (File *) &filestosave->at(0);
    currentfile = (File *) &(filestosave->back());
    saveFile(currentfile);
}

void FileManager::resetDatabaseFiles()
{
    query->exec("update file set status = 0");
    if(!query->isActive())
        qDebug()<<"Error when reseting the local database files : " + query->lastError().text();
}

bool FileManager::deleteFileLocally(File * file)
{
    QString request = "delete from file where file.path = '"+file->getPath()+"'";
    //qDebug() << request;
    query->exec(request);
    if(!query->isActive())
    {
        qDebug()<<"Error when deleting file : " + query->lastError().text();
        return false;
    }
    return true;
}
*/

