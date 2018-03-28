#include "DriveTypeManager.h"

DriveType * DriveTypeManager::add(DriveType *type)
{
    QString request = "insert into Drive_type(id, name) select NULL, '"+type->getName()+"' where not exists (select 1 from Drive_type where name = '"+type->getName()+"')";
    //qDebug() << request << endl;
    QSqlQuery sqlQuery(Parameters::localDb);
    sqlQuery.exec(request);
    if (!sqlQuery.isActive()){
        qDebug() << "Failed to insert a drive_type in the db !" << endl;
        return NULL;
    }

    QMap<QString, QString> driveTypeProperties;
    driveTypeProperties.insert("name", type->getName());
    return getOneBy(driveTypeProperties);
}

bool DriveTypeManager::remove(DriveType *type)
{
    QString request = "delete from Drive_type where id = "+QString::number(type->getId());
    qDebug() << request << endl;
    QSqlQuery sqlQuery(Parameters::localDb);
    sqlQuery.exec(request);
    if (!sqlQuery.isActive()){
        qDebug() << "Failed to delete a drive_type in the db !" << endl;
        return false;
    }
    return true;
}

DriveType* DriveTypeManager::update(DriveType *type)
{
    QString request = "update Drive_type set name = '"+type->getName()+"' where id = "+QString::number(type->getId());
    qDebug() << request << endl;
    QSqlQuery sqlQuery(Parameters::localDb);
    sqlQuery.exec(request);
    if (!sqlQuery.isActive()){
        qDebug() << "Failed to update a drive_type in the db !" << endl;
        return NULL;
    }
    return type;
}

void DriveTypeManager::initDbTable()
{
    this->add(new DriveType(0, "Computer"));
    this->add(new DriveType(0, "Smartphone"));
    this->add(new DriveType(0, "External hard drive"));
}

DriveTypeManager::DriveTypeManager(): AbstractManager()
{

}

DriveType *DriveTypeManager::getOneBy(QMap<QString, QString> properties)
{
    //Building the sql request
    QString request = "select * from Drive_type where ";

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
        qDebug() << "Failed to select the drive type object ! " << endl;
        return NULL;
    }

    if (sqlQuery.next()){
        return new DriveType(sqlQuery.value(0).toInt(), sqlQuery.value(1).toString());
    }
    return NULL;
}

QList<DriveType *> *DriveTypeManager::getBy(QMap<QString, QString> properties)
{
    //Building the sql request
    QString request = "select * from Drive_type where ";

    QMapIterator<QString, QString> i(properties);
    int j = 0;
    while(i.hasNext()){
        i.next();
        request += ((j != 0) ? "and" : "") + i.key()+" = '"+i.value()+"'";
        j++;
    }

    QSqlQuery sqlQuery(Parameters::localDb);
    sqlQuery.exec(request);
    if (!sqlQuery.isActive()){
        qDebug() << "Failed to select the drive type object ! " << endl;
        return NULL;
    }

    QList<DriveType*> * driveTypes = new QList<DriveType *>();
    while (sqlQuery.next()){
        driveTypes->append(new DriveType(sqlQuery.value(0).toInt(), sqlQuery.value(1).toString()));
    }
    return driveTypes;
}

QList<DriveType *> *DriveTypeManager::getAll()
{
    //Building the sql request
    QString request = "select * from Drive_type";

    QSqlQuery sqlQuery(Parameters::localDb);
    sqlQuery.exec(request);
    if (!sqlQuery.isActive()){
        qDebug() << "Failed to select the drive type object ! " << endl;
        return NULL;
    }

    QList<DriveType*> * driveTypes = new QList<DriveType *>();
    while (sqlQuery.next()){
        driveTypes->append(new DriveType(sqlQuery.value(0).toInt(), sqlQuery.value(1).toString()));
    }
    return driveTypes;
}
