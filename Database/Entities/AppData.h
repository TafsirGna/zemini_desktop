#ifndef APPDATA_H
#define APPDATA_H

#include <QString>
#include <Database/Entities/DbEntity.h>
/**
 * @brief The AppData class : Class corresponding to AppData Table in the database
 */
class AppData: public DbEntity
{
private:
    QString key;
    QString value;
public:
    AppData();
    AppData(QString, QString);
    QString getValue() const;
    void setValue(const QString &value);
    QString getKey() const;
    void setKey(const QString &value);

    // static properties
    const static QString LAST_EXIT;
};

#endif // APPDATA_H
