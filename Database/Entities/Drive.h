#ifndef DRIVE_H
#define DRIVE_H

#include <Database/Entities/DriveType.h>
#include <Config/Parameters.h>
#include <Database/Entities/DbEntity.h>

class Drive : public DbEntity
{

private:
    int id;
    QString absolutepath;
    DriveType * type;

public:
    Drive();
    Drive(int, QString, DriveType *);
    int getId() const;
    void setId(int value);
    QString getAbsolutepath() const;
    void setAbsolutepath(const QString &value);
    DriveType *getType() const;
    void setType(DriveType *value);

    QString toString();
    QString serialize();
};

#endif // DRIVE_H
