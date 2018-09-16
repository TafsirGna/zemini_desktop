#include "Drive.h"

int Drive::getId() const
{
    return id;
}

void Drive::setId(int value)
{
    id = value;
}

QString Drive::getAbsolutepath() const
{
    return absolutepath;
}

void Drive::setAbsolutepath(const QString &value)
{
    absolutepath = value;
}

DriveType *Drive::getType() const
{
    return type;
}

void Drive::setType(DriveType *value)
{
    type = value;
}

void Drive::setRequestParams(QUrlQuery & params)
{
    type->setRequestParams(params);
    params.addQueryItem("drivePath", absolutepath);
}

QString Drive::toString()
{
    return "id = "+QString::number(id)+", absolutepath = "+absolutepath+", type = "+type->getName();
}

QString Drive::serialize()
{
    return absolutepath+Parameters::NET_REQUEST_SEPARATOR+type->serialize();
}

bool Drive::isDefaultDrive() const
{
    return defaultDrive;
}

void Drive::setDefault(bool value)
{
    defaultDrive = value;
}

Drive::Drive()
{
    id = 0;
    absolutepath = "";
    type = new DriveType();
    defaultDrive = false;
}

Drive::Drive(int id , QString path, DriveType * type, bool defaultDrive)
{
    this->id = id;
    this->absolutepath = path;
    this->type = type;
    this->defaultDrive = defaultDrive;
}
