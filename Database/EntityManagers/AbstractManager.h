#ifndef ABSTRACTMANAGER_H
#define ABSTRACTMANAGER_H

#include <QMessageBox>
#include <QObject>
#include <QNetworkReply>
#include <QWidget>
#include <QNetworkAccessManager>
#include "Config\Parameters.h"
#include <QDataStream>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>

class AbstractManager : public QObject
{
    Q_OBJECT
protected:
        //  Properties
        QSqlDatabase db;
        QSqlQuery * query;
        QString queryString;
        bool busy;

        //  Functions
        void saveUpdateToRemoteDB();
public:
    //Constructors
    AbstractManager();
    AbstractManager(QSqlDatabase &db);
    AbstractManager(const AbstractManager & absManager);
    AbstractManager & operator =(const AbstractManager & absManager);

    bool isBusy();

signals:
    void remoteUpdatesDone(int idmanager);
};

#endif // ABSTRACTMANAGER_H
