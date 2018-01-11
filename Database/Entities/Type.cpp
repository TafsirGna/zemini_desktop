#include "Database\Entities\Type.h"

//---   Builders
Type::Type()
{

}

Type::Type(int id, QString name, Category *category)
{
    this->id = id;
    this->name = name;
    this->category = category;
}

//---   Setters
void Type::setId(int id)
{
    this->id = id;
}

void Type::setName(QString name)
{
    this->name = name;
}

void Type::setCategory(Category *category)
{
    this->category = category;
}

//---   Getters
int Type::getId()
{
    return this->id;
}

QString Type::getName()
{
    return this->name;
}

Category * Type::getCategory()
{
    return this->category;
}
