#ifndef CLASSFILE_H
#define CLASSFILE_H

#include <QString>
#include <QDateTime>
#include "Database/Entities/Type.h"
#include "Database/Entities/Category.h"
#include <QDebug>
#include <QFileInfo>

using namespace std;

class File
{
private:
    int id;
    QDateTime creationTime;
    QDateTime updateTime;
    QString path;
    QString fileName;
    int size;
    bool saved;
    Type * type;
    Category * category;
    File * folder;

public:
    //Constructors
    File();
    File(int id, QString fileName, QString path, QDateTime creationTime, QDateTime updateTime, int size, bool saved, Type * type, Category * category, File * folder);
    File(const File & file);
    File & operator =(const File & file);

    //Getters
    int getId();
    QString getPath();
    QString getFileName();
    bool isSaved();
    QDateTime getUpdateTime();
    QDateTime getCreationTime();
    Category * getCategory();
    Type * getType();
    File * getFolder();
    int getSize();

    //Setters
    void setId(int id);
    void setPath(QString path);
    void setSize(int size);
    void setCategory(Category * category);
    void setFolder(File * folder);
    void setType(Type * type);
    void setFileName(QString fileName);
    void setSaved(bool saved);
    void setCreationTime(QDateTime creationTime);
    void setUpdateTime(QDateTime updateTime);

    void toString();

    //static functions
    static File * convertToFile(QFileInfo);
};

#endif // CLASSFILE_H
