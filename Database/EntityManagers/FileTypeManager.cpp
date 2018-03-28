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
        FileTypeManager::getOneBy(fileTypeProperties)->toString();
        return FileTypeManager::getOneBy(fileTypeProperties);
    }

    QMap<QString, QString> fileTypeProperties;
    fileTypeProperties.insert("suffix", fileInfo.suffix());
    FileType * type = FileTypeManager::getOneBy(fileTypeProperties);
    if (type == NULL)
        return add(new FileType(0, fileInfo.suffix(), fileInfo.suffix())); //TODO
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
        return new FileType(sqlQuery.value(0).toInt(), sqlQuery.value(1).toString(), sqlQuery.value(2).toString());
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
    QString request = "insert into File_type(id, name, suffix) select NULL, '"+type->getName()+"', '"+type->getSuffix()+"' where not exists (select 1 from File_type where suffix = '"+type->getSuffix()+"') ";
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
    QString request = "select id, name, suffix from File_type";
    QSqlQuery sqlQuery(Parameters::localDb);
    sqlQuery.exec(request);
    if (!sqlQuery.isActive()){
        qDebug() << "Failed fetching all types : " << sqlQuery.lastError().text() << endl;
        return NULL;
    }
    while (sqlQuery.next()){
        types->append(new FileType(sqlQuery.value(0).toInt(), sqlQuery.value(1).toString(), sqlQuery.value(2).toString()));
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

/***        function designed to retrieve a type object from the local database     ***/
/*
Type * TypeManager::findTypeById(int id)
{
    QString request = "select idcategory,name from type where id = "+QString::number(id)+"";
    //qDebug()<< request;

    query->exec(request);

    if (!query->isActive())
    {
        qDebug()<<"Error when selecting type : " + query->lastError().text();
        return NULL;
    }

    if (query->next())
    {
        Category * category = categoryManager->findCategoryById(query->value(2).toInt());
        return new Type(id,query->value(1).toString(),category);
    }
    else
        return getDefaultType();
}

int TypeManager::getTypeSize(int idType)
{
    QString request = "select sum(size) from file where idtype = "+QString::number(idType)+" and file.status <> 0 ";
    query->exec(request);
    if (!query->isActive())
    {
        qDebug()<< "Error when selecting all files' size ";
        return 0;
    }

    int size = 0;
    if (query->next())
        size = query->value(0).toInt();

    return size;
}

int TypeManager::getTypeSize(QString nameType)
{
    QString request = "select sum(size) from file,type,category where file.idtype = type.id and type.idcategory = category.id and category.name = '"+nameType+"' and file.status <> 0 ";
    query->exec(request);
    if (!query->isActive())
    {
        qDebug()<< "Error when selecting all files' size ";
        return 0;
    }

    int size = 0;
    if (query->next())
        size = query->value(0).toInt();

    //qDebug() << "flush 2 "+ QString::number(size);
    return size;
}

int TypeManager::getAllTypesSize()
{
    QString request = "select sum(size) from file where status <> 0";
    query->exec(request);
    if (!query->isActive())
    {
        qDebug()<< "Error when selecting all files' size ";
        return 0;
    }

    int size = 0;
    if (query->next())
        size = query->value(0).toInt();

    //qDebug() << "flush 1 "+ QString::number(size);
    return size;
}

int TypeManager::getTypePercentage(QString nameType)
{
    double proportion = (double) ((getTypeSize(nameType)*100)/getAllTypesSize());
    //qDebug() << "flush 3 "+ QString::number(proportion);
    int percent = (int) floor(proportion);
    return percent;
}

Type * TypeManager::getDefaultType()
{
    query->exec("select type.id,idcategory,type.name, category.name from type,category  where category.id = type.idcategory and  category.name = 'Others'");
    if (!query->isActive())
    {
        qDebug()<<"Error when selecting type : " + query->lastError().text();
        return NULL;
    }

    if (query->next())
    {
        Category * category = new Category(query->value(1).toInt(),query->value(3).toString());
        return new Type(query->value(0).toInt(),query->value(2).toString(),category);
    }
    return NULL;
}
*/

