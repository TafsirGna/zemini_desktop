#include "DirectoryService.h"

/***    Folder's default constructor    ***/
/**
 * @brief DirectoryService::DirectoryService
 */
DirectoryService::DirectoryService()
{
    fsWatchers = new QList<QFileSystemWatcher*>();
    active = false;
    stopSaving = false;
    pauseSaving = false;
}

/**
 * @brief DirectoryService::start
 */
void DirectoryService::start()
{
    active = true;
    watchZeminiFolder();
}

/**
 * @brief DirectoryService::watchZeminiFolder
 */
void DirectoryService::watchZeminiFolder()
{
    //first, i check that all directories corresponding to the categories exist
    FileManager::deleteAll();

    QList<Drive*> * drives = DriveManager::getAll();
    // i start going through all files and directories to store and track them
    int nbDrives(drives->size());

    for (int iter(0); iter < nbDrives; iter++){

        Drive * drive = drives->at(iter);
        checkRootDirIntegrity(drive);

        QDir root_dir(drive->getAbsolutepath()+"/"+Parameters::ROOT_DIR_NAME);
        //qDebug() << "Drive path : " << drive->getAbsolutepath() <<endl;
        QDir thumbsDir(root_dir.absolutePath()+"/"+Parameters::THUMBS_DIR_NAME);
        // starting the static file service that wil serve requested files
        StaticFileService * fileServer = new StaticFileService(root_dir.absolutePath(), NULL);

        queue = new QFileInfoList();
        (*queue) += root_dir.entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot);

        QFileInfo currentObject = (queue->last());
        queue->removeLast();
        fsWatchers->append(new QFileSystemWatcher());
        connect(fsWatchers->last(), SIGNAL(directoryChanged(QString)), this, SLOT(handleDirChanges(QString)));
        //connect(fsWatchers->last(), SIGNAL(fileChanged(QString)), this, SLOT(handleFileChanges(QString)));

        emit startWatchingRootDir();

        while(true){

            if (pauseSaving){
                break;
            }

            if (stopSaving){
                queue->clear();
                break;
            }
            qDebug() << "test : " << currentObject.absoluteFilePath() << endl;
            if (currentObject.absoluteFilePath() != thumbsDir.absolutePath())
                emit storeInDb(currentObject);

            if (currentObject.isDir() && (currentObject.absoluteFilePath() != thumbsDir.absolutePath())){
                // get its children
                (*queue) += QDir(currentObject.absoluteFilePath()).entryInfoList(QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot);
            }
            if (queue->isEmpty())
                break;

            // i set the size of the parent folder
            if (currentObject.absolutePath() != queue->last().absolutePath()){
                QFileInfo parentFileInfo = QFileInfo(currentObject.absolutePath());
                if (parentFileInfo.fileName() != Parameters::ROOT_DIR_NAME){
                    QMap<QString, QString> fileProperties;
                    fileProperties.insert("path", Functions::getRelativePath(parentFileInfo.absolutePath()));
                    fileProperties.insert("filename", parentFileInfo.fileName());
                    File * file = FileManager::getOneBy(fileProperties);
                    file->setSize(FileManager::getSizeOnDb(file));
                    file = FileManager::update(file);
                }
            }
            currentObject = (queue->last());
            queue->removeLast();
        }
    }
    qDebug() << "Folder on watching " << endl;
    emit rootFolderOnWatching();
}

/**
 * @brief DirectoryService::watchFile
 * @param fileInfo
 */
void DirectoryService::watchFile(File *file)
{
    //qDebug() << "watching "+file->getFileName() << endl;
    bool added = fsWatchers->last()->addPath(file->getAbsolutePath());
    if (!added){
        fsWatchers->append(new QFileSystemWatcher());
        connect(fsWatchers->last(), SIGNAL(directoryChanged(QString)), this, SLOT(handleDirChanges(QString)));
        //connect(fsWatchers->last(), SIGNAL(fileChanged(QString)), this, SLOT(handleFileChanges(QString)));
        fsWatchers->last()->addPath(file->getAbsolutePath());
    }
    qDebug() << "Added successfully" << endl;
}

void DirectoryService::stop()
{
    int list_size = fsWatchers->size();
    for (int i(0); i < list_size; i++){
        QFileSystemWatcher * fsWatcher = fsWatchers->at(i);
        delete(fsWatcher);
    }

    active = false;
}

// building all the structure of the user storage directory
// this function just buildind folders corresponding to categories for now
// it's gonna be extend in the future to handle other kind of data
/**
 * @brief DirectoryService::makeInitDirectories
 * @param categories
 * @return
 */
