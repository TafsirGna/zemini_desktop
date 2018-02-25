#include "DirectoryService.h"

/***    Folder's default constructor    ***/
DirectoryService::DirectoryService()
{
    fsWatchers = new QList<QFileSystemWatcher*>();
}

void DirectoryService::start()
{
    watchZeminiFolder();
}

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
    //fsWatchers->append(new QFileSystemWatcher());
    //connect(fsWatchers->last(), SIGNAL(directoryChanged(QString)), this, SLOT(handleDirChanges(QString)));
    while(true){
        qDebug() << currentObject.absoluteFilePath() << endl;
        if (currentObject.isFile()){
            storeInDb(currentObject);
        }
        else{
            if (currentObject.fileName() != Parameters::THUMBS_DIR){
                // store the directory
                storeInDb(currentObject);
                // add to a fsWatcher
                //watchFolder(currentObject);
                // get its children
                QDir currentDirectory(currentObject.absoluteFilePath());
                (*queue) += currentDirectory.entryInfoList(QDir::Files | QDir::NoSymLinks | QDir::AllDirs | QDir::NoDotAndDotDot);
            }
        }
        if (queue->isEmpty())
            break;
        currentObject = (queue->last());
        queue->removeLast();
    }
}

void DirectoryService::watchFolder(QFileInfo fileInfo)
{
    bool added = fsWatchers->last()->addPath(fileInfo.absoluteFilePath());
    if (!added){
        fsWatchers->append(new QFileSystemWatcher());
        connect(fsWatchers->last(), SIGNAL(directoryChanged(QString)), this, SLOT(handleDirChanges(QString)));
        fsWatchers->last()->addPath(fileInfo.absoluteFilePath());
    }
}

// building all the structure of the user storage directory
// this function just buildind folders corresponding to categories for now
// it's gonna be extend in the future to handle other kind of data
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
    QString thumbsDirPath = Parameters::THUMBS_DIR;
    QDir thumbsDir(thumbsDirPath.replace("\\", "/"));
    if (!thumbsDir.exists()){
        if (!thumbsDir.mkdir("."))
            qDebug()<<"Error when creating the " << Parameters::THUMBS_DIR <<" folder"<<endl;
    }

    // finally make a link to the zemini folder
    Functions::makeLinkToZeminiFolder();

    return true;
}

void DirectoryService::storeInDb(QFileInfo fileInfo)
{
    emit fileInfoToSave(fileInfo);
}

void DirectoryService::handleDirChanges(QString dirPath)
{
    //updated = true;
    qDebug() << "Dir changed " << endl;
    QDir dir(dirPath);
    if (dir.exists())
        emit dirContentToUpdate(dir);
    else
        emit dirDeleted(dir);
}

/*
 *
 * //while (!stopped)
    //{

    // On sÃ©lectionne le rÃ©pertoire Ã  partir duquel on va rechercher les fichiers AVI et MP3
    QString selectDir = path;

    // On remplit une QStringList avec chacun des filtres dÃ©sirÃ©s ici "*.mp3" et "*.avi".
    QStringList listFilter;
    //listFilter << "*.avi";
    //listFilter << "*.mp3";

    // Variable qui contiendra tous les fichiers correspondant Ã  notre recherche
    QFileInfoList fileList;

    // On dÃ©clare un QDirIterator dans lequel on indique que l'on souhaite parcourir un rÃ©pertoire et ses sous-rÃ©pertoires.
    // De plus, on spÃ©cifie le filtre qui nous permettra de rÃ©cupÃ©rer uniquement les fichiers du type souhaitÃ©.
    if (path == Parameters::storageDirectory())
    {
        QDirIterator dirIterator(selectDir, listFilter ,QDir::Files | QDir::NoSymLinks | QDir::AllDirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
        // Tant qu'on n'est pas arrivÃ© Ã  la fin de l'arborescence...
        qDebug() << "---    Listing of files and directories ";
        fileList << QFileInfo(Parameters::storageDirectory());
        while(dirIterator.hasNext())
        {
            // ...on va au prochain fichier correspondant Ã  notre filtre
            //fileList << QFileInfo(dirIterator.next());
            QFileInfo fileInfo1 = QFileInfo(dirIterator.next());
            //qDebug()<< fileInfo1.filePath() + ((fileInfo1.isDir()) ? " directory" : " file");
            fileList << fileInfo1;
        }
        qDebug() << "---    End of listing";

    }
    else
    {
        QDirIterator dirIterator(selectDir, listFilter ,QDir::Files | QDir::NoSymLinks | QDir::AllDirs | QDir::NoDotAndDotDot);
        // Tant qu'on n'est pas arrivÃ© Ã  la fin de l'arborescence...
        qDebug() << "---    Listing of files and directories ";
        while(dirIterator.hasNext())
        {
            // ...on va au prochain fichier correspondant Ã  notre filtre
            //fileList << QFileInfo(dirIterator.next());
            QFileInfo fileInfo1 = QFileInfo(dirIterator.next());
            //qDebug()<< fileInfo1.filePath() + ((fileInfo1.isDir()) ? " directory" : " file");
            fileList << fileInfo1;
        }
        qDebug() << "---    End of listing";
    }
    emit directoryChecked(&fileList);
    sleep(Parameters::getDirServiceSleepTime());
    //}
  */
