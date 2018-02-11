#ifndef CLASSTYPE_H
#define CLASSTYPE_H

#include <QString>

class Type
{
private:
    //Class' properties
    int id;
    QString name;
    QString suffix;


public:

    //Builders
    Type();
    Type(int, QString, QString);

    //Getters
    int getId();
    QString getName();
    QString getSuffix();

    //Setters
    void setId(int id);
    void setName(QString);
    void setSuffix(QString);
};


#endif // CLASSTYPE_H
