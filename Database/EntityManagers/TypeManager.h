#ifndef CLASSTYPEMANAGER_H
#define CLASSTYPEMANAGER_H

#include "AbstractManager.h"
#include "Database\Entities\Type.h"
#include "Database\EntityManagers\CategoryManager.h"
#include <QSqlError>

class TypeManager : public AbstractManager
{
private :
    CategoryManager * categoryManager;
public:
    TypeManager();
    TypeManager(QSqlDatabase &db);
    TypeManager(QSqlDatabase &db, CategoryManager * categoryManager);

    //functions to retrieve a type object from the local database using a given critera
    Type * findTypeById(int id);
    Type * findTypeByName(QString name);
    int getTypeSize(int idType);
    int getAllTypesSize();
    int getTypeSize(QString nameType);
    int getTypePercentage(QString nameType);

    Type * getDefaultType();
};

#endif // CLASSTYPEMANAGER_H
