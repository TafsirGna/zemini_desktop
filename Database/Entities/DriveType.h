#ifndef DRIVETYPE_H
#define DRIVETYPE_H

#include <QString>
#include <Database/Entities/DbEntity.h>

class DriveType : public DbEntity
{

private:
    int id;
    QString name;

public:
    DriveType();
    DriveType(int id, QString name);
    int getId() const;
    void setId(int value);
    QString getName() const;
    void setName(const QString &value);

    QString serialize();
    void setRequestParams(QUrlQuery &);

    // constant static variables
    const static QString COMPUTER;
    const static QString SMARTPHONE;
    const static QString EX_HARD_DRIVE;
};

#endif // DRIVETYPE_H
