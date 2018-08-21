#include "Database/EntityManagers/FileManager.h"

using namespace cv;

/***            Default constructor         ***/
FileManager::FileManager() : AbstractManager()
{
    //this->fileTypeManager = new FileTypeManager();
    //this->categoryManager = new CategoryManager();
}

/**
 * @brief FileManager::saveFile
 * @return
 */
File* FileManager::add(QFileInfo rootFileInfo)
{
    // we apply the depth first search to add the subdirectories and subfiles of this folder
    // i start going through all files and directories to store and track them
    QFileInfoList * queue = new QFileInfoList();
    (*queue) += QDir(rootFileInfo.absoluteFilePath()).entryInfoList(QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot);

    if (queue->isEmpty()){
        qDebug() << "Empty queue ! " << endl;
        return add(convertToFile(rootFileInfo));
    }

    QFileInfo currentObject = (queue->last());
    queue->removeLast();
    while(true){
        //qDebug() << currentObject.absoluteFilePath() << endl;
        File * file = convertToFile(currentObject);
        add(file);

        if (currentObject.isDir())
            // get its children
            (*queue) += QDir(currentObject.absoluteFilePath()).entryInfoList(QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot);
        if (queue->isEmpty())
            break;
        currentObject = (queue->last());
        queue->removeLast();
    }
}



/**
 * @brief FileManager::saveFile
 * @param file
 * @return
 */
File* FileManager::add(File * file)
{
    qDebug() << "adding file" << endl;
    QString request = "insert into File(id, iddir, idtype, idcategory, drive_id, createdat, filename, path, size, status, thumbnail, length) select NULL, "
            +((file->getFolder() == NULL) ? "-1" : QString::number(file->getFolder()->getId()))+", "
            +QString::number(file->getType()->getId())+", "
            +((file->getCategory() == NULL) ? "NULL" : QString::number(file->getCategory()->getId()))+", "
            +QString::number(file->getDrive()->getId())+", '"
            +file->getCreatedAt().toString(Parameters::timeFormat)
            +"', '"+file->getFileName()+"', '"+file->getPath()
            +"', "+QString::number(file->getSize())+", "+QString::number(File::FILE_ADDED)
            +", "+ ((file->getThumbnail() == NULL) ? "NULL": "'"+file->getThumbnail()->fileName()+"'")
            +", "+QString::number(file->getLength())+" where not exists (select 1 from File where path = '"+file->getPath()+"' and filename = '"+file->getFileName()+"');";

    /*
    QString request = "insert or replace into File(id, iddir, idtype, idcategory, createdat, updatedat, filename, path, size, status, thumbnail) values(NULL, "
            +((file->getFolder() == NULL) ? "NULL" : QString::number(file->getFolder()->getId()))+", "
            +QString::number(file->getType()->getId())+", "
            +QString::number(file->getCategory()->getId())+", '"
            +file->getCreatedAt().toString(Parameters::timeFormat)
            +"', '"+file->getUpdatedAt().toString(Parameters::timeFormat)
            +"', '"+file->getFileName()+"', '"+file->getPath()
            +"', "+QString::number(file->getSize())+", "+QString::number(File::FILE_ADDED)
            +", "+ ((file->getThumbnail() == NULL) ? "NULL": "'"+Functions::getRelativePath(file->getThumbnail()->absoluteFilePath())+"'")
            +");";
            */

    qDebug() << request << endl;

    QSqlQuery sqlQuery(Parameters::localDb);
    sqlQuery.exec(request);
    if (!sqlQuery.isActive()){
        qDebug() << "Failed to save a file " << sqlQuery.lastError().text() << endl;
        return NULL;
    }

    // After inserting the file, update the property "iddir" of its children already saved
    //qDebug() << "frou " << file->getAbsolutePath() << endl;
    if (QFileInfo(file->getAbsolutePath()).isDir()){
        updateDbChildrenOf(file);
    }

    emit fileSaved(file);

    QMap<QString, QString> fileProperties;
    fileProperties.insert("filename", file->getFileName());
    fileProperties.insert("path", file->getPath());
    return getOneBy(fileProperties);
}

/**
 * @brief FileManager::updateFile
 * @param file
 * @return
 */
