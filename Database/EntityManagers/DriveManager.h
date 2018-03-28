#ifndef DRIVEMANAGER_H
#define DRIVEMANAGER_H

#include <Database/Entities/Drive.h>
#include <Database/EntityManagers/AbstractManager.h>
#include <Database/EntityManagers/DriveTypeManager.h>
#include <Config/Functions.h>

class DriveManager: public AbstractManager
{

private:
    //DriveTypeManager * driveTypeManager;

public:
    DriveManager();

    static Drive *add(Drive *);
    static bool remove(Drive *);
    static Drive *update(Drive *);

    // Getters
    static QList<Drive *> *getAll();
    static Drive *getOneBy(QMap<QString, QString>);
    static Drive *getDrive(QFileInfo);
    static QList<Drive *> *getBy(QMap<QString, QString>);
    static void displayAll();
};

#endif // DRIVEMANAGER_H
