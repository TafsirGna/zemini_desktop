#include "Database\EntityManagers\UserManager.h"
#include "Controllers\MainController.h"

/***            Constructors            ***/
UserManager::UserManager(): AbstractManager()
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

/**
 * @brief UserManager::insertUser
 * @param user
 * @return
 */
User * UserManager::add(User user)
{
    QString request = "INSERT INTO User VALUES("+QString::number(user.getId())+", '"+user.getFamilyname()+"', '"+user.getFirstname()+"', '"+user.getEmail()+"','"+user.getUsername()+"','"+user.getPassword()+"',"+((user.isActivated()) ? QString::number(1) : QString::number(0))+")";
    QSqlQuery sqlQuery(Parameters::localDb);
    sqlQuery.exec(request);
    if(!sqlQuery.isActive()){
        qDebug()<<"Erreur insertion User: " + sqlQuery.lastError().text();
        return NULL;
    }
    qDebug()<<"Success inserting User: " + request << endl;
    return getUser();
}

/***            This function allows to update a user of the database           ***/
User * UserManager::update(User user)
{

}

/***            This function allows to delete a user of the database           ***/
bool UserManager::remove(User user)
{

}

User * UserManager::getUser()
{
    // TODO : Test that the request has been executed as expected
    QSqlQuery sqlQuery(Parameters::localDb);
    sqlQuery.exec("select id,familyname, firstname, email, username, password, activated from user where id = 1");
    if (sqlQuery.next()){
        User * user = new User(sqlQuery.value(0).toInt(),sqlQuery.value(1).toString(),
                               sqlQuery.value(2).toString(),sqlQuery.value(3).toString(),
                               sqlQuery.value(4).toString(),sqlQuery.value(5).toString());
        user->setActivated(((sqlQuery.value(7).toInt() == 0) ? false : true ));
        return user;
    }
    return NULL;
}

