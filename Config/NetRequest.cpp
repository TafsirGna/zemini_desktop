#include "NetRequest.h"

QNetworkRequest NetRequest::getNetworkRequest() const
{
    return networkRequest;
}

void NetRequest::setNetworkRequest(const QNetworkRequest &value)
{
    networkRequest = value;
}

QString NetRequest::getType() const
{
    return type;
}

void NetRequest::setType(const QString &value)
{
    type = value;
}

QByteArray NetRequest::getData() const
{
    return data;
}

void NetRequest::setData(const QByteArray &value)
{
    data = value;
}

int NetRequest::getCode() const
{
    return code;
}

void NetRequest::setCode(int value)
{
    code = value;
}

NetRequest::NetRequest()
{

}

NetRequest::NetRequest(int code, QString type, QNetworkRequest request)
{
    this->code = code;
    networkRequest = request;
    this->type = type;
}

NetRequest::NetRequest(int code, QString type, QNetworkRequest request, QByteArray data)
{
    this->code = code;
    networkRequest = request;
    this->type = type;
    this->data = data;
}
