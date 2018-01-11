#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlTableModel>



class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseManager(QObject *parent = 0);

public:
    QSqlDatabase connectToDatabase(const QString &dbName, const QString &userName, const QString &hostName,  int port, const QString &pwd,
                          const QString &type, const QString &conName);
    void initializeDatabase(const QString &conName);
signals:

public slots:

};


#endif // DATABASEMANAGER_H
