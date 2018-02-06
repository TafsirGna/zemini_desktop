#ifndef TESTSMANAGER_H
#define TESTSMANAGER_H

#include "Controllers/MainController.h"
#include <QSqlQuery>

class TestsManager
{
private:
    MainController * app;
    QSqlQuery * sqlQuery;

public:
    TestsManager(MainController * zemini);
    bool runTests();
    bool testUserInsertion();
};

#endif // TESTSMANAGER_H
