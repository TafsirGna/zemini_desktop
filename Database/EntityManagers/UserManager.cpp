#include "Database\EntityManagers\UserManager.h"
#include "Controllers\MainController.h"

/***            Constructors            ***/
UserManager::UserManager(QSqlDatabase &db): AbstractManager(db)
{
    user = NULL;
}

/***            This function allows to create or save a user into a database           ***/
/*
void UserManager::saveUser(User user)
{
    //qDebug()<<"sending data";

    if (!saveUserLocally(user))
    {
        QMessageBox::critical(0,"Message","An error occured when saving your parameters! Please, close the window and start over all the process");
        return;
    }

    if(connected)
    {
        //if the user is connected then the data on the user is sent to the server

        QUrl params;

        params.addQueryItem("action",QString::number(1));
        params.addQueryItem("familyname",user.getFamilyname());
        params.addQueryItem("firstname",user.getFirstname());
        params.addQueryItem("email",user.getEmail());
        params.addQueryItem("username",user.getUsername());
        params.addQueryItem("password",user.getPassword());


        QUrl resource(str_Url);
        QNetworkRequest request(resource);
        //request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
        this->accessManager->get(QNetworkRequest(params));
    }
}
*/

bool UserManager::insertUser(User user)
{
    //QString request = "INSERT INTO User VALUES("+QString::number(user.getId())+", '"+user.getFamilyname()+"', '"+user.getFirstname()+"', '"+user.getEmail()+"','"+user.getUsername()+"','"+user.getPassword()+"','"+user.getDateInscription().toString(Parameters::timeFormat())+"',"+((user.isActivated()) ? QString::number(1) : QString::number(0))+")";
    QString request = "INSERT INTO User VALUES("+QString::number(user.getId())+", '"+user.getFamilyname()+"', '"+user.getFirstname()+"', '"+user.getEmail()+"','"+user.getUsername()+"','"+user.getPassword()+"',"+((user.isActivated()) ? QString::number(1) : QString::number(0))+")";
    qDebug() << request << endl;
    query->exec(request);
    if(!query->isActive()){
        qDebug()<<"Erreur insertion User: " + query->lastError().text();
        return false;
    }
    return true;
}

/***            This function allows to update a user of the database           ***/
void UserManager::updateUser(User user)
{

}

/***            This function allows to delete a user of the database           ***/
void UserManager::deleteUser(User user)
{

}

User * UserManager::getUser()
{
    //if (user != NULL)
    //    return user;
    qDebug() << "test" << endl;
    query->exec("select id,familyname, firstname, email, username, password, dateinscription, activated from user where id = 1");
    if (query->next())
    {
        //qDebug() << "a user 's here !";
        User * user = new User(query->value(0).toInt(),query->value(1).toString(),
                               query->value(2).toString(),query->value(3).toString(),
                               query->value(4).toString(),query->value(5).toString());
        user->setActivated(((query->value(7).toInt() == 0) ? false : true ));
        return user;
    }
    return NULL;
}

void UserManager::downloadUser(User *user)
{
    //if the user is connected then the data on the user is sent to the server

    //this->action = 4;

    /*QUrl params(str_Url+"?action=4"+
                "&email=" + user->getEmail() +
                "&password=" + user->getPassword() +
                "&familyname="+ user->getFamilyname()+
                "&firstname="+ user->getFirstname()+
                "&username=" + user->getUsername()
                );*/

    //request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    //this->accessManager->get(QNetworkRequest(params));
}
/*
void UserManager::showUsersInDb()
{
    query->exec("select id,familyname, firstname, email, username, password, dateinscription, activated from user");
    qDebug() << " --- listing of users";
    while (query->next())
    {
        //qDebug() << "a user 's here !";
        User * user = new User(query->value(0).toInt(),query->value(1).toString(),
                               query->value(2).toString(),query->value(3).toString(),
                               query->value(4).toString(),query->value(5).toString());
        user->setActivated(((query->value(7).toInt() == 0) ? false : true ));
        user->toString();
        //return user;
    }
    qDebug() << " --- end of listing ";
}*/

