#include <QApplication>
#include "Controllers\MainController.h"
#include <QStyle>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Setting style for the app
    a.setStyle(QStyleFactory::create("plastique"));

    // building and launching the app
    MainController * zemini = new MainController();
    zemini->start();

    return a.exec();
}
