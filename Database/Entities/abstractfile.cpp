#include "abstractfile.h"

/***        Builders        ***/
AbstractFile::AbstractFile()
{

}

AbstractFile::AbstractFile(QString name, QDateTime datecreation, int size, QString path, int status, Type *type)
{
    this->name = name;
    this->datecreation = datecreation;
    this->size = size;
    this->path = path;
    this->status = status;
    this->type = type;
}

AbstractFile::AbstractFile(int id, QString name, QDateTime datecreation, int size, QString path, int status, Type *type)
{
    this->id = id;
    this->name = name;
    this->datecreation = datecreation;
    this->size = size;
    this->path = path;
    this->status = status;
    this->type = type;
}

/***        Getters     ***/
int AbstractFile::getId()
{
    return this->id;
}

QString AbstractFile::getName()
{
    return this->name;
}

QDateTime AbstractFile::getDatecreation()
{
    return this->datecreation;
}

int AbstractFile::getSize()
{
    return this->size;
}

QString AbstractFile::getPath()
{
    return this->path;
}

int AbstractFile::getStatus()
{
    return this->status;
}

Type * AbstractFile::getType()
{
    return this->type;
}

/***        Setters     ***/
void AbstractFile::setId(int id)
{
    this->id = id;
}

void AbstractFile::setName(QString name)
{
    this->name = name;
}

void AbstractFile::setDatecreation(QDateTime datecreation)
{
    this->datecreation = datecreation;
}

void AbstractFile::setSize(int size)
{
    this->size = size;
}

void AbstractFile::setStatus(int status)
{
    this->status = status;
}

void AbstractFile::setPath(QString path)
{
    this->path = path;
}

void AbstractFile::setType(Type * type)
{
    this->type = type;
}
