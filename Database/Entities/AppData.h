#ifndef APPDATA_H
#define APPDATA_H

#include <QString>
/**
 * @brief The AppData class : Class corresponding to AppData Table in the database
 */
class AppData
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
};

#endif // APPDATA_H
