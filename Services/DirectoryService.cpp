#include "DirectoryService.h"

/***    Folder's default constructor    ***/
/**
 * @brief DirectoryService::DirectoryService
 */
DirectoryService::DirectoryService()
{
    fsWatchers = new QList<QFileSystemWatcher*>();
}

/**
 * @brief DirectoryService::start
 */
void DirectoryService::start()
{
    watchZeminiFolder();
}

/**
 * @brief DirectoryService::watchZeminiFolder
 */
void DirectoryService::watchZeminiFolder()
{
    // i start going through all files and directories to store and track them
    QDir root_dir(Parameters::STORE_DIR);
    QFileInfoList * queue = new QFileInfoList();
    (*queue) += root_dir.entryInfoList(QDir::Files | QDir::NoSymLinks | QDir::AllDirs | QDir::NoDotAndDotDot);

    if (queue->size() == 0){
        QMessageBox::critical(0, "Zemini error", "Some folders are currently missing in the workplace, preventing the app from working correctly. Please! Try to reinstall Zemini!",QMessageBox::Ok);
        return;
    }

    QFileInfo currentObject = (queue->last());
    queue->removeLast();
    fsWatchers->append(new QFileSystemWatcher());
    connect(fsWatchers->last(), SIGNAL(directoryChanged(QString)), this, SLOT(handleDirChanges(QString)));
    connect(fsWatchers->last(), SIGNAL(fileChanged(QString)), this, SLOT(handleFileChanges(QString)));
    while(true){
        //qDebug() << currentObject.absoluteFilePath() << endl;
        emit storeInDb(currentObject);
        if (currentObject.isDir()){
            if (currentObject.absoluteFilePath() != Parameters::THUMBS_DIR){
                // get its children
                (*queue) += QDir(currentObject.absoluteFilePath()).entryInfoList(QDir::Files | QDir::NoSymLinks | QDir::AllDirs | QDir::NoDotAndDotDot);
            }
        }
        if (queue->isEmpty())
            break;
        currentObject = (queue->last());
        queue->removeLast();
    }
}

/**
 * @brief DirectoryService::watchFile
 * @param fileInfo
 */
void DirectoryService::watchFile(QFileInfo fileInfo)
{
    qDebug() << "watching "+fileInfo.fileName() << endl;
    bool added = fsWatchers->last()->addPath(fileInfo.absoluteFilePath());
    if (!added){
        fsWatchers->append(new QFileSystemWatcher());
        connect(fsWatchers->last(), SIGNAL(directoryChanged(QString)), this, SLOT(handleDirChanges(QString)));
        connect(fsWatchers->last(), SIGNAL(fileChanged(QString)), this, SLOT(handleFileChanges(QString)));
        fsWatchers->last()->addPath(fileInfo.absoluteFilePath());
    }
    qDebug() << "Added successfully" << endl;
}

// building all the structure of the user storage directory
// this function just buildind folders corresponding to categories for now
// it's gonna be extend in the future to handle other kind of data
/**
 * @brief DirectoryService::makeInitDirectories
 * @param categories
 * @return
 */
bool DirectoryService::makeInitDirectories(QList<Category> * categories)
{
    if (categories == NULL)
        return false;

    // making the first root directory under which the others will be put
    QString rootDir = Parameters::STORE_DIR;
    rootDir.replace("\\","/");
    QDir zeminiDirectory(rootDir);
    if (!zeminiDirectory.exists())
    {
        if (!zeminiDirectory.mkdir("."))
            qDebug()<<"Error creating the main folder"<<endl;
    }

    for (int i = 0; i < categories->size(); i++){
        Category cat = (categories->at(i));

        QString categoryDirPath = rootDir+"/"+cat.getName();
        categoryDirPath.replace("\\","/");
        QDir categoryDir(categoryDirPath);
        if (!categoryDir.exists())
        {
            if (!categoryDir.mkdir("."))
                qDebug()<<"Error when creating the " << cat.getName() <<" folder"<<endl;
        }

    }

    // then making the dir of the thumbnails
    QString keysDirPath = Parameters::KEYS_DIR;
    QDir keysDir(keysDirPath.replace("\\", "/"));
    if (!keysDir.exists()){
        if (!keysDir.mkdir("."))
            qDebug()<<"Error when creating the " << Parameters::KEYS_DIR <<" folder"<<endl;
    }

    // finally make a link to the zemini folder
    Functions::makeLinkToZeminiFolder();

    return true;
}

/**
 * @brief DirectoryService::handleDirChanges
 * @param dirPath
 */
void DirectoryService::handleDirChanges(QString dirPath)
{
    //updated = true;
    QFileInfo fileInfo(dirPath);
    if (fileInfo.exists()){
        qDebug() << "Dir changed " << endl;
        emit dirUpdated(fileInfo);
    }
    else{
        qDebug() << "Dir deleted " << endl;
        // removing the watch over this folder
        removeWatchOver(fileInfo);
        emit dirDeleted(fileInfo);
    }
}

/**
 * @brief DirectoryService::removeWatchOver
 * @param fileInfo
 * @return
 */
bool DirectoryService::removeWatchOver(QFileInfo fileInfo)
{
    int size = fsWatchers->size();
    for (int i(0); i < size; i++){
        if (fsWatchers->at(i)->removePath(fileInfo.absoluteFilePath()))
            return true;
    }
    return false;
}

/**
 * @brief DirectoryService::handleFileChanges
 * @param filePath
 */
void DirectoryService::handleFileChanges(QString filePath)
{
    //updated = true;
    QFileInfo fileInfo(filePath);
    if (fileInfo.exists()){
        qDebug() << "file updated " << endl;
        emit fileUpdated(fileInfo);
    }
    else{
        qDebug() << "file deleted " << endl;
        emit fileDeleted(fileInfo);
    }
}
