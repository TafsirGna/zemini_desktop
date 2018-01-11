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
        QString phonenumber;
        QString username;
        QString password;
        QDateTime dateinscription;
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
        QString getPhonenumber();
        int getIdlocation();
        QString getUsername();
        QString getPassword();
        QDateTime getDateExit();
        QList<QString> getRoles();
        bool isActivated();
        QString getActivationkey();
        QDateTime getDateInscription();
        QString getSalt();

        //Setters
        void setId(int id);
        void setFamilyname(QString name);
        void setFirstname(QString firstName);
        void setEmail (QString email);
        void setPhonenumber(QString phoneNumber);
        void setIdlocation(int numberLocation);
        void setDateInscription(QDateTime dateInscription);
        void setUsername(QString pseudo);
        void setPassword(QString password);
        void setDateExit(QDateTime dateDesinscription);
        void setActivated(bool activated);
        void setActivationkey(QString cleActivation);
        void setRoles(QList<QString> roles);
        void setSalt(QString salt);

        void toString();

};

#endif // CLASSUSER_H