bool DirectoryService::initFolder(QDir rootDir, QStringList dirNames)
{
    if (dirNames.isEmpty())
        return false;

    QDir zeminiDir(rootDir.absolutePath()+"/"+Parameters::ROOT_DIR_NAME);
    //qDebug() << "Diro " << zeminiDir.absolutePath() << zeminiDir.exists() < endl;
    if (!zeminiDir.exists() && !zeminiDir.mkdir(".")){
        qDebug() << "Failed to create zemini folder" << endl;
        QMessageBox::critical(0, "Zemini Folder", "An error occured when making the main folder. Check the reason and restart please.", QMessageBox::Yes);
        return false;
    }

    int list_size = dirNames.size();
    for (int i = 0; i < list_size; i++){
        QString subDirPath = zeminiDir.absolutePath()+"/"+dirNames.at(i);
        QDir subDir(subDirPath);
        if (!subDir.exists() && !subDir.mkdir("."))
            qDebug()<<"Error when creating the " << dirNames.at(i) <<" folder"<<endl;
    }

    // finally make a link to the zemini folder
    Functions::makeLinkToRootFolder(zeminiDir);
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

bool DirectoryService::checkRootDirIntegrity(Drive * drive)
{
    // first, i check if the zemini dire is still there, if no then i create a new one corresponding to the previous one
    QDir rootDir(drive->getAbsolutepath());
    if (!rootDir.exists()){
        qDebug() << "The zemini root directory doesn't exist! " << endl;
        try {
            if (rootDir.mkdir(".")){
                QMessageBox::warning(0, "Zemini Info", "Zemini is unable to access the drive "+drive->getAbsolutepath(), QMessageBox::Yes);
                return false;
            }
        } catch (...) {
        }
    }
    else
        return false;

    // checking that all directories corresponding to each category exists otherwise create them
    QList<Category> * categories = CategoryManager::getAll();
    int size(categories->size());
    for (int i(0); i < size; i++){
        Category category = categories->at(i);
        QDir categoryDir(rootDir.absolutePath()+"/"+category.getName());
        if (!categoryDir.exists()){
            qDebug() << "Directory : "+category.getName()+" didn't exist !" << endl;
            try {
                categoryDir.mkdir(".");
            } catch (...) {
            }
        }
    }
}

/**
 * @brief DirectoryService::handleFileChanges
 * @param filePath
 */
/*
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
*/

bool DirectoryService::setUserFolder(QWidget* parent)
{
    bool go_On;
    QString selectedDirPath;

    do {
        selectedDirPath = QFileDialog::getExistingDirectory(parent, tr("Open directory"), QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        qDebug() << "Select folder : " << selectedDirPath << endl;
        go_On = ((selectedDirPath == "") ? true : false);
        if (go_On){
            int response = QMessageBox::information(parent, "Zemini Info", "It seems you have not choosen a directory. Do you want to try again?", QMessageBox::Yes, QMessageBox::No);
            go_On = ((response == QMessageBox::Yes) ? true : false);
        }
    } while (go_On);

    if (!go_On && selectedDirPath == ""){
        parent->close();
        return false;
    }

    // after entering the select folder to serve as the zemini folder, he user is asked to entrer where is the folder
    DriveTypeDialog *dialog = new DriveTypeDialog();
    QString driveTypeName;
    if (dialog->exec() == QDialog::Accepted){
        driveTypeName = dialog->getSelectedSupport();
    }

    // making the directories following the categories
    QStringList subDirNames = LocalDBService::getSubDirNames();
    if (initFolder(QDir(selectedDirPath), subDirNames)){

        // Creating a drive object corresponding to the device on which the software is being installed
        QMap<QString, QString> properties;
        properties.insert("Name", driveTypeName);
        DriveType * computerType = DriveTypeManager::getOneBy(properties);

        if (computerType == NULL)
            computerType = DriveTypeManager::add(new DriveType(0, driveTypeName));

        if (computerType != NULL){
            DriveManager::add(new Drive(0, selectedDirPath, computerType, true));
        }
        qDebug() << "root folder inserted" << endl;
        return true;
    }
    return false;
}

bool DirectoryService::isActive()
{
    return active;
}

/*
void DirectoryService::setTrayIcon(QSystemTrayIcon * sysTrayIcon)
{
    this->trayIcon = sysTrayIcon;
}
*/

void DirectoryService::onProcessPaused()
{
    pauseSaving = true;
}

void DirectoryService::onProcessCancelled()
{
    stopSaving = true;
}

void DirectoryService::onProcessResumed()
{
    pauseSaving = false;
}

void DirectoryService::onProcessRestarted()
{
    stopSaving = false;
}

void DirectoryService::onThumbBackedUp(File * file)
{
    if (file->getThumbnail() != NULL){
        QFile qFile(file->getThumbnail()->absoluteFilePath());
        try {
            if (qFile.remove()){
                qDebug() << "Failed to remove the thumbnail of file " << file->getAbsolutePath() << endl;
            }
        } catch (...) {
        }
    }
}
