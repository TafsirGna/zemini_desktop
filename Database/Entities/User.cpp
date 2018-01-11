#include "Database\Entities\User.h"

/***            Default constructor         ***/
User::User()
{
    //Nothing to do here for now
}

/***            Another constructor for setting up the values of the class object           ***/
User::User(QString familyname, QString firstname, QString email,QString username,QString password)
{
    this->id = 0;
    this->familyname = familyname;
    this->firstname = firstname;
    this->email = email;
    this->phonenumber = "+229";
    this->activated = 0;
    this->password = password;
    //this->dateinscription = (*(new QDateTime()));
    //this->roles = (*(new QList<QString>()));
    this->username = username;
}

/***            Another constructor for setting up the values of the class object           ***/
User::User(int id,QString familyname, QString firstname, QString email,QString username,QString password)
{
    this->id = id;
    this->familyname = familyname;
    this->firstname = firstname;
    this->email = email;
    this->phonenumber = "+229";
    this->activated = 0;
    this->password = password;
    this->dateinscription = (*(new QDateTime()));
    this->username = username;
}

/***            Getters         ***/
QString User::getFirstname()
{
    return this->firstname;
}

QString User::getFamilyname()
{
    return this->familyname;
}

QString User::getPhonenumber()
{
    return this->phonenumber;
}

QString User::getEmail()
{
    return this->email;
}

int User::getId()
{
    return this->id;
}

QString User::getUsername()
{
    return this->username;
}

QString User::getPassword()
{
    return this->password;
}

QDateTime User::getDateInscription()
{
    return this->dateinscription;
}

bool User::isActivated()
{
    return this->activated;
}

/***               Setters          ***/
void User::setFirstname(QString firstname)
{
    this->firstname = firstname;
}

void User::setFamilyname(QString familyname)
{
    this->familyname =familyname;
}

void User::setPhonenumber(QString phonenumber)
{
    this->phonenumber = phonenumber;
}

void User::setId(int id)
{
    this->id = id;
}

void User::setEmail(QString email)
{
    this->email = email;
}

void User::setUsername(QString username)
{
    this->username = username;
}

void User::setPassword(QString password)
{
    this->password = password;
}

void User::setDateInscription(QDateTime dateinscription)
{
    this->dateinscription = dateinscription;
}

void User::setActivated(bool activated)
{
    this->activated = activated;
}

void User::toString()
{
    //qDebug()<< "--- user's data display ---";
    //qDebug()<< "id : " + QString::number(this->id) + ", familyname : " + this->familyname + ", firstname : " + this->firstname +
               ", email : " + this->email + ", username : " + this->username + ", password : " + this->password +
               ", dateinscription : " + this->dateinscription.toString() + ", phonenumber : " + this->phonenumber;
}
