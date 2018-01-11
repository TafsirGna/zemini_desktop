#include "DirectoryService.h"

/***    Folder's default constructor    ***/
DirectoryService::DirectoryService(QObject *parent) : QThread(parent)
{
    this->parent = parent;
    stopped = false;
    this->path = Parameters::storageDirectory();
}

/***    Redefinition of the function run    ***/
void DirectoryService::run()
{
    checkFolder();
}

void DirectoryService::checkFolder()
{
    //while (!stopped)
    //{

    // On sélectionne le répertoire à partir duquel on va rechercher les fichiers AVI et MP3
    QString selectDir = path;

    // On remplit une QStringList avec chacun des filtres désirés ici "*.mp3" et "*.avi".
    QStringList listFilter;
    //listFilter << "*.avi";
    //listFilter << "*.mp3";

    // Variable qui contiendra tous les fichiers correspondant à notre recherche
    QFileInfoList fileList;

    // On déclare un QDirIterator dans lequel on indique que l'on souhaite parcourir un répertoire et ses sous-répertoires.
    // De plus, on spécifie le filtre qui nous permettra de récupérer uniquement les fichiers du type souhaité.
    if (path == Parameters::storageDirectory())
    {
        QDirIterator dirIterator(selectDir, listFilter ,QDir::Files | QDir::NoSymLinks | QDir::AllDirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
        // Tant qu'on n'est pas arrivé à la fin de l'arborescence...
        qDebug() << "---    Listing of files and directories ";
        fileList << QFileInfo(Parameters::storageDirectory());
        while(dirIterator.hasNext())
        {
            // ...on va au prochain fichier correspondant à notre filtre
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
        // Tant qu'on n'est pas arrivé à la fin de l'arborescence...
        qDebug() << "---    Listing of files and directories ";
        while(dirIterator.hasNext())
        {
            // ...on va au prochain fichier correspondant à notre filtre
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
}

/***    this stop the execution of a FolderThread object    ***/
void DirectoryService::stop()
{
    stopped = true;
}

void DirectoryService::setPath(QString path)
{
    this->path = path;
}
