#ifndef NETREQUEST_H
#define NETREQUEST_H

#include "QNetworkRequest"
#include <Services/LocalDbService.h>
#include <Database/EntityManagers/UserManager.h>

class NetRequest{
private:
    int code;
    QNetworkRequest *networkRequest;
    QString type;
    QByteArray data;
    QString entityClass;
    DbEntity * entity;

public:

    NetRequest();
    NetRequest(int, QString tableName);
    NetRequest(int, DbEntity *);
    NetRequest(int, QFileInfo);
    void exec();
    QString getType() const;
    void setType(const QString &value);
    QByteArray getData() const;
    void setData(const QByteArray &value);
    int getCode() const;
    void setCode(int value);
    DbEntity *getEntity() const;
    void setEntity(DbEntity *value);
    QString getEntityClass() const;
    void setEntityClass(const QString &value);
    QNetworkRequest *getNetworkRequest() const;
    void setNetworkRequest(QNetworkRequest *value);

    // static variables
    static QNetworkAccessManager * NetworkAccessManager;

};

#endif // NETREQUEST_H
