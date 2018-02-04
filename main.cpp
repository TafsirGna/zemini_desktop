#include <QApplication>
#include "Controllers\MainController.h"
#include <QStyle>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle(QStyleFactory::create("plastique"));

    // building and launching the app
    if (Parameters::localDb == NULL)
        return 0;

    MainController * zemini = new MainController();
    zemini->start();

    return a.exec();
}
