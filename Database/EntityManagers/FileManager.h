#ifndef CLASSFILEMANAGER_H
#define CLASSFILEMANAGER_H

#include "Database/Entities/File.h"
#include "Database/EntityManagers/TypeManager.h"

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
    bool cleanDirFile(QDir);
    QList<File> getAllFiles();
    QList<File> getNotSavedFiles();
    bool setFileSaved(int);
    File * convertToFile(QFileInfo);
    File * getByFileName(QString);

    void setCategoryManager(CategoryManager *);
    void setTypeManager(TypeManager *);

    Category * getCategory(QFileInfo);
    Type * getType(QFileInfo);
    File * getFolder(QFileInfo);

public slots:

};

#endif // CLASSFILEMANAGER_H
