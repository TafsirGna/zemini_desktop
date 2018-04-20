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

    if (sqlQuery.next()){
        return new FileFormat(sqlQuery.value(0).toInt(), sqlQuery.value(1).toString());
    }
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
