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

    static AppData * getByKey(QString);

    const static QString STORAGE_DIR_KEY;
};

#endif // APPDATAMANAGER_H
