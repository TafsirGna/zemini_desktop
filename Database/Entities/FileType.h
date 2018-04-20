#ifndef CLASSTYPE_H
#define CLASSTYPE_H

#include <QString>
#include <QDebug>
#include <Database/Entities/DbEntity.h>
#include <Database/Entities/FileFormat.h>

class FileType: public DbEntity
{
private:
    //Class' properties
    int id;
    QString name;
    QString suffix;
    FileFormat *fileFormat;

public:

    //Builders
    FileType();
    FileType(int, QString, QString, FileFormat*);

    //Getters
    int getId();
    QString getName();
    QString getSuffix();

    //Setters
    void setId(int id);
    void setName(QString);
    void setSuffix(QString);

    // Other functions
    void toString();
    FileFormat *getFileFormat() const;
    void setFileFormat(FileFormat *value);
};


#endif // CLASSTYPE_H
