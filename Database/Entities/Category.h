#ifndef CLASSCATEGORY_H
#define CLASSCATEGORY_H

#include <QString>
#include <Database/Entities/DbEntity.h>

class Category : public DbEntity
{
private:
    //Class' properties
    int id;
    QString name;

public:

    //Builders
    Category();
    Category(int id, QString name);

    //Getters
    int getId();
    QString getName();

    //Setters
    void setId(int id);
    void setName(QString name);

    QString toString();
};

#endif // CLASSCATEGORY_H
