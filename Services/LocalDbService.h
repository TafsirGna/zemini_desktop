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
#include <Services/NetworkService.h>

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

    QStringList dbTables2Init;

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
    static bool isDbEmpty();
    const static AbstractManager * getManager(QString);
    static DbEntity * save(QMap<QString, QString>, DbEntity *);
    static  bool update(AppData * );
    void onDbInit(QMap<QString, QString>, QList<DbEntity*> *);
    static void completeDbInit();
    static DbEntity * getOneBy(QMap<QString, QString>);

public slots:
    bool save(QFileInfo);
    bool saveFileDeletion(QFileInfo);
    void startBackingUp();
    void refreshDb();
    bool saveFileUpdate(QFileInfo);
    void onRequestReplyReceived(QMap<QString,QString>,QList<DbEntity*>*);

signals:
    void filesToSend(QString, QList<File*>*);
    void userSaved();
    void dbInitialized();

};
#endif // LOCALDBSERVICE_H
