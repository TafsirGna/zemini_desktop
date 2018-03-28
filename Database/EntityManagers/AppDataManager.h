#ifndef APPDATAMANAGER_H
#define APPDATAMANAGER_H

#include <Database/Entities/AppData.h>
#include <Database/EntityManagers/AbstractManager.h>

class AppDataManager: public AbstractManager
{
public:
    AppDataManager();

    AppData *add(AppData*);
    AppData * update(AppData*);
    bool remove(AppData*);
    QList<AppData *> * getAll();

    AppData * getByKey(QString);
};

#endif // APPDATAMANAGER_H
