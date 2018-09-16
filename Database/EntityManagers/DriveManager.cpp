#include "DriveManager.h"

DriveManager::DriveManager() : AbstractManager()
{
    //driveTypeManager = new DriveTypeManager();
}

Drive * DriveManager::add(Drive * drive)
{
    QString request = "insert into Drive(id, absolutepath, driveType_id, default_drive) select NULL, '"+drive->getAbsolutepath()+"', "+QString::number(drive->getType()->getId())+", "+((drive->isDefaultDrive()) ? "1" : "0")+" where not exists (select 1 from Drive where absolutepath = '"+drive->getAbsolutepath()+"')";
    qDebug() << request << endl;
    QSqlQuery sqlQuery(Parameters::localDb);
    sqlQuery.exec(request);
    if (sqlQuery.isActive()){
        qDebug() << "Failed to insert a drive in the db !" << sqlQuery.lastError().text() << endl;
        return NULL;
    }

    QMap<QString, QString> driveProperties;
    driveProperties.insert("absolutepath", drive->getAbsolutepath());
    drive = getOneBy(driveProperties);
    return drive;
}

bool DriveManager::remove(Drive * drive)
{
    QString request = "delete from Drive where id = "+QString::number(drive->getId());
    qDebug() << request << endl;
    QSqlQuery sqlQuery(Parameters::localDb);
    sqlQuery.exec(request);
    if (sqlQuery.isActive()){
        qDebug() << "Failed to delete a drive in the db !" << endl;
        return false;
    }
    return true;
}

Drive * DriveManager::update(Drive * drive)
{
    QString request = "update Drive set absolutepath = '"+drive->getAbsolutepath()+"' and driveType_id = "+QString::number(drive->getType()->getId());
    qDebug() << request << endl;
    QSqlQuery sqlQuery(Parameters::localDb);
    sqlQuery.exec(request);
    if (sqlQuery.isActive()){
        qDebug() << "Failed to update a drive in the db !" << endl;
        return NULL;
    }

    QMap<QString, QString> driveProperties;
    driveProperties.insert("absolutepath", drive->getAbsolutepath());
    drive = getOneBy(driveProperties);
    return drive;
}

QList<Drive *> *DriveManager::getAll()
{
    //Building the sql request
    QString request = "select * from Drive";

    QSqlQuery sqlQuery(Parameters::localDb);
    sqlQuery.exec(request);
    if (!sqlQuery.isActive()){
        qDebug() << "Failed to select the drive object ! " << endl;
        return NULL;
    }

    QList<Drive*> * drives = new QList<Drive *>();
    while (sqlQuery.next()){
        QMap<QString, QString> properties;
        properties.insert("id", sqlQuery.value(2).toString());
        drives->append(Functions::fromSqlRecord2Drive(sqlQuery.record()));
    }
    return drives;
}

Drive *DriveManager::getOneBy(QMap<QString, QString> properties)
{
    //Building the sql request
    QString request = "select * from Drive where ";

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
        qDebug() << "Failed to select the drive object ! " << sqlQuery.lastError() << endl;
        return NULL;
    }

    if (sqlQuery.next()){
        return Functions::fromSqlRecord2Drive(sqlQuery.record());
    }
    return NULL;
}

Drive *DriveManager::getDrive(QFileInfo fileInfo)
{
    QMap<QString, QString> properties;
    properties.insert("absolutepath", Functions::getDriveAbsPath(fileInfo.absoluteFilePath()));
    Drive *drive = getOneBy(properties);
//    if (drive == NULL){
//        QMap<QString, QString> driveTypeProperties;
//        driveTypeProperties.insert("name", DriveType::COMPUTER);
//        return add(new Drive(0, Functions::getDriveAbsPath(fileInfo.absoluteFilePath()), DriveTypeManager::getOneBy(driveTypeProperties)));
//    }
//    qDebug() << drive->toString() << endl;
    return drive;
}

void DriveManager::displayAll()
{
    QList<Drive *> * drives = getAll();
    int size  = drives->length();
    for (int i(0); i < size; i++){
        Drive *drive = drives->at(i);
        qDebug() << drive->toString() << endl;
    }
}

Drive *DriveManager::getDefaultDrive()
{
    QMap<QString, QString> properties;
    properties.insert("default_drive", "1");
    return getOneBy(properties);
}
