#include "Database\Entities\Type.h"

//---   Builders
/**
 * @brief Type::Type
 */
Type::Type()
{

}

/**
 * @brief Type::Type
 * @param id
 * @param name
 * @param suffix
 */
Type::Type(int id, QString name, QString suffix)
{
    this->id = id;
    this->name = name;
    this->suffix = suffix;
}

//---   Setters
/**
 * @brief Type::setId
 * @param id
 */
void Type::setId(int id)
{
    this->id = id;
}

/**
 * @brief Type::setName
 * @param name
 */
void Type::setName(QString name)
{
    this->name = name;
}

/**
 * @brief Type::setSuffix
 * @param suffix
 */
void Type::setSuffix(QString suffix)
{
    this->suffix = suffix;
}

//---   Getters
/**
 * @brief Type::getId
 * @return
 */
int Type::getId()
{
    return this->id;
}

/**
 * @brief Type::getName
 * @return
 */
QString Type::getName()
{
    return this->name;
}

/**
 * @brief Type::getSuffix
 * @return
 */
QString Type::getSuffix()
{
    return this->suffix;
}

//  Other functions

/**
 * @brief Type::toString
 */
void Type::toString()
{
    qDebug() << "id : " << id << " | name : " << name << " | suffix : " << suffix << endl;
}
