#ifndef CLASSFILE_H
#define CLASSFILE_H

#include <QString>
#include <QDateTime>
#include "Database/Entities/FileType.h"
#include "Database/Entities/Category.h"
#include "Database/Entities/Drive.h"
#include <QDebug>
#include <QFileInfo>
#include <Config/Parameters.h>
#include <Database/Entities/DbEntity.h>
#include <QImage>
#include <QBuffer>

using namespace std;

class File : public DbEntity
{
private:
    int id;
    QDateTime createdAt;
    QDateTime updatedAt;
    QString path;
    QString fileName;
    int size;
    int status;
    FileType * type;
    Category * category;
    File * folder;
    QFileInfo * thumbnail;
    Drive * drive;

public:
    //Constructors
    File();
    File(int id, QString fileName, QString path, QDateTime createdAt, QDateTime updatedAt, int size, int status, QFileInfo *thumbnail, FileType * type, Category * category, File * folder, Drive * drive);
    File(const File & file);
    File & operator =(const File & file);

    //Getters
    int getId() const;
    QString getPath() const;
    QString getFileName() const;
    QDateTime getUpdatedAt() const;
    QDateTime getCreatedAt() const;
    Category * getCategory() const;
    FileType * getType() const;
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
    void setType(FileType * fileType);
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
    Drive *getDrive() const;
    void setDrive(Drive *value);
};

#endif // CLASSFILE_H
