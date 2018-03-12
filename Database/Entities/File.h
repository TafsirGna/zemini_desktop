#ifndef CLASSFILE_H
#define CLASSFILE_H

#include <QString>
#include <QDateTime>
#include "Database/Entities/Type.h"
#include "Database/Entities/Category.h"
#include <QDebug>
#include <QFileInfo>
#include <Config/Parameters.h>

using namespace std;

class File
{
private:
    int id;
    QDateTime createdAt;
    QDateTime updatedAt;
    QString path;
    QString fileName;
    int size;
    int status;
    Type * type;
    Category * category;
    File * folder;
    QFileInfo * thumbnail;

public:
    //Constructors
    File();
    File(int id, QString fileName, QString path, QDateTime createdAt, QDateTime updatedAt, int size, int status, QFileInfo *thumbnail, Type * type, Category * category, File * folder);
    File(const File & file);
    File & operator =(const File & file);

    //Getters
    int getId() const;
    QString getPath() const;
    QString getFileName() const;
    QDateTime getUpdatedAt() const;
    QDateTime getCreatedAt() const;
    Category * getCategory() const;
    Type * getType() const;
    File * getFolder() const;
    int getSize() const;
    QFileInfo *getThumbnail() const;
    QString getAbsolutePath() const;

    //Setters
    void setId(int id);
    void setPath(QString path);
    void setSize(int size);
    void setCategory(Category * category);
    void setFolder(File * folder);
    void setType(Type * type);
    void setFileName(QString fileName);
    void setCreatedAt(QDateTime);
    void setUpdatedAt(QDateTime);
    void setThumbnail(QFileInfo *value);

    QString toString();
    QString serialize();

    //static variables
    const static int FILE_SAVED;
    const static int FILE_ADDED;
    const static int FILE_DELETED;
    const static int FILE_UPDATED;
    bool getStatus() const;
    void setStatus(bool value);
};

#endif // CLASSFILE_H
