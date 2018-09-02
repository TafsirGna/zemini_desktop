#include <QApplication>
#include "Controllers\MainController.h"
#include <QStyle>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //checking the ssl support
    if (!QSslSocket::supportsSsl()){
        QMessageBox::information(0, "Secure socket client", "This software doesn't support SSL/TLS. ");
        return -1;
    }

    // Launching the app
    MainController * zemini = new MainController();
    zemini->start();

    return a.exec();
}