File * FileManager::update(File *file)
{
    QString request = "update File set size = "+QString::number(file->getSize())+", status = "+QString::number(File::FILE_UPDATED)+", length = "+QString::number(file->getLength())
            +" where id = "+QString::number(file->getId());
    qDebug() << "File updated in db! " << request << endl;
    QSqlQuery sqlQuery(Parameters::localDb);
    sqlQuery.exec(request);
    if (!sqlQuery.isActive()){
        qDebug() << "Failed to update a file " << sqlQuery.lastError().text() << endl;
        return NULL;
    }
    /*
    emit fileUpdated(file);
    QMap<QString, QString> fileProperties;
    fileProperties.insert("id", QString::number(file->getId()));
    return getOneBy(fileProperties);
    */
    return file;
}

/**
 * @brief FileManager::hardDelete
 * @param file
 * @return
 */
bool FileManager::hardDelete(File *file)
{
    QString request = "delete from File where id = "+QString::number(file->getId());
    qDebug() << "deleting : " << request << endl;
    QSqlQuery sqlQuery(Parameters::localDb);
    sqlQuery.exec(request);
    if (!sqlQuery.isActive()){
        qDebug() << "Failed to delete a file " << sqlQuery.lastError().text() << endl;
        return false;
    }
    //printAllFiles();
    qDebug() << "Succeed in deleting a file " << sqlQuery.lastError().text() << endl;
    return true;
}

/**
 * @brief FileManager::symDeleteFile consist in marking the file like to be deleted
 * @param file
 * @return
 */
bool FileManager::symDelete(File *file)
{
    QString request = "update File set status = "+QString::number(File::FILE_DELETED)+" where id = "+QString::number(file->getId());
    qDebug() << "deleting : " << request << endl;
    QSqlQuery sqlQuery(Parameters::localDb);
    sqlQuery.exec(request);
    if (!sqlQuery.isActive()){
        qDebug() << "Failed to delete a file " << sqlQuery.lastError().text() << endl;
        return false;
    }
    //printAllFiles();
    qDebug() << "Succeed in deleting a file " << sqlQuery.lastError().text() << endl;
    emit fileDeleted(file);
    return true;
}

bool FileManager::updateDirContent(QDir dir)
{
    QFileInfoList children_dirs = dir.entryInfoList(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);
    int size(children_dirs.size());
    for (int i(0); i < size ; i ++){
        QFileInfo child = children_dirs.at(i);
        File * childFile = convertToFile(child);
        // i get all the recently updated or created files
        QDateTime createdAt = child.created(),
                currentDateTime = QDateTime::currentDateTime();
        if (createdAt.date() == currentDateTime.date() and createdAt.time().hour() == currentDateTime.time().hour() and (currentDateTime.time().minute() - createdAt.time().minute()) < Parameters::MIN_CREAT_DELAY){
            if (child.isFile())
                add(childFile);
            else
                add(child);
            qDebug() << "Updating Dir'content !" << endl;
        }
    }
    return true;
}

QList<File*>* FileManager::getAll()
{
    QList<File*> *files = new QList<File *>();
    QString request = "SELECT * FROM File";
    QSqlQuery sqlQuery(Parameters::localDb);
    sqlQuery.exec(request);
    if (!sqlQuery.isActive()){
        qDebug()<<"Error when selecting all files : " + sqlQuery.lastError().text();
        return NULL;
    }

    while (sqlQuery.next()){
        //qDebug() << "fou" << endl;
        File * file = Functions::fromSqlRecord2File(sqlQuery.record());
        files->append(file);
    }
    return files;
}

/**
 * @brief FileManager::getNotSavedFiles
 * @return
 */
QList<File*> * FileManager::getNotSavedFiles()
{
    QList<File*> *files = new QList<File *>();

    QString request("SELECT * from File where status <> "+QString::number(File::FILE_SAVED));
    QSqlQuery sqlQuery(Parameters::localDb);
    sqlQuery.exec(request);
    if (!sqlQuery.isActive()){
        qDebug()<<"Error when selecting not saved files : " + sqlQuery.lastError().text();
        return NULL;
    }


    while (sqlQuery.next()){
        File * file = Functions::fromSqlRecord2File(sqlQuery.record());
        files->append(file);
    }
    return files;
}

bool FileManager::setFileSaved(int fileId)
{
    QString request = "update File set status = "+QString::number(File::FILE_SAVED)+" where id = "+QString::number(fileId);
    qDebug() << request << endl;
    QSqlQuery sqlQuery(Parameters::localDb);
    sqlQuery.exec(request);
    if(!sqlQuery.isActive())
    {
        qDebug()<<"Error when changing file status : " + sqlQuery.lastError().text();
        return false;
    }
    return true;
}

Drive *FileManager::getDrive()
{

}

