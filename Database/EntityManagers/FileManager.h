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
    File* update(File * file);
    bool symDelete(File * file);
    bool hardDelete(File * file);
    bool updateDirContent(QDir);
    static QList<File *> *getAll();
    static QList<File *> *getNotSavedFiles();
    static bool setFileSaved(int);
    static File * convertToFile(QFileInfo);
    static File *getOneBy(QMap<QString, QString>);
    static void printAll();
    static void deleteAll();

    //void setCategoryManager(CategoryManager *);
    //void setFileTypeManager(FileTypeManager *);

    static Category * getCategory(QFileInfo);
    static FileType * getType(QFileInfo);
    static File * getFolder(QFileInfo);
    static File * getFileAt(QFileInfo);
    Drive *getDrive();

public slots:

signals:
    void fileSaved(File *);
    void fileDeleted(File *);
    void fileUpdated(File *);

};

#endif // CLASSFILEMANAGER_H
