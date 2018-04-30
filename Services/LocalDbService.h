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
#include <Services/NetworkService.h>

class LocalDBService: public ZeminiService
{
    Q_OBJECT
private :
    //All entity managers
    FileManager * fileManager;
    CategoryManager * categoryManager;
    UserManager * userManager;
    FileTypeManager * fileTypeManager;
    AppDataManager * appDataManager;
    DriveTypeManager * driveTypeManager;
    DriveManager * driveManager;
    FileFormatManager * fileFormatManager;

    QStringList dbTables2Init;

    // Some getters
    UserManager * getUserManager();
    CategoryManager * getCategoryManager();
    FileManager * getFileManager();
    FileTypeManager * getFileTypeManager();
    AppDataManager * getAppDataManager();
    DriveTypeManager * getDriveTypeManager();
    DriveManager * getDriveManager();
    FileFormatManager * getFileFormatManager();

protected:

public:

    //Constants
    const static QString USER;
    const static QString CATEGORY;
    const static QString TYPE;
    const static QString FILE;
    const static QString FILE_TYPE;
    const static QString APP_DATA;
    const static QString DRIVE_TYPE;
    const static QString DRIVE;
    const static QString FILE_FORMAT;
    const static QStringList INIT_DATA_LIST;

    //Builder
    LocalDBService();

    // Getters
    bool isDbEmpty();
    AbstractManager * getManager(QString);
    DbEntity * save(File *);
    DbEntity *save(FileType *);
    DbEntity *save(FileFormat *);
    DbEntity * save(Category *);
    bool update(AppData * );
    void onDbInit(QMap<QString, QString>, QList<DbEntity> *);
    void completeDbInit();

public slots:
    bool save(QFileInfo);
    bool saveFileDeletion(QFileInfo);
    void startBackingUp();
    void refreshDb();
    bool saveFileUpdate(QFileInfo);
    void onRequestReplyReceived(QMap<QString,QString>,QList<DbEntity>*);

signals:
    void filesToSend(QList<File*>*);
    void userLoggedIn();
    void dbInitialized();

};
#endif // LOCALDBSERVICE_H
