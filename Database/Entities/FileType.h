#ifndef CLASSTYPE_H
#define CLASSTYPE_H

#include <QString>
#include <QDebug>
#include <Database/Entities/DbEntity.h>

class FileType: public DbEntity
{
private:
    //Class' properties
    int id;
    QString name;
    QString suffix;


public:

    //Builders
    FileType();
    FileType(int, QString, QString);

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
};


#endif // CLASSTYPE_H
