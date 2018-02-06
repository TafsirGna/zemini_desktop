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
        QSqlQuery * query;

        //  Functions
        void saveUpdateToRemoteDB();
public:
    //Constructors
    AbstractManager();
    AbstractManager(const AbstractManager & absManager);
    AbstractManager & operator =(const AbstractManager & absManager);

};

#endif // ABSTRACTMANAGER_H
