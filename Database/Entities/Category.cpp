#include "Database\Entities\Category.h"

//---   Builders
Category::Category()
{

}

Category::Category(int id, QString name)
{
    this->id = id;
    this->name = name;
}

//---   Getters
int Category::getId()
{
    return this->id;
}

QString Category::getName()
{
    return this->name;
}

//---   Setters

void Category::setId(int id)
{
    this->id = id;
}

void Category::setName(QString name)
{
    this->name = name;
}

QString Category::toString()
{
    return "Category : id = "+QString::number(id)+", name = "+name;
}
