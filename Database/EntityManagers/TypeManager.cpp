#include "Database\EntityManagers\typemanager.h"
#include <QException>

TypeManager::TypeManager(CategoryManager * categoryManager): AbstractManager()
{
    this->categoryManager = categoryManager;
}
/***        Another builder         ***/
TypeManager::TypeManager() : AbstractManager()
{
    this->categoryManager = new CategoryManager();

    /*
    //query->exec("select idcategory,name from type where id = '"+QString::number(id)+"'");
    query->exec("select idcategory,name from type where id = 3");
    if (!query->isActive())
    {
        qDebug()<<"Error when selecting type : " + query->lastError().text();
        return NULL;
    }

    if (query->size() == 0) return NULL;

    query->next();

    qDebug() << "type --- 0 "+ query->value(0).toInt();

    Category * category = NULL;//categoryManager->findCategoryById(query->value(2).toInt());
    return new Type(id,query->value(1).toString(),category);
    */
}

/***        function designed to retrieve a type object from the local database     ***/
/*
Type * TypeManager::findTypeById(int id)
{
    QString request = "select idcategory,name from type where id = "+QString::number(id)+"";
    //qDebug()<< request;

    query->exec(request);

    if (!query->isActive())
    {
        qDebug()<<"Error when selecting type : " + query->lastError().text();
        return NULL;
    }

    if (query->next())
    {
        Category * category = categoryManager->findCategoryById(query->value(2).toInt());
        return new Type(id,query->value(1).toString(),category);
    }
    else
        return getDefaultType();
}

Type * TypeManager::findTypeByName(QString name)
{
    query->exec("select type.id,idcategory,type.name from type,category  where category.id = type.idcategory and  category.name = '"+name+"'");
    if (!query->isActive())
    {
        qDebug()<<"Error when selecting type : " + query->lastError().text();
        return NULL;
    }

    if (query->next())
    {
        Category * category = new Category(query->value(1).toInt(),name);
        return new Type(query->value(0).toInt(),query->value(2).toString(),category);
    }
    else
    {
        return getDefaultType();
    }
}

int TypeManager::getTypeSize(int idType)
{
    QString request = "select sum(size) from file where idtype = "+QString::number(idType)+" and file.status <> 0 ";
    query->exec(request);
    if (!query->isActive())
    {
        qDebug()<< "Error when selecting all files' size ";
        return 0;
    }

    int size = 0;
    if (query->next())
        size = query->value(0).toInt();

    return size;
}

int TypeManager::getTypeSize(QString nameType)
{
    QString request = "select sum(size) from file,type,category where file.idtype = type.id and type.idcategory = category.id and category.name = '"+nameType+"' and file.status <> 0 ";
    query->exec(request);
    if (!query->isActive())
    {
        qDebug()<< "Error when selecting all files' size ";
        return 0;
    }

    int size = 0;
    if (query->next())
        size = query->value(0).toInt();

    //qDebug() << "flush 2 "+ QString::number(size);
    return size;
}

int TypeManager::getAllTypesSize()
{
    QString request = "select sum(size) from file where status <> 0";
    query->exec(request);
    if (!query->isActive())
    {
        qDebug()<< "Error when selecting all files' size ";
        return 0;
    }

    int size = 0;
    if (query->next())
        size = query->value(0).toInt();

    //qDebug() << "flush 1 "+ QString::number(size);
    return size;
}

int TypeManager::getTypePercentage(QString nameType)
{
    double proportion = (double) ((getTypeSize(nameType)*100)/getAllTypesSize());
    //qDebug() << "flush 3 "+ QString::number(proportion);
    int percent = (int) floor(proportion);
    return percent;
}

Type * TypeManager::getDefaultType()
{
    query->exec("select type.id,idcategory,type.name, category.name from type,category  where category.id = type.idcategory and  category.name = 'Others'");
    if (!query->isActive())
    {
        qDebug()<<"Error when selecting type : " + query->lastError().text();
        return NULL;
    }

    if (query->next())
    {
        Category * category = new Category(query->value(1).toInt(),query->value(3).toString());
        return new Type(query->value(0).toInt(),query->value(2).toString(),category);
    }
    return NULL;
}
*/

