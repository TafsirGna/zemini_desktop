#include <QApplication>
#include "Controllers\MainController.h"
#include <QStyle>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle(QStyleFactory::create("plastique"));

    // Checking first the connection before launching the app
    /*
    DatabaseManager dbManager;
    QSqlDatabase * db = dbManager.connectToDatabase("ZeminiDB", "root","localhost", 3306, "","QSQLITE", "db");
    if (db == NULL)
        return 0;
    */

    // Launching the app
    MainController * zemini = new MainController();
    zemini->start();

    return a.exec();
}
