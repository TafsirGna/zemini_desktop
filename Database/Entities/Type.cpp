#include "Database\Entities\Type.h"

//---   Builders
Type::Type()
{

}

Type::Type(int id, QString name, QString suffix)
{
    this->id = id;
    this->name = name;
    this->suffix = suffix;
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

void Type::setSuffix(QString suffix)
{
    this->suffix = suffix;
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

QString Type::getSuffix()
{
    return this->suffix;
}
