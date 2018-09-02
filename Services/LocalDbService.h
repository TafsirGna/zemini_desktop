#ifndef LOCALDBSERVICE_H
#define LOCALDBSERVICE_H

#include <QDebug>
#include <QFileInfoList>
#include "Database/EntityManagers/UserManager.h"
#include "Database/EntityManagers/FileManager.h"
#include <Services/ZeminiService.h>
#include <Database/EntityManagers/AppDataManager.h>
#include <Database/EntityManagers/DriveTypeManager.h>
#include <Database/EntityManagers/DriveManager.h>
#include <Database/EntityManagers/FileFormatManager.h>
#include <Database/EntityManagers/FileManager.h>
#include <QTimer>

class LocalDBService: public ZeminiService
{
    Q_OBJECT
private :
    //All entity managers
    static FileManager * fileManager;
    static CategoryManager * categoryManager;
    static UserManager * userManager;
    static FileTypeManager * fileTypeManager;
    static AppDataManager * appDataManager;
    static DriveTypeManager * driveTypeManager;
    static DriveManager * driveManager;
    static FileFormatManager * fileFormatManager;
    static bool dbInitStatus;

    QStringList dbTables2Init;
    int nbFiles2Send;

    // Some getters
    static UserManager * getUserManager();
    static CategoryManager * getCategoryManager();
    static FileManager * getFileManager();
    static FileTypeManager * getFileTypeManager();
    static AppDataManager * getAppDataManager();
    static DriveTypeManager * getDriveTypeManager();
    static DriveManager * getDriveManager();
    static FileFormatManager * getFileFormatManager();

protected:

public:

    //Builder
    LocalDBService();

    // Getters
    static bool isDbEmpty();
    static bool isDbInitialized();
    const static AbstractManager * getManager(QString);
    static DbEntity * save(QMap<QString, QString>, DbEntity *);
    static  bool update(AppData * );
    void onDbInit(QMap<QString, QString>, QList<DbEntity*> *);
    static void completeDbInit();
    static DbEntity * getOneBy(QMap<QString, QString>);
    static QStringList getSubDirNames();

public slots:
    bool save(QFileInfo);
    bool saveFileDeletion(QFileInfo);
    void startBackingUp();
    void refreshDb();
    bool saveFileUpdate(QFileInfo);
    void onRequestReplyReceived(QMap<QString,QString>,QList<DbEntity*>*);

signals:
    void filesToSend(QString, QList<DbEntity*>*);
    void userSaved(int);
    void userEnabled(bool);
    void dbInitialized();
    void fileBackedUp(File *);
    void thumbBackedUp(File *);
};
#endif // LOCALDBSERVICE_H
