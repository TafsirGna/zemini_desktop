#ifndef CLASSFILEMANAGER_H
#define CLASSFILEMANAGER_H

#include "Database/Entities/File.h"
#include "Database/EntityManagers/FileTypeManager.h"
#include "Config/Functions.h"
#include "Database/EntityManagers/DriveManager.h"

class FileManager : public AbstractManager
{
    Q_OBJECT
private :
    //FileTypeManager * fileTypeManager;
    //CategoryManager * categoryManager;

protected:

public :
    FileManager();

    File* add(QFileInfo);
    File* add(File * file);
    static File* update(File * file);
    bool symDelete(File * file);
    bool hardDelete(File * file);
    bool updateDirContent(QDir);
    static QList<File *> *getAll();
    static QList<File *> *getNotSavedFiles();
    static bool setFileSaved(int);
    static File * convertToFile(QFileInfo);
    static File *getOneBy(QMap<QString, QString>);
    static QList<File *> *getBy(QMap<QString, QString>);
    static void printAll();
    static void deleteAll();
    static bool updateDbChildrenOf(File *);

    //void setCategoryManager(CategoryManager *);
    //void setFileTypeManager(FileTypeManager *);

    static Category * getCategory(QFileInfo);
    static FileType * getType(QFileInfo);
    Drive *getDrive();
    static int getSizeOnFS(QFileInfo);
    static int getSizeOnDb(File *);
    static int getSizeOnDb(QFileInfo );

    //static int getSizeOf(QFileInfo);
    static int getSizePercentage(QFileInfo, QDir);

public slots:

signals:
    void fileSaved(File *);
    void fileDeleted(File *);
    void fileUpdated(File *);

};

#endif // CLASSFILEMANAGER_H
