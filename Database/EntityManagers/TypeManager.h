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
    Type * getByName(QString);
    Type * getBySuffix(QString);
    bool insertType(Type *);
    QList<Type *> * getAllTypes();
    void printTypes(QList<Type *> *);
    void printAllTypes();

    /*
    //functions to retrieve a type object from the local database using a given critera
    Type * findTypeById(int id);
    int getTypeSize(int idType);
    int getAllTypesSize();
    int getTypeSize(QString nameType);
    int getTypePercentage(QString nameType);
    Type * getDefaultType();
    */
};

#endif // CLASSTYPEMANAGER_H
