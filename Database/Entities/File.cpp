#include "Database\Entities\File.h"

const int File::FILE_SAVED = 2;
const int File::FILE_UPDATED = 0;
const int File::FILE_DELETED = -1;
const int File::FILE_ADDED = 1;

QFileInfo *File::getThumbnail() const
{
    return thumbnail;
}

QString File::getAbsolutePath() const
{
    QString absPath = AppDataManager::getByKey(AppDataManager::STORAGE_DIR_KEY)->getValue();
    QStringList pathList = absPath.split("/");
    pathList.removeLast();

    return pathList.join("/")+path+Parameters::FILE_SYS_SEPARATOR+fileName;
}

bool File::isFile() const
{
    return false;
}

bool File::isDir() const
{
    return false;
}

void File::setThumbnail(QFileInfo *value)
{
    thumbnail = value;
}

bool File::getStatus() const
{
    return status;
}

void File::setStatus(bool value)
{
    status = value;
}

Drive *File::getDrive() const
{
    return drive;
}

void File::setDrive(Drive *value)
{
    drive = value;
}

int File::getLength() const
{
    return length;
}

void File::setLength(int value)
{
    length = value;
}

File::File()
{
    //Nothing to do for now
}

/***            A second constructor            ***/
File::File(int id, QString fileName, QString path, QDateTime createdAt,QDateTime addedAt, QDateTime updatedAt, int size, int status, QFileInfo *thumbnail, int length, FileType *type, Category *category, File * folder, Drive * drive)
{
    this->id = id;
    this->fileName = fileName;
    this->path = path;
    this->size = size;
    this->status = status;
    this->category = category;
    this->folder = folder;
    this->createdAt = createdAt;
    this->type = type;
    this->thumbnail = thumbnail;
    this->drive =  drive;
    this->length = length;
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
    this->status = file.status;
    this->type = file.type;
    this->size = file.size;
    this->folder = file.folder;
    this->category = file.category;
    this->thumbnail = file.thumbnail;
    this->drive =  file.drive;
    this->length = file.length;
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
FileType *File::getType() const
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
void File::setType(FileType *fileType)
{
    this->type = fileType;
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

void File::setPath(QString path)
{
    this->path = path;
}

QString File::toString()
{
    QString toString = "id : "+ QString::number(id) + " - filename : "+ fileName + " - path : "+ path + " - createdat : " + createdAt.toString(Parameters::timeFormat) + " - status : "+ QString::number(status) + " - size : " + QString::number(size) + " - parent folder : "+ ((folder != NULL) ? folder->getFileName() : "NULL") + " - type : " + ((type != NULL) ? type->getName() : "NULL" ) + " - category : " + ((category != NULL) ? category->getName() : "NULL") + " - drive : " + drive->getAbsolutepath()+
            " - thumbnail : "+((thumbnail == NULL) ? "NULL" : thumbnail->absolutePath());
    return toString;
}

/**
 * @brief File::serialize
 * @return
 */
QString File::serialize()
{
    QString res = drive->serialize()+Parameters::NET_REQUEST_SEPARATOR
            +QString::number(status)+Parameters::NET_REQUEST_SEPARATOR
            +QString::number(id)+Parameters::NET_REQUEST_SEPARATOR
            +fileName+Parameters::NET_REQUEST_SEPARATOR
            +path.replace("/", "+")+Parameters::NET_REQUEST_SEPARATOR
            +QString::number(size)+Parameters::NET_REQUEST_SEPARATOR
            +createdAt.toString(Parameters::timeFormat)+Parameters::NET_REQUEST_SEPARATOR
            +((folder != NULL) ? folder->getFileName() : "NULL")+Parameters::NET_REQUEST_SEPARATOR
            +((folder != NULL) ? folder->getPath().replace("/", "+") : "NULL")+Parameters::NET_REQUEST_SEPARATOR
            +type->getName()+Parameters::NET_REQUEST_SEPARATOR
            +((type == NULL || type->getName()== "directory") ? "NULL" : type->getSuffix())+Parameters::NET_REQUEST_SEPARATOR
            +category->getName();//+Parameters::NET_REQUEST_SEPARATOR+byteArray;
    return res;
}

void File::setRequestParams(QUrlQuery & params)
{
    drive->setRequestParams(params);
    params.addQueryItem("fileStatus", QString::number(status));
    params.addQueryItem("fileId", QString::number(id));
    params.addQueryItem("fileName", fileName);
    params.addQueryItem("filePath", path);
    params.addQueryItem("fileSize", QString::number(size));
    params.addQueryItem("fileCreatedAt", createdAt.toString(Parameters::timeFormat));
    params.addQueryItem("fileFolderName", ((folder != NULL) ? folder->getFileName() : "NULL"));
    params.addQueryItem("fileFolderPath", ((folder != NULL) ? folder->getPath() : "NULL"));
    params.addQueryItem("fileTypeName", type->getName());
    params.addQueryItem("fileTypeSuffix", ((type == NULL || type->getName()== "directory") ? "NULL" : type->getSuffix()));
    params.addQueryItem("fileCategoryName", category->getName());
}

int File::getDirSize(QDir rootDir)
{
    int dirSize = 0;
    QFileInfoList queue;
    QFileInfo currentNode(rootDir.absolutePath()) ;
    while(true){
        // if the root directory is a file then
        if (currentNode.isFile()){
            dirSize += currentNode.size();
        }
        else{
            //otherwise
            queue += QDir(currentNode.absoluteFilePath()).entryInfoList(QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot);
            //QMessageBox::information(0, "ok", QString::number(queue.size()));
        }
        if (queue.isEmpty())
            return dirSize;
        currentNode = queue.last();
        queue.removeLast();
    }
}

bool File::isMedia(QFileInfo)
{
    return false;
}


