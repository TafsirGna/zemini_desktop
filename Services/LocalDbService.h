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

    void initDb();

public slots:
    bool save(User *);
    bool save(QFileInfo);
    bool saveFileDeletion(QFileInfo);
    void startBackingUp();
    void markFileSaved(int);
    void initDb(QList<DbEntity> *, QString);
    void refreshDb();
    bool saveFileUpdate(QFileInfo);

signals:
    void filesToSend(QList<File*>*);
    void userLoggedIn();
    void dbTableInitialized();

};
#endif // LOCALDBSERVICE_H
