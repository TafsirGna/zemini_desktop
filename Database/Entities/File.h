#ifndef CLASSFILE_H
#define CLASSFILE_H

#include <QString>
#include <QDateTime>
#include "Database\Entities\User.h"
#include "Database\Entities\Type.h"
#include "Database\Entities\AbstractFile.h"
#include "Database\Entities\Directory.h"
#include <QDebug>
using namespace std;

class File : public AbstractFile
{
private:
    QString suffix;
    Directory * directory;

public:
    //Constructors
    File();
    File(QString filename, QDateTime datecreation, int size, QString path, int status, Type * type, QString suffix, Directory * directory);
    File(int id, QString filename, QDateTime datecreation, int size, QString path, int status, Type * type, QString suffix, Directory *directory);
    File(const File & file);
    File & operator =(const File & file);

    //Getters
    QString getSuffix();
    Directory * getDirectory();

    //Setters
    void setSuffix(QString suffix);
    void setDirectory(Directory * directory);

    void toString();
};

#endif // CLASSFILE_H
