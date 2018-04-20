#include "FileFormat.h"

int FileFormat::getId() const
{
    return id;
}

void FileFormat::setId(int value)
{
    id = value;
}

QString FileFormat::getName() const
{
    return name;
}

void FileFormat::setName(const QString &value)
{
    name = value;
}

FileFormat::FileFormat()
{
    id = 0;
    name = "";
}

FileFormat::FileFormat(int id, QString name)
{
    this->id = id;
    this->name = name;
}
