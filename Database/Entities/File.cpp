#include "Database\Entities\File.h"

/***            Default constructor         ***/
File::File() : AbstractFile()
{
    //Nothing to do for now
}

/***            A second constructor            ***/
File::File(QString name, QDateTime datecreation, int size, QString path, int status, Type *type, QString suffix, Directory *directory) : AbstractFile(name,datecreation,size,path,status, type)
{
    this->suffix = suffix;
    this->directory = directory;
}

/***            A second constructor            ***/
File::File(int id, QString name, QDateTime datecreation, int size, QString path, int status, Type *type, QString suffix, Directory * directory) : AbstractFile(id,name,datecreation,size,path,status, type)
{
    this->suffix = suffix;
    this->directory = directory;
}

File::File(const File &file)
{
    this->id = file.id;
    this->name = file.name;
    this->suffix = file.suffix;
    this->path = file.path;
    this->datecreation = file.datecreation;
    this->status = status;
    this->type = file.type;
    this->size = file.size;
    this->directory = file.directory;
}

File & File::operator =(const File & file)
{
    this->id = file.id;
    this->name = file.name;
    this->suffix = file.suffix;
    this->path = file.path;
    this->datecreation = file.datecreation;
    this->status = status;
    this->type = file.type;
    this->size = file.size;
    this->directory = file.directory;
    return *this;
}

/***            Getters         ***/
QString File::getSuffix()
{
    return this->suffix;
}

Directory * File::getDirectory()
{
    return this->directory;
}

/***            Setters         ***/
void File::setSuffix(QString suffix)
{
    this->suffix = suffix;
}

void File::setDirectory(Directory *directory)
{
    this->directory = directory;
}

void File::toString()
{
    qDebug() << "---    File::toString ";
    qDebug() << "id : "+ QString::number(id) + " - name : "+ name + " - suffix : "+ suffix + " - path : "+ path + " - datecreation : " + datecreation.toString() + " - status : "+ QString::number(status) + " - size : " + QString::number(size) + " - iddirectory : "+ QString::number(directory->getId()) + " - idtype : " + QString::number(type->getId()) ;
}


