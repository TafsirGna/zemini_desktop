#include "AbstractManager.h"

/***            Class Constructor           ***/
AbstractManager::AbstractManager()
{
    this->accessManager = new QNetworkAccessManager(this);
    this->query= new QSqlQuery(db);
    this->busy = false;
}

AbstractManager::AbstractManager(QSqlDatabase &db)
{
    this->accessManager = new QNetworkAccessManager(this);
    this->db = db;
    this->query= new QSqlQuery(db);
    this->busy = false;
}

AbstractManager::AbstractManager(const AbstractManager &absManager)
{
    this->accessManager = absManager.accessManager;
    this->db = absManager.db;
    this->query = absManager.query;
    this->str_Url = absManager.str_Url;
    this->action = absManager.action;
    this->connected = absManager.connected;
    this->busy = absManager.busy;
}

AbstractManager & AbstractManager::operator =(const AbstractManager & absManager)
{
    this->accessManager = absManager.accessManager;
    this->db = absManager.db;
    this->query = absManager.query;
    this->str_Url = absManager.str_Url;
    this->action = absManager.action;
    this->connected = absManager.connected;
    this->busy = absManager.busy;
    return *this;
}

bool AbstractManager::isConnected()
{
    return this->connected;
}

bool AbstractManager::isBusy()
{
    return this->busy;
}

void AbstractManager::saveUpdateToRemoteDB()
{

}
