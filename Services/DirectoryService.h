#ifndef DIRECTORYSERVICE_H
#define DIRECTORYSERVICE_H

#include <QDirIterator>
#include <QDir>
#include <QDebug>
#include "Config/Parameters.h"
#include <Database/Entities/Category.h>
#include <QList>
#include <QFileSystemWatcher>
#include <Services/ZeminiService.h>
#include "Config/Functions.h"
#include <QMessageBox>

class DirectoryService : public ZeminiService
{
    Q_OBJECT
private:
    // Variables
    QList<QFileSystemWatcher*> * fsWatchers;

    //Functions
    void storeInDb(QFileInfo );
    bool removeWatchOver(QFileInfo);

protected:

public:
    DirectoryService(); //FolderThread's constructor
    bool makeInitDirectories(QList<Category> *);
    void watchZeminiFolder();
    void start();

signals:
    void fileInfoToSave(QFileInfo);
    void dirContentToUpdate(QFileInfo);
    void dirDeleted(QFileInfo);
    void fileDeleted(QFileInfo);
    void fileUpdated(QFileInfo);

private slots:
    void handleDirChanges(QString);
    void handleFileChanges(QString);

public slots:
    void watchFile(QFileInfo);

};

#endif // DIRECTORYSERVICE_H
