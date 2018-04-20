#include "Database\Entities\FileType.h"

//---   Builders
/**
 * @brief FileType::FileType
 */

FileFormat *FileType::getFileFormat() const
{
    return fileFormat;
}

void FileType::setFileFormat(FileFormat *value)
{
    fileFormat = value;
}

FileType::FileType()
{

}

/**
 * @brief FileType::FileType
 * @param id
 * @param name
 * @param suffix
 */
FileType::FileType(int id, QString name, QString suffix, FileFormat * fileFormat)
{
    this->id = id;
    this->name = name;
    this->suffix = suffix;
    this->fileFormat = fileFormat;
}

//---   Setters
/**
 * @brief FileType::setId
 * @param id
 */
void FileType::setId(int id)
{
    this->id = id;
}

/**
 * @brief FileType::setName
 * @param name
 */
void FileType::setName(QString name)
{
    this->name = name;
}

/**
 * @brief FileType::setSuffix
 * @param suffix
 */
void FileType::setSuffix(QString suffix)
{
    this->suffix = suffix;
}

//---   Getters
/**
 * @brief FileType::getId
 * @return
 */
int FileType::getId()
{
    return this->id;
}

/**
 * @brief FileType::getName
 * @return
 */
QString FileType::getName()
{
    return this->name;
}

/**
 * @brief FileType::getSuffix
 * @return
 */
QString FileType::getSuffix()
{
    return this->suffix;
}

//  Other functions

/**
 * @brief FileType::toString
 */
void FileType::toString()
{
    qDebug() << "id : " << id << " , name : " << name << " , suffix : " << suffix << endl;
}