int FileManager::getSizeOnDb(File * file)
{
    QMap<QString, QString> childrenProperties;
    childrenProperties.insert("iddir", ((file->getFileName() == Parameters::ROOT_DIR_NAME) ? "-1" : QString::number(file->getId())));
    QList<File *> * childrenFiles = getBy(childrenProperties);

    int size = childrenFiles->size(), result(0);
    //printAll();
    for (int i(0); i < size; i++){
        File * childFile = childrenFiles->at(i);
        result += childFile->getSize();
    }
    qDebug() << "Result : " << file->getFileName() << size << result << endl;
    return result;
}

int FileManager::getSizeOnDb(QFileInfo fileInfo)
{
    qDebug() << "Zoo" << fileInfo.fileName() << Parameters::ROOT_DIR_NAME << endl;
    File * file = NULL;
    if (fileInfo.fileName() == Parameters::ROOT_DIR_NAME){
        file = new File();
        file->setId(NULL);
        file->setPath(Functions::getRelativePath(fileInfo.absolutePath()));
        file->setFileName(fileInfo.fileName());
    }
    else{
        QMap<QString, QString> fileProperties;
        fileProperties.insert("path", Functions::getRelativePath(fileInfo.absolutePath()));
        fileProperties.insert("filename", fileInfo.fileName());
        file = getOneBy(fileProperties);
    }

    qDebug() << "FileInfo -> file : " << fileInfo.fileName() << endl;
    if (file == NULL)
        return 0;
    return getSizeOnDb(file);
}

//int FileManager::getSizeOf(QFileInfo fileInfo)
//{
//    QMap<QString, QString> fileProperties;
//    QString path, filename;
//    QStringList list = fileInfo.absoluteFilePath().split("/");
//    list = list.mid(list.indexOf(Parameters::ROOT_DIR_NAME));
//    filename = list.last();
//    list.removeLast();
//    path = "/"+list.join("/");
//    if (path == "/" && filename == Parameters::ROOT_DIR_NAME){ // if it's the root dir that's been given

//    }

//    fileProperties.insert("path", path);
//    fileProperties.insert("filename", filename);
//    File * file = FileManager::getOneBy(fileProperties);
//    qDebug() << "in getSizeOf() " << ((file == NULL) ? "NULL" : file->getFileName()) << endl;
//    if (file != NULL)
//        if (fileInfo.isDir())
//            return getSizeOnDb(file);
//        else
//            return file->getSize();
//    return getSizeOnFS(fileInfo);
//}

int FileManager::getSizePercentage(QFileInfo fileInfo, QDir dir)
{
    // i check first that file fileInfo1 is under the fileInfo dir
    int num = getSizeOnDb(fileInfo);
    //if (num == 0)
    //    return 0;
    int percentage = (int) (num*100/getSizeOnDb(QFileInfo(dir.absolutePath())));
    //qDebug() << "Testoo" << percentage << endl;
    return percentage;
}

/**
 * @brief FileManager::convertToFile
 * @param fileInfo
 * @return
 */
File *FileManager::convertToFile(QFileInfo fileInfo)
{
    QMap<QString, QString> fileProperties;
    fileProperties.insert("path", Functions::getRelativePath(fileInfo.absolutePath()));
    fileProperties.insert("filename", fileInfo.fileName());
    File * savedFile = getOneBy(fileProperties);

    File * file = NULL;
    if (savedFile != NULL){
        file = savedFile;
        if (fileInfo.isDir())
            file->setSize(getSizeOnDb(file));
        else
            file->setSize(fileInfo.size());
        file->setStatus(File::FILE_UPDATED); // TODO temporary value
    }
    else{
        FileType * fileType = FileTypeManager::getType(fileInfo);
        if (fileType == NULL)
            qDebug() << "File type NULL" << endl;
        Category * fileCategory = CategoryManager::getCategory(fileInfo);
        if (fileCategory == NULL)
            qDebug() << "Category NULL " << endl;
        Drive * drive = DriveManager::getDrive(fileInfo);
        if (drive == NULL)
            qDebug() << "Drive NULL" <<endl;

        qDebug() << "Converting file" << fileInfo.fileName() <<  endl;
        int size(0);
        if (fileInfo.isDir()){
            size = 0;
        }
        else{
            size = fileInfo.size();
        }

        // retrive file's folder
        QFileInfo parentFileInfo(fileInfo.absolutePath());
        fileProperties.insert("path", Functions::getRelativePath(parentFileInfo.absolutePath()));
        fileProperties.insert("filename", parentFileInfo.fileName());
        File * folder = getOneBy(fileProperties);

        file = new File(0, fileInfo.fileName(), Functions::getRelativePath(fileInfo.absolutePath()), fileInfo.created(), size, File::FILE_ADDED, ((fileInfo.isDir()) ? NULL : Functions::generateThumbnails(fileInfo)), Functions::getLength(fileInfo), fileType, fileCategory, folder, drive);
    }
    return file;
}

