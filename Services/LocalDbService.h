#ifndef LOCALDBSERVICE_H
#define LOCALDBSERVICE_H

#include <QDebug>
#include <QFileInfoList>
#include "Database/EntityManagers/UserManager.h"
#include "Database/EntityManagers/FileManager.h"
#include <Services/ZeminiService.h>
#include <Database/EntityManagers/AppDataManager.h>

class LocalDBService: public ZeminiService
{
    Q_OBJECT
private :
    //All entity managers
    FileManager * fileManager;
    CategoryManager * categoryManager;
    UserManager * userManager;
    TypeManager * typeManager;
    AppDataManager * appDataManager;

    // Some getters
    UserManager * getUserManager();
    CategoryManager * getCategoryManager();
    FileManager * getFileManager();
    TypeManager * getTypeManager();
    AppDataManager * getAppDataManager();

protected:

public:

    //Constants
    const static QString USER;
    const static QString CATEGORY;
    const static QString TYPE;
    const static QString FILE;
    const static QString APP_DATA;

    //Builder
    LocalDBService();

    // Getters
    bool isDbEmpty();
    AbstractManager * getManager(QString);
    bool save(File *);
    bool update(AppData * );

public slots:
    bool save(User *);
    bool save(QFileInfo);
    void updateDirContent(QFileInfo);
    bool deleteDir(QFileInfo);
    void startBackingUp();
    void markFileSaved(int);
    void initDb(QList<Category> *);
    void refreshDb();
    bool deleteFile(QFileInfo);
    bool updateFile(QFileInfo);

signals:
    void fileToSend(File*);
    void userLoggedIn();

};
#endif // LOCALDBSERVICE_H
