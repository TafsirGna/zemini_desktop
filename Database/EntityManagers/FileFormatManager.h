#ifndef FILEFORMATMANAGER_H
#define FILEFORMATMANAGER_H

#include <Database/Entities/FileFormat.h>
#include <Database/EntityManagers/AbstractManager.h>
#include <QMap>

class FileFormatManager : public AbstractManager
{
public:
    FileFormatManager();

    static FileFormat * getOneBy(QMap<QString, QString>);
    static FileFormat * add(FileFormat *);
};

#endif // FILEFORMATMANAGER_H
