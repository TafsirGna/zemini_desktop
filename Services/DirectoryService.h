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
#include <Services/LocalDbService.h>
#include <QMessageBox>
#include <QFileDialog>
//#include <>

class DirectoryService : public ZeminiService
{
    Q_OBJECT
private:
    // Variables
    QList<QFileSystemWatcher*> * fsWatchers;
    bool active;
    //QSystemTrayIcon *trayIcon;

    //Functions
    bool removeWatchOver(QFileInfo);

protected:

public:
    DirectoryService(); //FolderThread's constructor
    static bool initFolder(QDir, QStringList);
    void watchZeminiFolder();
    void start();
    QStringList getSubDirNames();
    static bool setUserFolder(QWidget * parent = 0);
    bool isActive();
    static int getDirSize(QDir);
    //void setTrayIcon(QSystemTrayIcon *);

signals:
    void storeInDb(QFileInfo );
    void dirUpdated(QFileInfo);
    void dirDeleted(QFileInfo);
    void fileDeleted(QFileInfo);
    void fileUpdated(QFileInfo);

private slots:
    void handleDirChanges(QString);
    void handleFileChanges(QString);

public slots:
    void watchFile(File *);
    void stop();

signals:
    void rootFolderOnWatching();
    void startWatchingRootDir();

};

#endif // DIRECTORYSERVICE_H
