#include "databasemanager.h"

DatabaseManager::DatabaseManager(QObject *parent) :
    QObject(parent)
{

}


QSqlDatabase DatabaseManager::connectToDatabase(const QString &dbName, const QString &userName,  const QString &hostName, int port, const QString &pwd,
                                                const QString &type, const QString &conName)
{
    QSqlDatabase db = QSqlDatabase::addDatabase(type, conName);
    db.setDatabaseName(dbName);
    db.setUserName(userName);
    db.setHostName(hostName);
    db.setPassword(pwd);
    db.setPort(port);

    if(db.open())
    {
        qDebug()<<"Successfull connection to the database ..." << endl;
        DatabaseManager::createTables(conName);
        return db;
    }
    else
    {
        db.close();
        qDebug()<<"Error when connecting to the database: "<< db.lastError().text() << endl;
        //return NULL;
    }
}


void DatabaseManager::createTables(const QString &conName)
{
    QSqlDatabase db=QSqlDatabase::database(conName);
    QSqlQuery query(db);

    QString userCreateQuery = "CREATE TABLE IF NOT EXISTS User("
                                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                "familyname TEXT,"
                                "firstname TEXT,"
                                "email TEXT,"
                                "username TEXT,"
                                "password TEXT,"
                                "activated INT"
                                ");";

    QString categoryCreateQuery = "CREATE TABLE IF NOT EXISTS Category("
                                  "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                  "name TEXT NOT NULL UNIQUE"
                                  ");";

    QString driveTypeCreateQuery = "CREATE TABLE IF NOT EXISTS Drive_type("
                                  "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                  "name TEXT NOT NULL UNIQUE"
                                  ");";

    QString driveCreateQuery = "CREATE TABLE IF NOT EXISTS Drive("
                                  "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                  "absolutepath TEXT NOT NULL UNIQUE,"
                                  "driveType_id INT,"
                                  "default_drive INT,"
                                  "CONSTRAINT fk_drive_id_drivetype FOREIGN KEY(driveType_id) REFERENCES Drive_type(id)"
                                  ");";

    QString formatCreateQuery = "CREATE TABLE IF NOT EXISTS File_format("
                              "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                              "name TEXT NOT NULL UNIQUE"
                              ");";

    QString typeCreateQuery = "CREATE TABLE IF NOT EXISTS File_type("
                              "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                              "name TEXT NOT NULL,"
                              "suffix TEXT UNIQUE,"
                              "format_id INT NULL,"
                              "CONSTRAINT fk_format_id_type FOREIGN KEY(format_id) REFERENCES File_format(id)"
                              ");";

    QString fileCreateQuery = "CREATE TABLE IF NOT EXISTS File("
                              "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                              "iddir INT NULL,"
                              "idtype TEXT NOT NULL,"
                              "idcategory TEXT NOT NULL,"
                              "drive_id TEXT NOT NULL,"
                              "createdat TEXT,"
                              "filename TEXT,"
                              "path TEXT,"
                              "size TEXT,"
                              "status INT,"
                              "thumbnail TEXT NULL UNIQUE,"
                              "length INT,"
                              "unique(filename, path) on conflict replace,"
                              "CONSTRAINT fk_file_id_type FOREIGN KEY(idtype) REFERENCES File_type(id),"
                              "CONSTRAINT fk_file_id_file FOREIGN KEY(iddir) REFERENCES Type(id) on delete cascade,"
                              "CONSTRAINT fk_file_id_category FOREIGN KEY(idcategory) REFERENCES Category(id),"
                              "CONSTRAINT fk_file_id_drive FOREIGN KEY(drive_id) REFERENCES Drive(id)"
                              ");";

    QString appDataCreateQuery = "CREATE TABLE IF NOT EXISTS Appdata("
                                 "key TEXT PRIMARY KEY,"
                                 "value TEXT NOT NULL"
                                 ");";

    query.exec(userCreateQuery);
    if(!query.isActive())
        qDebug()<<"An error occured while creating table user: "+query.lastError().text();

    query.exec(formatCreateQuery);
    if(!query.isActive())
        qDebug()<<"An error occured while creating table file_format: "+query.lastError().text();

    query.exec(categoryCreateQuery);
    if(!query.isActive())
        qDebug()<<"An error occured while creating table Category : "+query.lastError().text();

    query.exec(typeCreateQuery);
    if(!query.isActive())
        qDebug()<<"An error occured while creating table Type: "+query.lastError().text();

    query.exec(driveTypeCreateQuery);
    if(!query.isActive())
        qDebug()<<"An error occured while creating table DriveType: "+query.lastError().text();

    query.exec(driveCreateQuery);
    if(!query.isActive())
        qDebug()<<"An error occured while creating table Drive: "+query.lastError().text();

    query.exec(fileCreateQuery);
    if(!query.isActive())
        qDebug()<<"An error occured while creating table File: "+query.lastError().text();

    query.exec(appDataCreateQuery);
    if(!query.isActive())
        qDebug()<<"An error occured while creating table Appdata: "+query.lastError().text();
}

