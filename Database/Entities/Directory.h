#ifndef CLASSDIRECTORY_H
#define CLASSDIRECTORY_H

#include "abstractfile.h"
#include <QDebug>

class Directory : public AbstractFile
{
private :
    Directory * directory;
public :
    Directory();
    Directory(QString name, QDateTime datecreation, int size, QString path, int status,Type * type,Directory * directory);
    Directory(int id, QString name, QDateTime datecreation, int size, QString path, int status, Type * type, Directory *directory);
    Directory(const Directory & directory);
    Directory & operator=(const Directory & directory);

    void toString();

    Directory * getDirectory();
    void setDirectory(Directory * directory);
};

#endif // CLASSDIRECTORY_H
