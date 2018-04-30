#ifndef NETREQUEST_H
#define NETREQUEST_H

#include "QNetworkRequest"

class NetRequest{
private:
    int code;
    QNetworkRequest networkRequest;
    QString type;
    QByteArray data;

public:
    NetRequest();
    NetRequest(int, QString, QNetworkRequest);
    NetRequest(int, QString, QNetworkRequest, QByteArray);
    QNetworkRequest getNetworkRequest() const;
    void setNetworkRequest(const QNetworkRequest &value);
    QString getType() const;
    void setType(const QString &value);
    QByteArray getData() const;
    void setData(const QByteArray &value);
    int getCode() const;
    void setCode(int value);
};

#endif // NETREQUEST_H
