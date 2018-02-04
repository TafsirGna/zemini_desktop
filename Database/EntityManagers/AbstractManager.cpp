#include "AbstractManager.h"

/***            Class Constructor           ***/
AbstractManager::AbstractManager()
{
    this->query= new QSqlQuery(db);
    this->busy = false;
}

AbstractManager::AbstractManager(QSqlDatabase &db)
{
    this->db = db;
    this->query= new QSqlQuery(db);
    this->busy = false;
}

AbstractManager::AbstractManager(const AbstractManager &absManager)
{
    this->db = absManager.db;
    this->query = absManager.query;
    this->busy = absManager.busy;
}

AbstractManager & AbstractManager::operator =(const AbstractManager & absManager)
{
    this->db = absManager.db;
    this->query = absManager.query;
    this->busy = absManager.busy;
    return *this;
}

bool AbstractManager::isBusy()
{
    return this->busy;
}

void AbstractManager::saveUpdateToRemoteDB()
{

}
