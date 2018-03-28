#include "DriveType.h"

// initializing the static variables
const QString DriveType::COMPUTER = "Computer";
const QString DriveType::EX_HARD_DRIVE = "External hard drive";
const QString DriveType::SMARTPHONE = "Smartphone";

int DriveType::getId() const
{
    return id;
}

void DriveType::setId(int value)
{
    id = value;
}

QString DriveType::getName() const
{
    return name;
}

void DriveType::setName(const QString &value)
{
    name = value;
}

/**
 * @brief DriveType::serialize
 * @return
 */
QString DriveType::serialize()
{
    return name;
}

DriveType::DriveType()
{
    this->id = 0;
    this->name = "";
}

DriveType::DriveType(int id, QString name)
{
    this->id = id;
    this->name = name;
}
