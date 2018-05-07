#include "AppDataManager.h"

const QString AppDataManager::STORAGE_DIR_KEY = "STORAGE_DIR";

AppDataManager::AppDataManager() : AbstractManager()
{

}

AppData * AppDataManager::add(AppData * data)
{
    QString request = "insert into Appdata(key, value) select '"+data->getKey()+"', '"+data->getValue()
            +"' where not exists (select 1 from Appdata where key = '"+data->getKey()+"')";
    //QString request = "insert into Appdata values ('"+data->getKey()+"', '"+data->getValue()+"');";
    qDebug() << request << endl;
    QSqlQuery sqlQuery(Parameters::localDb);
    sqlQuery.exec(request);
    if (!sqlQuery.isActive()){
        qDebug() << "Failed in inserting an app data ! " << sqlQuery.lastError().text() << endl;
        return NULL;
    }

    return data;
}

AppData * AppDataManager::update(AppData * data)
{
    QString request = "update Appdata set value = '"+data->getValue()+"' where key = '"+data->getKey()+"' ";
    QSqlQuery sqlQuery(Parameters::localDb);
    sqlQuery.exec(request);
    if (!sqlQuery.isActive()){
        qDebug() << "Failed in updating an app data ! " << endl;
        return NULL;
    }
    return data;
}

bool AppDataManager::remove(AppData *data)
{
    QString request = "delete from Appdata where key = '"+data->getKey()+"'";
    QSqlQuery sqlQuery(Parameters::localDb);
    sqlQuery.exec(request);
    if (!sqlQuery.isActive()){
        qDebug() << "Failed in deleting an app data ! " << endl;
        return false;
    }
    return true;
}

AppData *AppDataManager::getByKey(QString key)
{
    //qDebug() << "test1" << endl;
    QString request = "select key, value from Appdata where key = '"+key+"'";
    QSqlQuery sqlQuery(Parameters::localDb);
    sqlQuery.exec(request);
    if (!sqlQuery.isActive()){
        qDebug() << "Failed in selecting an app data ! " << sqlQuery.lastError().text() << endl;
        return NULL;
    }

    //qDebug() << "test2 : " << query->size() << endl;
    if (sqlQuery.next()){
        return new AppData(sqlQuery.value(0).toString(), sqlQuery.value(1).toString());
    }
    return NULL;
}

QList<AppData *> * AppDataManager::getAll()
{
    QString request = "select key, value from Appdata";
    QSqlQuery sqlQuery(Parameters::localDb);
    sqlQuery.exec(request);
    if (!sqlQuery.isActive()){
        qDebug() << "Failed to get all app's data ! " << sqlQuery.lastError().text() << endl;
        return NULL;
    }
    QList<AppData*> *data = new QList<AppData*>();
    while(sqlQuery.next()){
        data->append(new AppData(sqlQuery.value(0).toString(), sqlQuery.value(1).toString()));
    }
    return data;
}
