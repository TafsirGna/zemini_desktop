#include "AbstractManager.h"

/***            Class Constructor           ***/
AbstractManager::AbstractManager()
{
    this->query= new QSqlQuery(Parameters::localDb);
}

AbstractManager::AbstractManager(const AbstractManager &absManager)
{
    this->query = absManager.query;
}

AbstractManager & AbstractManager::operator =(const AbstractManager & absManager)
{
    this->query = absManager.query;
    return *this;
}
