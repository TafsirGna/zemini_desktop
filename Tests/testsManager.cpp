#include "TestsManager.h"
#include <QSqlQuery>

/**
 * @brief TestsManager::TestsManagers
 * @param zemini
 */
TestsManager::TestsManager(MainController *zemini)
{
    app = zemini;
    //sqlQuery = new QSqlQuery(zemini->getServices("localdb"));
}

/**
 * @brief TestsManager::runTests
 * Runs a set of test functions
 * @return
 */
bool TestsManager::runTests()
{
    return NULL;
}

/**
 * @brief TestsManager::testUserInsertion
 * Tests the insertion of a user in the local database
 * @return
 */
bool TestsManager::testUserInsertion()
{
    /*
    QString request = "INSERT INTO User(id, familyname, firstname, email, username, password, dateinscription, activated) VALUES("+QString::number(user.getId())+", '"+user.getFamilyname()+"', '"+user.getFirstname()+"', '"+user.getEmail()+"','"+user.getUsername()+"','"+user.getPassword()+"','"+user.getDateInscription().toString(Parameters::timeFormat())+"',"+((user.isActivated()) ? QString::number(1) : QString::number(0))+")";
    qDebug() << request << endl;
    query->exec(request);
    if(!query->isActive()){
        qDebug()<<"Erreur insertion User: " + query->lastError().text();
        return false;
    }
    return true;
    */
}
