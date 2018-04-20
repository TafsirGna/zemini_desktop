#ifndef CLASSTYPEMANAGER_H
#define CLASSTYPEMANAGER_H

#include "AbstractManager.h"
#include "Database\Entities\FileType.h"
#include "Database\EntityManagers\CategoryManager.h"
#include <Database/EntityManagers/FileFormatManager.h>
#include <QSqlError>

class FileTypeManager : public AbstractManager
{
private :
    CategoryManager * categoryManager;
public:
    FileTypeManager();
    static FileType * getOneBy(QMap<QString, QString>);
    static FileType * add(FileType *);
    static FileType * update(FileType *);
    bool remove(FileType*);
    static QList<FileType *> * getAll();
    static void print(QList<FileType *> *);
    static FileType * getType(QFileInfo);
    static void printAll();

    /*
    //functions to retrieve a type object from the local database using a given critera
    Type * findTypeById(int id);
    int getTypeSize(int idType);
    int getAllTypesSize();
    int getTypeSize(QString nameType);
    int getTypePercentage(QString nameType);
    Type * getDefaultType();
    */
};

#endif // CLASSTYPEMANAGER_H
