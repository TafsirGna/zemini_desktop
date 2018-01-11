#ifndef ABSTRACTFILE_H
#define ABSTRACTFILE_H

#include <QObject>
#include <QDateTime>
#include <QString>
#include "Database\Entities\Type.h"

class AbstractFile : public QObject
{
    Q_OBJECT

protected:
    int id;
    QString name;
    QDateTime datecreation;
    int size;
    QString path;
    int status;
    Type * type;

public:
    //builders
    AbstractFile();
    AbstractFile(QString name, QDateTime datecreation, int size, QString path,int status, Type * type);
    AbstractFile(int id,QString name, QDateTime datecreation, int size, QString path,int status, Type * type);

    //Getters
    int getId();
    QString getName();
    QDateTime getDatecreation();
    int getSize();
    QString getPath();
    int getStatus();
    Type * getType();

    //Setters
    void setId(int id);
    void setName(QString name);
    void setDatecreation(QDateTime datecreation);
    void setSize(int size);
    void setPath(QString path);
    void setStatus(int status);
    void setType(Type * type);
};

#endif // ABSTRACTFILE_H
