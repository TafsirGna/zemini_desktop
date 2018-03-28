#include "Database\EntityManagers\categorymanager.h"

// --- Builders
CategoryManager::CategoryManager() : AbstractManager()
{

}

Category * CategoryManager::getCategory(QFileInfo fileInfo)
{
    //printAll();
    QString path = fileInfo.absoluteFilePath();
    QStringList pathTab = path.split("/");
    int index  = pathTab.indexOf(Parameters::ROOT_DIR_NAME);

    QMap<QString, QString> catProperties;
    catProperties.insert("name", pathTab.at(index+1));
    Category * category = CategoryManager::getOneBy(catProperties);
    if (category == NULL)
        return add(new Category(0, pathTab.at(index+1)));
    return category;
}

Category * CategoryManager::add(Category category)
{
    QString request = "insert into Category values('"+QString::number(category.getId())+"', '"+category.getName()+"')";
    //qDebug() << request << endl;
    QSqlQuery sqlQuery(Parameters::localDb);
    sqlQuery.exec(request);
    if (!sqlQuery.isActive()){
        qDebug() << "Failed to insert a new catagory : " << sqlQuery.lastError().text() << endl;
        return NULL;
    }

    QMap<QString, QString> catProperties;
    catProperties.insert("name", category.getName());
    return getOneBy(catProperties);
}

Category * CategoryManager::add(Category * category)
{
    return add(*category);
}

QList<Category> * CategoryManager::getAll()
{
    QList<Category> * categories = NULL;

    QString request = "select id, name from Category";
    QSqlQuery sqlQuery(Parameters::localDb);
    sqlQuery.exec(request);
    if (!sqlQuery.isActive()){
        return categories;
    }

    categories = new QList<Category>();
    if (sqlQuery.size()==0)
        return categories;
    while (sqlQuery.next()){
        Category cat(sqlQuery.value(0).toInt(), sqlQuery.value(1).toString());
        categories->append(cat);
    }
    return categories;
}

Category *CategoryManager::getOneBy(QMap<QString, QString> properties)
{
    //Building the sql request
    QString request = "select * from Category where ";

    QMapIterator<QString, QString> i(properties);
    int j = 0;
    while(i.hasNext()){
        i.next();
        request += ((j != 0) ? " and " : " ") + i.key()+" = '"+i.value()+"'";
        j++;
    }

    QSqlQuery sqlQuery(Parameters::localDb);
    sqlQuery.exec(request);
    if (!sqlQuery.isActive()){
        qDebug() << "Failed to select the category ! " << sqlQuery.lastError().text() <<endl;
        return NULL;
    }

    if (sqlQuery.next()){
        //qDebug() << "Yes category" << endl;
        return new Category(sqlQuery.value(0).toInt(), sqlQuery.value(1).toString());
    }
    return NULL;
}

void CategoryManager::printAll()
{
    print(getAll());
}

void CategoryManager::print(QList<Category>* categories)
{
    for (int i(0); i < categories->size(); i++){
        Category category = categories->at(i);
        qDebug() << category.toString() << endl;
    }
}

bool CategoryManager::isEmpty()
{
    QString queryString = "select count(*) from Category";
    QSqlQuery sqlQuery(Parameters::localDb);
    sqlQuery.exec(queryString);
    if(sqlQuery.next() && sqlQuery.value(0).toInt() == 0)
        return true;
    return false;
}

bool CategoryManager::remove(Category category)
{
    QString request = "delete from Category where id = "+ QString::number(category.getId());
    QSqlQuery sqlQuery(Parameters::localDb);
    sqlQuery.exec(request);
    if(!sqlQuery.isActive())
        return true;
    return false;
}

Category * CategoryManager::update(Category category)
{
    QString request = "update Category set name = '"+category.getName()+"' where id = "+QString::number(category.getId());
    QSqlQuery sqlQuery(Parameters::localDb);
    sqlQuery.exec(request);
    if (!sqlQuery.isActive()){
        qDebug() << "Failed to update the given category : " << sqlQuery.lastError().text() << endl;
        return NULL;
    }
    return &category;
}
