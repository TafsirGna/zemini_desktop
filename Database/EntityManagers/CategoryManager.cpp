#include "Database\EntityManagers\categorymanager.h"

// --- Builders
CategoryManager::CategoryManager() : AbstractManager()
{

}

// --- implementations of functions used to retrieve category object from the local database following some given critera
Category * CategoryManager::findCategoryById(int id)
{
    QString queryString = "select id,name from category where id = '"+QString::number(id)+"' ";
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
    QString queryString = "insert into Category values('"+QString::number(category.getId())+"', '"+category.getName()+"')";
    query->exec(queryString);
    return query->isActive();
}

QList<Category> * CategoryManager::getAllCategories()
{
    QList<Category> * categories = NULL;

    QString queryString = "select id, name from category";
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
    QString queryString = "select * from Category where name = '"+name+"'";
    query->exec(queryString);
    if (query->next()){
        return new Category(query->value(0).toInt(), query->value(1).toString());
    }
    return NULL;
}

bool CategoryManager::isEmpty()
{
    QString queryString = "select count(*) from Category";
    query->exec(queryString);
    if(query->next() && query->value(0).toInt() == 0)
        return true;
    return false;
}
