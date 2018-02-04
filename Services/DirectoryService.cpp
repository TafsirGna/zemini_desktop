#include "DirectoryService.h"
#include <Services/LocalDbService.h>
#include <Services/NetworkService.h>

/***    Folder's default constructor    ***/
DirectoryService::DirectoryService(QObject *parent)
{
    fsWatchers = new QList<QFileSystemWatcher>();
}

void DirectoryService::start()
{
    watchZeminiFolder();
}

void DirectoryService::watchZeminiFolder()
{
    // i start going through all files and directories to store and track them
    QFileInfo currentObject(Parameters::storageDirectory());
    QFileInfoList * queue = new QFileInfoList();
    while(true){
        qDebug() << currentObject.absoluteFilePath() << endl;
        if (currentObject.isFile())
            storeInDb(currentObject);
        else{
            QDir currentDirectory(currentObject.absoluteFilePath());
            (*queue) += currentDirectory.entryInfoList(QDir::Files | QDir::NoSymLinks | QDir::AllDirs | QDir::NoDotAndDotDot);
        }
        if (queue->isEmpty())
            break;
        currentObject = (queue->last());
        queue->removeLast();
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
    QString rootDir = Parameters::storageDirectory();
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
    // finally make a link to the zemini folder
    Functions::makeLinkToZeminiFolder();

    return true;
}

void DirectoryService::storeInDb(QFileInfo fileInfo)
{
    emit fileInfoToSave(fileInfo);
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