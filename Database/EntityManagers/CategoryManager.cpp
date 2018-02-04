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
    queryString = "select id,name from category where id = '"+QString::number(id)+"' ";
    query->exec(queryString);
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

bool CategoryManager::addCategory(Category category)
{
    queryString = "insert into Category values('"+QString::number(category.getId())+"', '"+category.getName()+"')";
    query->exec(queryString);
    return query->isActive();
}

QList<Category> * CategoryManager::getAllCategories()
{
    QList<Category> * categories = NULL;

    queryString = "select id, name from category";
    query->exec(queryString);
    if (!query->isActive()){
        return categories;
    }

    if (query->size()==0){
        return categories;
    }

    categories = new QList<Category>();
    while (query->next()){
        Category cat(query->value(0).toInt(), query->value(1).toString());
        categories->append(cat);
    }
    return categories;
}

Category * CategoryManager::getByName(QString name)
{
    queryString = "select * from Category where name = '"+name+"'";
    query->exec(queryString);
    if (query->next()){
        return new Category(query->value(0).toInt(), query->value(1).toString());
    }
    return NULL;
}
