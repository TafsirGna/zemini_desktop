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
    bool defaultDrive;

public:
    Drive();
    Drive(int, QString, DriveType *, bool);
    int getId() const;
    void setId(int value);
    QString getAbsolutepath() const;
    void setAbsolutepath(const QString &value);
    DriveType *getType() const;
    void setType(DriveType *value);
    void setRequestParams(QUrlQuery &);

    QString toString();
    QString serialize();
    bool isDefaultDrive() const;
    void setDefault(bool value);
};

#endif // DRIVE_H
