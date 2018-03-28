#ifndef CLASSCATEGORYMANAGER_H
#define CLASSCATEGORYMANAGER_H

#include "Database\Entities\Category.h"
#include "Database\EntityManagers\AbstractManager.h"
#include <QSqlError>
#include <QList>

class CategoryManager : public AbstractManager
{
private:

public:
    CategoryManager();
    bool isEmpty();

    static Category *add(Category);
    static Category *add(Category *);

    static Category * update(Category category);
    bool remove(Category category);
    static Category* getCategory(QFileInfo fileInfo);

    static QList<Category> * getAll();
    static Category * getOneBy(QMap<QString, QString>);
    static void printAll();
    static void print(QList<Category> *);
};


#endif // CLASSCATEGORYMANAGER_H
