#include "Database\EntityManagers\categorymanager.h"

// --- Builders
CategoryManager::CategoryManager() : AbstractManager()
{

}

CategoryManager::CategoryManager( QSqlDatabase &db) : AbstractManager(db)
{

}

// --- implementations of functions used to retrieve category object from the local database following some given critera
Category * CategoryManager::findCategoryById(int id)
{
    query->exec("select id,name from category where id = '"+QString::number(id)+"' ");
    if (!query->isActive())
    {
        qDebug()<<"Error when selecting category : " + query->lastError().text();
        return NULL;
    }

    if (query->next())
        return new Category(query->value(0).toInt(),query->value(1).toString());
    else
        return NULL;
}
