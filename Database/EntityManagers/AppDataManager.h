#ifndef APPDATAMANAGER_H
#define APPDATAMANAGER_H

#include <Database/Entities/AppData.h>
#include <Database/EntityManagers/AbstractManager.h>

class AppDataManager: public AbstractManager
{
public:
    AppDataManager();

    bool addAppData(AppData*);
    bool updateAppData(AppData*);
    bool deleteAppData(AppData*);
    QList<AppData *> * getAllData();

    AppData * getByKey(QString);
};

#endif // APPDATAMANAGER_H