File *FileManager::getOneBy(QMap<QString, QString> properties)
{
    //Building the sql request
    QString request = "select * from File where ";

    QMapIterator<QString, QString> i(properties);
    int j = 0;
    while(i.hasNext()){
        i.next();
        request += ((j != 0) ? " and " : " ") + i.key()+" = '"+i.value()+"'";
        j++;
    }

    //qDebug() << request << endl;
    QSqlQuery sqlQuery(Parameters::localDb);
    sqlQuery.exec(request);
    if (!sqlQuery.isActive()){
        qDebug() << "Failed to select the file : " << sqlQuery.lastError().text() <<endl;
        return NULL;
    }

    qDebug() << "request is : " << request << endl;
    if (sqlQuery.next()){
        return Functions::fromSqlRecord2File(sqlQuery.record());
    }
    return NULL;
}

QList<File *> *FileManager::getBy(QMap<QString, QString> properties)
{
    //Building the sql request
    QString request = "select * from File where ";

    QMapIterator<QString, QString> i(properties);
    int j = 0;
    while(i.hasNext()){
        i.next();
        request += ((j != 0) ? " and " : " ") + i.key()+" = '"+i.value()+"'";
        j++;
    }

    //qDebug() << request << endl;
    QSqlQuery sqlQuery(Parameters::localDb);
    sqlQuery.exec(request);
    if (!sqlQuery.isActive()){
        qDebug() << "Failed to select the file : " << sqlQuery.lastError().text() <<endl;
        return NULL;
    }

    QList<File * > * files = new QList<File *>();
    while (sqlQuery.next()){
        files->append(Functions::fromSqlRecord2File(sqlQuery.record()));
    }
    //qDebug() << "request is : " << request << endl;
    return files;
}

//void FileManager::setFileTypeManager(FileTypeManager * fileTypeManager)
//{
//    this->fileTypeManager = fileTypeManager;
//}

//void FileManager::setCategoryManager(CategoryManager * categoryManager)
//{
//    this->categoryManager = categoryManager;
//}

/**
 * @brief FileManager::printAllFiles
 */
void FileManager::printAll()
{
    QList<File *> * files = getAll();
    int size = files->size();
    for (int i(0); i < size; i++){
        qDebug() << files->at(i)->toString() << endl;
    }
}

void FileManager::deleteAll()
{
    QString request = "delete from "+Parameters::DB_FILE;
    QSqlQuery sqlQuery(Parameters::localDb);
    sqlQuery.exec(request);
    if (!sqlQuery.isActive()){
        qDebug()<<"Error when deleting all files : " + sqlQuery.lastError().text();
        return;
    }
    qDebug() << "Succeed in deleting all files " << endl;
    qDebug() << "The remaining files are " << getAll()->size() << endl;
}

bool FileManager::updateDbChildrenOf(File * file)
{
    qDebug() << "Updating children " << endl;
    QMap<QString, QString> fileProperties;
    fileProperties.insert("path", (file->getPath()+"/"+file->getFileName()));

    QList<File * >* childrenFiles = getBy(fileProperties);
    int size = childrenFiles->size();
    qDebug() << file->getPath()+"/"+file->getFileName() << size << endl;
    bool ok = true;
    for(int i(0); i < size; i++){
        File * childFile = childrenFiles->at(i);
        childFile->setFolder(file);
        if (update(childFile) == NULL){
            ok = false;
        }
    }
    return ok;
}

int FileManager::getSizeOnFS(QFileInfo fileInfo)
{
    int dirSize = 0;
    QFileInfoList queue;
    QFileInfo currentNode = fileInfo ;
    while(true){
        // if the root directory is a file then
        if (currentNode.isFile()){
            dirSize += currentNode.size();
        }
        else{
            //otherwise
            queue += QDir(currentNode.absoluteFilePath()).entryInfoList(QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot);
            //QMessageBox::information(0, "ok", QString::number(queue.size()));
        }
        if (queue.isEmpty())
            return dirSize;
        currentNode = queue.last();
        queue.removeLast();
    }
}



