#include "AppData.h"

QString AppData::getValue() const
{
    return value;
}

void AppData::setValue(const QString &value)
{
    this->value = value;
}

QString AppData::getKey() const
{
    return key;
}

void AppData::setKey(const QString &value)
{
    key = value;
}

AppData::AppData()
{
}

AppData::AppData(QString key, QString value)
{
    this->key = key;
    this->value = value;
}
