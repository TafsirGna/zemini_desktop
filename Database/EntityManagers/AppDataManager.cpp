#include "AppDataManager.h"

AppDataManager::AppDataManager() : AbstractManager()
{

}

bool AppDataManager::addAppData(AppData * data)
{
    QString request = "insert into Appdata(key, value) select '"+data->getKey()+"', '"+data->getValue()
            +"' where not exists (select 1 from Appdata where key = '"+data->getKey()+"')";
    //QString request = "insert into Appdata values ('"+data->getKey()+"', '"+data->getValue()+"');";
    qDebug() << request << endl;
    query->exec(request);
    if (!query->isActive()){
        qDebug() << "Failed in inserting an app data ! " << query->lastError().text() << endl;
        return false;
    }
    //qDebug() << "Succeeded in inserting an app data ! " << query->lastError().text() << endl;
    return true;
}

bool AppDataManager::updateAppData(AppData * data)
{
    QString request = "update Appdata set value = '"+data->getValue()+"' where key = '"+data->getKey()+"' ";
    query->exec(request);
    if (!query->isActive()){
        qDebug() << "Failed in updating an app data ! " << endl;
        return false;
    }
    return true;
}

bool AppDataManager::deleteAppData(AppData *data)
{
    QString request = "delete from Appdata where key = '"+data->getKey()+"'";
    query->exec(request);
    if (!query->isActive()){
        qDebug() << "Failed in deleting an app data ! " << endl;
        return false;
    }
    return true;
}

AppData *AppDataManager::getByKey(QString key)
{
    //qDebug() << "test1" << endl;
    QString request = "select key, value from Appdata where key = '"+key+"'";
    query->exec(request);
    if (!query->isActive()){
        qDebug() << "Failed in selecting an app data ! " << endl;
        return NULL;
    }

    //qDebug() << "test2 : " << query->size() << endl;
    if (query->next()){
        return new AppData(query->value(0).toString(), query->value(1).toString());
    }
    return NULL;
}

QList<AppData *> * AppDataManager::getAllData()
{
    QString request = "select key, value from Appdata";
    query->exec(request);
    if (!query->isActive()){
        qDebug() << "Failed to get all app's data ! " << query->lastError().text() << endl;
        return NULL;
    }
    QList<AppData*> *data = new QList<AppData*>();
    while(query->next()){
        data->append(new AppData(query->value(0).toString(), query->value(1).toString()));
    }
    return data;
}
