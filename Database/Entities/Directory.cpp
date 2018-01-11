#include "Database\Entities\Directory.h"

/***        builders        ***/
Directory::Directory()
{

}

Directory::Directory(QString name, QDateTime datecreation, int size, QString path, int status, Type * type, Directory *directory) : AbstractFile(name,datecreation,size,path,status,type)
{
    this->directory = directory;
}

Directory::Directory(int id,QString name, QDateTime datecreation, int size, QString path, int status,Type * type, Directory * directory) : AbstractFile(id,name,datecreation,size,path,status,type)
{
    this->directory = directory;
}

Directory::Directory(const Directory & directory)
{
    this->id = directory.id;
    this->name = directory.name;
    this->datecreation = directory.datecreation;
    this->size = directory.size;
    this->path = directory.path;
    this->status = directory.status;
    this->type = directory.type;
    this->directory = directory.directory;
}

Directory & Directory::operator=(const Directory & directory)
{
    this->id = directory.id;
    this->name = directory.name;
    this->datecreation = directory.datecreation;
    this->size = directory.size;
    this->path = directory.path;
    this->status = directory.status;
    this->type = directory.type;
    this->directory = directory.directory;
    return *this;
}

void Directory::toString()
{
    qDebug() << "---    Directory::toString";
    qDebug() << "id : "+ QString::number(id) + " - name : "+ name + " - path : "+ path + " - datecreation : " + datecreation.toString() + " - status : "+ QString::number(status) + " - size : " + QString::number(size) + " - idDirectory : " + ((directory == NULL) ? QString::number(0) : QString::number(directory->getId())) + " - idType : " + ((type == NULL) ? QString::number(0) : QString::number(type->getId())) ;
}

void Directory::setDirectory(Directory *directory)
{
    this->directory = directory;
}

Directory * Directory::getDirectory()
{
    return this->directory;
}
