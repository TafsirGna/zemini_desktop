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

    QString typeCreateQuery = "CREATE TABLE IF NOT EXISTS Type("
                              "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                              "name TEXT NOT NULL,"
                              "suffix TEXT UNIQUE"
                              ");";

    QString fileCreateQuery = "CREATE TABLE IF NOT EXISTS File("
                              "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                              "iddir INT NULL,"
                              "idtype TEXT NULL,"
                              "idcategory TEXT NULL,"
                              "createdat TEXT,"
                              "updatedat TEXT,"
                              "filename TEXT,"
                              "path TEXT,"
                              "size TEXT,"
                              "status INT,"
                              "thumbnail TEXT NULL,"
                              "CONSTRAINT fk_file_id_type FOREIGN KEY(idtype) REFERENCES Type(id),"
                              "CONSTRAINT fk_file_id_file FOREIGN KEY(iddir) REFERENCES Type(id) on delete cascade,"
                              "CONSTRAINT fk_file_id_category FOREIGN KEY(idcategory) REFERENCES Category(id)"
                              ");";

    QString appDataCreateQuery = "CREATE TABLE IF NOT EXISTS Appdata("
                                 "key TEXT PRIMARY KEY,"
                                 "value TEXT NOT NULL"
                                 ");";

    query.exec(userCreateQuery);
    if(!query.isActive())
        qDebug()<<"An error occured while creating table user: "+query.lastError().text();

    query.exec(categoryCreateQuery);
    if(!query.isActive())
        qDebug()<<"An error occured while creating table Category : "+query.lastError().text();

    query.exec(typeCreateQuery);
    if(!query.isActive())
        qDebug()<<"An error occured while creating table Type: "+query.lastError().text();

    query.exec(fileCreateQuery);
    if(!query.isActive())
        qDebug()<<"An error occured while creating table File: "+query.lastError().text();

    query.exec(appDataCreateQuery);
    if(!query.isActive())
        qDebug()<<"An error occured while creating table Appdata: "+query.lastError().text();
}

