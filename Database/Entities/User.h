#ifndef CLASSUSER_H
#define CLASSUSER_H

#include <QString>
#include <QDateTime>
#include <QList>
#include <QDebug>

using namespace std;
class User
{
private:
        int id;
        QString familyname;
        QString firstname;
        QString email;
        QString username;
        QString password;
        //QDateTime dateinscription;
        bool activated;

public:
        //Constructor
        User();
        User(QString familyname, QString firstname, QString email,QString username,QString password);
        User(int id,QString familyname, QString firstname, QString email,QString username,QString password);

        //Getters
        int getId();
        QString getFamilyname();
        QString getFirstname();
        QString getEmail();
        QString getUsername();
        QString getPassword();
        bool isActivated();
        QDateTime getDateInscription();

        //Setters
        void setId(int id);
        void setFamilyname(QString name);
        void setFirstname(QString firstName);
        void setEmail (QString email);
        void setDateInscription(QDateTime dateInscription);
        void setUsername(QString pseudo);
        void setPassword(QString password);
        void setActivated(bool activated);

        void toString();

};

#endif // CLASSUSER_H
