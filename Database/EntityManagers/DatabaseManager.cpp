#include "databasemanager.h"

DatabaseManager::DatabaseManager(QObject *parent) :
    QObject(parent)
{

}


QSqlDatabase DatabaseManager::connectToDatabase(const QString &dbName, const QString &userName,  const QString &hostName, int port, const QString &pwd,
                                                const QString &type, const QString &conName)
{
    QSqlDatabase db=QSqlDatabase::addDatabase(type, conName);
    db.setDatabaseName(dbName);
    db.setUserName(userName);
    db.setHostName(hostName);
    db.setPassword(pwd);
    db.setPort(port);

    if(db.open())
    {
        qDebug()<<"Base de données connectée avec succès ...";
        initializeDatabase(conName);
    }
    else
    {
        db.close();
        qDebug()<<"Erreur de connection à la BD: "<<db.lastError().text();
    }
    return db;
}


void DatabaseManager::initializeDatabase(const QString &conName)
{
    QSqlDatabase db=QSqlDatabase::database(conName);
    QSqlQuery query(db);

    query.exec("CREATE TABLE IF NOT EXISTS User("
               "id INT NOT NULL UNIQUE,idlocation TEXT,"
               "activated INT, activationkey TEXT,"
               "dateexit TEXT,dateinscription TEXT,"
               "email TEXT, familyname TEXT,"
               "firstname TEXT,"
               "password TEXT,phonenumber TEXT,"
               "roles TEXT,salt TEXT,username TEXT,"
               "CONSTRAINT pk_user PRIMARY KEY(id)"
               ")");

    if(query.isActive())
    {
        qDebug()<<"Table User created successfully ...";
        //query.exec("INSERT INTO Etudiants VALUES(1, 'COCOU', 'Jules', 'PC')");
        //if(!query.isActive())
        //    qDebug()<<"Erreur insertion Etudiant: " + query.lastError().text();
    }
    else
        qDebug()<<"An error occured while creating table user: "+query.lastError().text();

    query.exec("CREATE TABLE IF NOT EXISTS Category("
               "id INT NOT NULL UNIQUE,"
               "name TEXT NOT NULL UNIQUE,"
               "CONSTRAINT pk_category PRIMARY KEY(id)"
               ")");

    if(query.isActive())
    {
        qDebug()<<"Table Category created successfully ...";
    }
    else
        qDebug()<<"An error occured while creating table Category : "+query.lastError().text();

    query.exec("CREATE TABLE IF NOT EXISTS Type("
               "id INT NOT NULL UNIQUE,"
               "idcategory INT NOT NULL,"
               "name TEXT NOT NULL, "
               "CONSTRAINT pk_type PRIMARY KEY(id),"
               "CONSTRAINT fk_type_idcat_cat FOREIGN KEY(idcategory) REFERENCES Category(id)"
               ")");

    if(query.isActive())
    {
        qDebug()<<"Table Type created successfully ...";
    }
    else
        qDebug()<<"An error occured while creating table Type: "+query.lastError().text();


    query.exec("CREATE TABLE IF NOT EXISTS Directory("
               "id INT NOT NULL UNIQUE,"
               "iddirectory INT NULL,"
               "idtype INT,"
               "datecreation TEXT,"
               "name TEXT,"
               "size TEXT,"
               "status INT,"
               "path TEXT UNIQUE, "
               "CONSTRAINT pk_directory PRIMARY KEY(id),"
               "CONSTRAINT fk_dir_id_type FOREIGN KEY(idtype) REFERENCES Type(id)"
               ")");

    if(query.isActive())
    {
        qDebug()<<"Table File created successfully ...";
    }
    else
        qDebug()<<"An error occured while creating table directory: "+query.lastError().text();

    query.exec("CREATE TABLE IF NOT EXISTS File("
               "id INT NOT NULL UNIQUE,"
               "iddirectory INT,"
               "idtype TEXT, "
               "datecreation TEXT,"
               "filename TEXT,"
               "path TEXT, "
               "size TEXT,"
               "status INT,"
               "suffix TEXT,"
               "CONSTRAINT pk_file PRIMARY KEY(id),"
               //"CONSTRAINT fk_file_id_directory FOREIGN KEY(iddirectory) REFERENCES directory(id),"
               "CONSTRAINT fk_file_id_type FOREIGN KEY(idtype) REFERENCES Type(id)"
               ")");

    if(query.isActive())
    {
        qDebug()<<"Table File created successfully ...";
    }
    else
        qDebug()<<"An error occured while creating table content: "+query.lastError().text();


    // --- i continue with the insertions into the database for getting started with zemini pc client
    // --- table category
    query.exec("INSERT INTO Category VALUES(1, 'Movies')");
    if(!query.isActive())
        qDebug()<<"An error occured while creating table Category: "+query.lastError().text();

    query.exec("INSERT INTO Category VALUES(2, 'Series')");
    if(!query.isActive())
        qDebug()<<"An error occured while creating table Category: "+query.lastError().text();

    query.exec("INSERT INTO Category VALUES(3, 'Softwares')");
    if(!query.isActive())
        qDebug()<<"An error occured while creating table Category: "+query.lastError().text();

    query.exec("INSERT INTO Category VALUES(4, 'Songs')");
    if(!query.isActive())
        qDebug()<<"An error occured while creating table Category: "+query.lastError().text();

    query.exec("INSERT INTO Category VALUES(5, 'Books')");
    if(!query.isActive())
        qDebug()<<"An error occured while creating table Category: "+query.lastError().text();

    query.exec("INSERT INTO Category VALUES(6, 'Others')");
    if(!query.isActive())
        qDebug()<<"An error occured while creating table Category: "+query.lastError().text();


    // --- table type
    query.exec("INSERT INTO Type VALUES(1,1,'None')");
    if(!query.isActive())
        qDebug()<<"An error occured while creating table Type: "+query.lastError().text();

    query.exec("INSERT INTO Type VALUES(2,2,'None')");
    if(!query.isActive())
        qDebug()<<"An error occured while creating table Type: "+query.lastError().text();

    query.exec("INSERT INTO Type VALUES(3,3, 'None')");
    if(!query.isActive())
        qDebug()<<"An error occured while creating table Type: "+query.lastError().text();

    query.exec("INSERT INTO Type VALUES(4,4, 'None')");
    if(!query.isActive())
        qDebug()<<"An error occured while creating table Type: "+query.lastError().text();

    query.exec("INSERT INTO Type VALUES(5,5, 'None')");
    if(!query.isActive())
        qDebug()<<"An error occured while creating table Type: "+query.lastError().text();

    query.exec("INSERT INTO Type VALUES(6,6, 'None')");
    if(!query.isActive())
        qDebug()<<"An error occured while creating table Type: "+query.lastError().text();

}

