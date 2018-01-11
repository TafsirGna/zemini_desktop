#ifndef CLASSCATEGORYMANAGER_H
#define CLASSCATEGORYMANAGER_H

#include "Database\Entities\Category.h"
#include "Database\EntityManagers\AbstractManager.h"
#include <QSqlError>

class CategoryManager : public AbstractManager
{
private:

public:
    CategoryManager();
    CategoryManager(QSqlDatabase &db);

    //functions used to retrieve a category object from the local database following some given critera;
    Category * findCategoryById(int id);
};


#endif // CLASSCATEGORYMANAGER_H
