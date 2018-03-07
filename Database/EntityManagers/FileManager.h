#ifndef CLASSFILEMANAGER_H
#define CLASSFILEMANAGER_H

#include "Database/Entities/File.h"
#include "Database/EntityManagers/TypeManager.h"
#include "Config/Functions.h"

class FileManager : public AbstractManager
{
    Q_OBJECT
private :
    TypeManager * typeManager;
    CategoryManager * categoryManager;

protected:

public :
    FileManager();

    bool saveFile(File * file);
    bool updateFile(File * file);
    bool deleteFile(File * file);
    bool updateDirContent(QDir);
    QList<File *> *getAllFiles();
    QList<File *> *getNotSavedFiles();
    bool setFileSaved(int);
    File * convertToFile(QFileInfo);
    File * getByFileName(QString);
    void printAllFiles();

    void setCategoryManager(CategoryManager *);
    void setTypeManager(TypeManager *);

    Category * getCategory(QFileInfo);
    Type * getType(QFileInfo);
    File * getFolder(QFileInfo);
    File * getFileAt(QFileInfo);

public slots:

signals:
    void fileSaved(QFileInfo);
    void fileSaved(File *);

};

#endif // CLASSFILEMANAGER_H
