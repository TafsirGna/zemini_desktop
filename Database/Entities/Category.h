#ifndef CLASSCATEGORY_H
#define CLASSCATEGORY_H

#include <QString>

class Category
{
private:
    //Class' properties
    int id;
    QString name;

public:

    //Builders
    Category();
    Category(int id, QString name);

    //Getters
    int getId();
    QString getName();

    //Setters
    void setId(int id);
    void setName(QString name);
};

#endif // CLASSCATEGORY_H
