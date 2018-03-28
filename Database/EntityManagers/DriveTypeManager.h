#ifndef DRIVETYPEMANAGER_H
#define DRIVETYPEMANAGER_H

#include <Database/Entities/DriveType.h>
#include <Database/EntityManagers/AbstractManager.h>
#include <QMap>

class DriveTypeManager: public AbstractManager
{
public:
    DriveTypeManager();
    DriveType* add(DriveType * type);
    bool remove(DriveType * type);
    DriveType* update(DriveType * type);
    void initDbTable();

    // Different finders
    static DriveType * getOneBy(QMap<QString, QString>);
    QList<DriveType *> *getBy(QMap<QString, QString> map);
    QList<DriveType *> *getAll();

};

#endif // DRIVETYPEMANAGER_H
