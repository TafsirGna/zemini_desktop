#include "Database\Entities\File.h"

File::File()
{
    //Nothing to do for now
}

/***            A second constructor            ***/
File::File(int id, QString fileName, QString path, QDateTime creationTime, QDateTime updateTime, int size, bool saved, Type *type, Category *category, File * folder)
{
    this->id = id;
    this->fileName = fileName;
    this->path = path;
    this->size = size;
    this->saved = saved;
    this->category = category;
    this->folder = folder;
    this->creationTime = creationTime;
    this->updateTime = updateTime;
    this->type = type;
}

File::File(const File &file)
{
    /*
    this->id = file.id;
    this->name = file.name;
    this->suffix = file.suffix;
    this->path = file.path;
    this->datecreation = file.datecreation;
    this->status = status;
    this->type = file.type;
    this->size = file.size;
    this->directory = file.directory;
    */
}

File & File::operator =(const File & file)
{
    this->id = file.id;
    this->fileName = file.fileName;
    this->path = file.path;
    this->creationTime = file.creationTime;
    this->saved = file.saved;
    this->type = file.type;
    this->size = file.size;
    this->folder = file.folder;
    this->updateTime = file.updateTime;
    this->category = file.category;
    return *this;
}

/**
 * @brief File::getId
 * @return
 */
int File::getId()
{
    return id;
}

/**
 * @brief File::getFileName
 * @return
 */
QString File::getFileName()
{
    return fileName;
}

/**
 * @brief File::getPath
 * @return
 */
QString File::getPath()
{
    return path;
}

/**
 * @brief File::getCreationTime
 * @return
 */
QDateTime File::getCreationTime()
{
    return creationTime;
}

/**
 * @brief File::getUpdateTime
 * @return
 */
QDateTime File::getUpdateTime()
{
    return updateTime;
}

/**
 * @brief File::getSize
 * @return
 */
int File::getSize()
{
    return size;
}

/**
 * @brief File::getCategory
 * @return
 */
Category * File::getCategory()
{
    return category;
}

/**
 * @brief File::getType
 * @return
 */
Type * File::getType()
{
    return type;
}

/**
 * @brief File::getFolder
 * @return
 */
File * File::getFolder()
{
    return folder;
}

/**
 * @brief File::isSaved
 * @return
 */
bool File::isSaved()
{
    return saved;
}

/**
 * @brief File::setId
 * @param id
 */
void File::setId(int id)
{
    this->id = id;
}

/**
 * @brief File::setCategory
 * @param category
 */
void File::setCategory(Category *category)
{
    this->category = category;
}

/**
 * @brief File::setType
 * @param type
 */
void File::setType(Type *type)
{
    this->type = type;
}

/**
 * @brief File::setFileName
 * @param fileName
 */
void File::setFileName(QString fileName)
{
    this->fileName = fileName;
}

/**
 * @brief File::setFolder
 * @param folder
 */
void File::setFolder(File *folder)
{
    this->folder = folder;
}

/**
 * @brief File::setUpdateTime
 * @param updateTime
 */
void File::setUpdateTime(QDateTime updateTime)
{
    this->updateTime = updateTime;
}

/**
 * @brief File::setCreationTime
 * @param creationTime
 */
void File::setCreationTime(QDateTime creationTime)
{
    this->creationTime = creationTime;
}

/**
 * @brief File::setSize
 * @param size
 */
void File::setSize(int size)
{
    this->size = size;
}

/**
 * @brief File::setSaved
 * @param saved
 */
void File::setSaved(bool saved)
{
    this->saved = saved;
}

void File::setPath(QString path)
{
    this->path = path;
}

void File::toString()
{
    qDebug() << "---    File::toString ";
    //qDebug() << "id : "+ QString::number(id) + " - name : "+ name + " - suffix : "+ suffix + " - path : "+ path + " - datecreation : " + datecreation.toString() + " - status : "+ QString::number(status) + " - size : " + QString::number(size) + " - iddirectory : "+ QString::number(directory->getId()) + " - idtype : " + QString::number(type->getId()) ;
}

File *File::convertToFile(QFileInfo fileInfo)
{
    //File * file(0, fileInfo.fileName(), fileInfo.absoluteFilePath(), fileInfo.created(), fileInfo.lastModified(), fileInfo.size(), 0, NULL, NULL, NULL);
    //return file;
    return NULL;
}


