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
                                "id INT PRIMARY KEY NOT NULL,"
                                "familyname TEXT,"
                                "firstname TEXT,"
                                "email TEXT,"
                                "username TEXT,"
                                "password TEXT,"
                                "activated INT"
                                ");";

    QString categoryCreateQuery = "CREATE TABLE IF NOT EXISTS Category("
                                  "id INT PRIMARY KEY NOT NULL,"
                                  "name TEXT NOT NULL UNIQUE"
                                  ");";

    QString typeCreateQuery = "CREATE TABLE IF NOT EXISTS Type("
                              "id INT NOT NULL UNIQUE,"
                              "name TEXT NOT NULL,"
                              "suffix TEXT,"
                              "CONSTRAINT pk_type PRIMARY KEY(id)"
                              ");";

    QString fileCreateQuery = "CREATE TABLE IF NOT EXISTS File("
                              "id INT NOT NULL UNIQUE,"
                              "iddir INT NULL,"
                              "idtype TEXT NULL,"
                              "idcategory TEXT NULL"
                              "creationtime TEXT,"
                              "updatetime TEXT,"
                              "filename TEXT,"
                              "path TEXT,"
                              "size TEXT,"
                              "saved INT,"
                              "CONSTRAINT pk_file PRIMARY KEY(id),"
                              "CONSTRAINT fk_file_id_type FOREIGN KEY(idtype) REFERENCES Type(id)"
                              "CONSTRAINT fk_file_id_file FOREIGN KEY(iddir) REFERENCES Type(id)"
                              "CONSTRAINT fk_file_id_category FOREIGN KEY(idcategory) REFERENCES Category(id)"
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
        qDebug()<<"An error occured while creating table content: "+query.lastError().text();
}

