#ifndef FILEFORMATMANAGER_H
#define FILEFORMATMANAGER_H

#include <Database/Entities/FileFormat.h>
#include <Database/EntityManagers/AbstractManager.h>
#include <Config/Functions.h>
#include <QMap>

class FileFormatManager : public AbstractManager
{
public:
    FileFormatManager();

    static FileFormat * getOneBy(QMap<QString, QString>);
    static FileFormat * add(FileFormat *);
    static QList<FileFormat *> *getAll();
};

#endif // FILEFORMATMANAGER_H
