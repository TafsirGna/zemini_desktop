#include "Database\EntityManagers\UserManager.h"
#include "Controllers\MainController.h"

/***            Constructors            ***/
UserManager::UserManager(): AbstractManager()
{
    user = NULL;
}

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
    QString request = "Update User set familyname = '"+user.getFamilyname()+"' ,  firstname = '"+user.getFirstname()+"' , email = '"+user.getEmail()+"' , username = '"+user.getUsername()+"' , password = '"+user.getPassword()+"' , activated = "+((user.isActivated()) ? QString::number(1) : QString::number(0))+" where id = "+QString::number(user.getId());
    QSqlQuery sqlQuery(Parameters::localDb);
    sqlQuery.exec(request);
    if(!sqlQuery.isActive()){
        qDebug()<<"Error when updating User: " + sqlQuery.lastError().text();
        return NULL;
    }
    qDebug()<<"Success in updating User: " + request << endl;
    return getUser();
}

/***            This function allows to delete a user of the database           ***/
bool UserManager::remove(User user)
{

}

User * UserManager::getUser()
{
    // TODO : Test that the request has been executed as expected
    QSqlQuery sqlQuery(Parameters::localDb);
    sqlQuery.exec("select id,familyname, firstname, email, username, password, activated from User where id = 1");
    if (sqlQuery.next()){
        User * user = new User(sqlQuery.value(0).toInt(),sqlQuery.value(1).toString(),
                               sqlQuery.value(2).toString(),sqlQuery.value(3).toString(),
                               sqlQuery.value(4).toString(),sqlQuery.value(5).toString());
        user->setActivated(((sqlQuery.value(6).toInt() == 0) ? false : true ));
        //user->toString();
        return user;
    }
    return NULL;
}

