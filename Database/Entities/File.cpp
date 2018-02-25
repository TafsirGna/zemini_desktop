#include "Database\Entities\File.h"

const int File::FILE_SAVED = 1;
const int File::FILE_NOT_SAVED = 0;

QFileInfo *File::getThumbnail() const
{
    return thumbnail;
}

void File::setThumbnail(QFileInfo *value)
{
    thumbnail = value;
}

File::File()
{
    //Nothing to do for now
}

/***            A second constructor            ***/
File::File(int id, QString fileName, QString path, QDateTime createdAt, QDateTime updatedAt, int size, bool saved, QFileInfo *thumbnail, Type *type, Category *category, File * folder)
{
    this->id = id;
    this->fileName = fileName;
    this->path = path;
    this->size = size;
    this->saved = saved;
    this->category = category;
    this->folder = folder;
    this->createdAt = createdAt;
    this->updatedAt = updatedAt;
    this->type = type;
    this->thumbnail = thumbnail;
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
    this->createdAt = file.createdAt;
    this->saved = file.saved;
    this->type = file.type;
    this->size = file.size;
    this->folder = file.folder;
    this->updatedAt = file.updatedAt;
    this->category = file.category;
    this->thumbnail = file.thumbnail;
    return *this;
}

/**
 * @brief File::getId
 * @return
 */
int File::getId() const
{
    return id;
}

/**
 * @brief File::getFileName
 * @return
 */
QString File::getFileName() const
{
    return fileName;
}

/**
 * @brief File::getPath
 * @return
 */
QString File::getPath() const
{
    return path;
}

/**
 * @brief File::getCreationTime
 * @return
 */
QDateTime File::getCreatedAt() const
{
    return createdAt;
}

/**
 * @brief File::getUpdateTime
 * @return
 */
QDateTime File::getUpdatedAt() const
{
    return updatedAt;
}

/**
 * @brief File::getSize
 * @return
 */
int File::getSize() const
{
    return size;
}

/**
 * @brief File::getCategory
 * @return
 */
Category * File::getCategory() const
{
    return category;
}

/**
 * @brief File::getType
 * @return
 */
Type * File::getType() const
{
    return type;
}

/**
 * @brief File::getFolder
 * @return
 */
File * File::getFolder() const
{
    return folder;
}

/**
 * @brief File::isSaved
 * @return
 */
bool File::isSaved() const
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
void File::setUpdatedAt(QDateTime updatedAt)
{
    this->updatedAt = updatedAt;
}

/**
 * @brief File::setCreationTime
 * @param creationTime
 */
void File::setCreatedAt(QDateTime createdAt)
{
    this->createdAt = createdAt;
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

/**
 * @brief File::serialize
 * @return
 */
QString File::serialize()
{
    QString res = fileName+Parameters::NET_REQUEST_SEPARATOR
            +path.replace("/", "+")+Parameters::NET_REQUEST_SEPARATOR
            +QString::number(size)+Parameters::NET_REQUEST_SEPARATOR
            +createdAt.toString(Parameters::timeFormat)+Parameters::NET_REQUEST_SEPARATOR
            +updatedAt.toString(Parameters::timeFormat);
    return res;
}


