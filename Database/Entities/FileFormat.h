#ifndef FILEFORMAT_H
#define FILEFORMAT_H

#include <Database/Entities/DbEntity.h>

class FileFormat: public DbEntity
{

private:
    int id;
    QString name;


public:
    FileFormat();
    FileFormat(int id, QString name);
    int getId() const;
    void setId(int value);
    QString getName() const;
    void setName(const QString &value);
};

#endif // FILEFORMAT_H
