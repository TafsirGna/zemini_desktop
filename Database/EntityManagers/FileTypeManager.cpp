#include "Database\EntityManagers\FileTypeManager.h"
#include <QException>

/**
 * @brief TypeManager::TypeManager
 */
FileTypeManager::FileTypeManager(): AbstractManager()
{

}

FileType *FileTypeManager::getType(QFileInfo fileInfo)
{
    if (fileInfo.isDir()){
        //printAll();
        QMap<QString, QString> fileTypeProperties;
        fileTypeProperties.insert("name", "directory");
        //qDebug() << "Oops " << (FileTypeManager::getOneBy(fileTypeProperties) == NULL) << endl;
        FileTypeManager::getOneBy(fileTypeProperties)->toString();
        return FileTypeManager::getOneBy(fileTypeProperties);
    }

    QMap<QString, QString> fileTypeProperties;
    fileTypeProperties.insert("suffix", fileInfo.suffix());
    FileType * type = FileTypeManager::getOneBy(fileTypeProperties);
    if (type == NULL)
        return add(new FileType(0, fileInfo.suffix(), fileInfo.suffix(), NULL)); //TODO
    return type;
}


FileType *FileTypeManager::getOneBy(QMap<QString, QString> properties)
{
    //Building the sql request
    QString request = "select * from File_type where ";

    QMapIterator<QString, QString> i(properties);
    int j = 0;
    while(i.hasNext()){
        i.next();
        request += ((j != 0) ? " and " : " ") + i.key()+" = '"+i.value()+"'";
        j++;
    }
    //qDebug() << request << endl;

    QSqlQuery sqlQuery(Parameters::localDb);
    sqlQuery.exec(request);
    if (!sqlQuery.isActive()){
        qDebug() << "Failed to select the file's type object ! " << sqlQuery.lastError().text() << endl;
        return NULL;
    }

    if (sqlQuery.next()){
        //qDebug() << "Yes file type" << endl;
        QMap<QString, QString> formatProperties;
        formatProperties.insert("id", sqlQuery.value(3).toString());
        return new FileType(sqlQuery.value(0).toInt(), sqlQuery.value(1).toString(), sqlQuery.value(2).toString(), FileFormatManager::getOneBy(formatProperties));
    }
    //qDebug() << "Yes file type" << properties["id"] << endl;
    return NULL;
}

/**
 * @brief FileTypeManager::add
 * @param type
 * @return
 */
FileType* FileTypeManager::add(FileType * type)
{
    QString request = "insert into File_type(id, name, suffix, format_id) select NULL, '"+type->getName()+"', '"+type->getSuffix()+"', "+((type->getFileFormat() == NULL) ? "-1" : QString::number(type->getFileFormat()->getId()))+" where not exists (select 1 from File_type where suffix = '"+type->getSuffix()+"') ";
    qDebug() << request << endl;
    QSqlQuery sqlQuery(Parameters::localDb);
    sqlQuery.exec(request);
    if (!sqlQuery.isActive()){
        qDebug() << "Failed to insert a new type : " << type->getName() << " | " << type->getSuffix() << " ? " << sqlQuery.lastError().text() << endl;
        return NULL;
    }
    QMap<QString, QString> fileTypeProperties;
    fileTypeProperties.insert("suffix", type->getSuffix());
    return getOneBy(fileTypeProperties);
}

/**
 * @brief FileTypeManager::getAll
 * @return
 */
QList<FileType *> * FileTypeManager::getAll()
{
    QList<FileType *> * types = new QList<FileType *>();
    QString request = "select * from File_type";
    QSqlQuery sqlQuery(Parameters::localDb);
    sqlQuery.exec(request);
    if (!sqlQuery.isActive()){
        qDebug() << "Failed fetching all types : " << sqlQuery.lastError().text() << endl;
        return NULL;
    }
    while (sqlQuery.next()){
        //qDebug() << "Vroom " << sqlQuery.value(3).toString() << endl;
        FileFormat * fileFormat = NULL;
        if (sqlQuery.value(3).toString() != "-1"){
            QMap<QString, QString> formatProperties;
            formatProperties.insert("id", sqlQuery.value(3).toString());
            fileFormat = FileFormatManager::getOneBy(formatProperties);
        }
        types->append(new FileType(sqlQuery.value(0).toInt(), sqlQuery.value(1).toString(), sqlQuery.value(2).toString(), fileFormat));
    }
    return types;
}

/**
 * @brief FileTypeManager::printAll
 * @param types
 */
void FileTypeManager::print(QList<FileType *> * types)
{
    int size = types->size();
    for (int i = 0; i < size; i++){
        types->at(i)->toString();
    }
}

void FileTypeManager::printAll()
{
    print(getAll());
}

QList<FileType *> *FileTypeManager::getBy(QMap<QString, QString> properties)
{
    QList<FileType * > * fileTypes = new QList<FileType * >();
    //Building the sql request
    QString request = "select * from File_type where ";

    QMapIterator<QString, QString> i(properties);
    int j = 0;
    while(i.hasNext()){
        i.next();
        request += ((j != 0) ? " and " : " ") + i.key()+" = '"+i.value()+"'";
        j++;
    }
    //qDebug() << request << endl;

    QSqlQuery sqlQuery(Parameters::localDb);
    sqlQuery.exec(request);
    if (!sqlQuery.isActive()){
        qDebug() << "Failed to select the file's type object ! " << sqlQuery.lastError().text() << endl;
        return NULL;
    }

    while (sqlQuery.next()){
        //qDebug() << "Yes file type" << endl;
        QMap<QString, QString> formatProperties;
        formatProperties.insert("id", sqlQuery.value(3).toString());
        fileTypes->append(new FileType(sqlQuery.value(0).toInt(), sqlQuery.value(1).toString(), sqlQuery.value(2).toString(), FileFormatManager::getOneBy(formatProperties)));
    }
    //qDebug() << "Yes file type" << properties["id"] << endl;
    return fileTypes;
}
