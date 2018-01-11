#ifndef CLASSTYPE_H
#define CLASSTYPE_H

#include <QString>
#include "Database\Entities\Category.h"

class Type
{
private:
    //Class' properties
    int id;
    QString name;
    Category * category;


public:

    //Builders
    Type();
    Type(int id, QString name, Category * category);

    //Getters
    int getId();
    QString getName();
    Category * getCategory();

    //Setters
    void setId(int id);
    void setName(QString name);
    void setCategory(Category * category);
};


#endif // CLASSTYPE_H
