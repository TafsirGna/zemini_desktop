#include "FileFormatManager.h"

FileFormatManager::FileFormatManager()
{

}

FileFormat *FileFormatManager::getOneBy(QMap<QString, QString> properties)
{
    QSqlQuery sqlQuery(Parameters::localDb);
    QString request = "select * from File_format where ";

    QMapIterator<QString, QString> i(properties);
    int j = 0;
    while(i.hasNext()){
        i.next();
        request += ((j != 0) ? " and " : " ") + i.key()+" = '"+i.value()+"'";
        j++;
    }

    sqlQuery.exec(request);
    if (!sqlQuery.isActive()){
        qDebug() << "Failed to fetch the fileformat object " << endl;
        return NULL;
    }

    //qDebug() << "poo 1" << request << endl;
    if (sqlQuery.next()){
        //qDebug() << "poo 2" << sqlQuery.value(0).toString() << endl;
        return new FileFormat(sqlQuery.value(0).toInt(), sqlQuery.value(1).toString());
    }
    return NULL;
}

FileFormat *FileFormatManager::add(FileFormat * format)
{
    QSqlQuery sqlQuery(Parameters::localDb);
    QString request = "insert into File_format values(NULL, '"+format->getName()+"')";
    sqlQuery.exec(request);
    if (!sqlQuery.isActive()){
        qDebug() << "Failed to insert the file format" << endl;
        return NULL;
    }
    QMap<QString, QString> properties;
    properties.insert("name", format->getName());
    return getOneBy(properties);
}

QList<FileFormat *> *FileFormatManager::getAll()
{
    QList<FileFormat * > * fileFormats = new QList<FileFormat * >();
    QSqlQuery sqlQuery(Parameters::localDb);
    QString request = "select * from File_format";
    sqlQuery.exec(request);
    if (!sqlQuery.isActive()){
        qDebug() << "Failed to insert the file format" << endl;
        return NULL;
    }

    while (sqlQuery.next()){
        FileFormat * fileFormat = Functions::fromSqlRecord2FileFormat(sqlQuery.record());
        fileFormats->append(fileFormat);
    }
    return fileFormats;
}
