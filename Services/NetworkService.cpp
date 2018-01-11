#include "NetworkService.h"

/***            Constructors            ***/
NetworkService::NetworkService(QWidget *parent)
{
    this->parent = parent;
    networkAccessManager = new QNetworkAccessManager(this);
    QObject::connect(networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleRequestReply(QNetworkReply*)));
    this->connected = false;

    //Setting the timers
    this->timer = new QTimer(this);
    QWidget::connect(timer, SIGNAL(timeout()),this, SLOT(refreshLocalDb()));
}

/***    this check the user's network status    ***/
bool NetworkService::isConnected()
{
    return connected;
}

// this function sends a request to the remote server to get its initial data to get started
void NetworkService::initLocalDb()
{
    networkAccessManager->get(QNetworkRequest(QUrl(Parameters::url())));
}

void NetworkService::handleRequestReply(QNetworkReply * reply)
{
    if (reply->error()!=QNetworkReply::NoError)
        return;

    connected = true;
}

void NetworkService::run()
{
    this->timer->start(Parameters::networkTimerFrequency());
}

void NetworkService::refreshLocalDb()
{

}

void NetworkService::sendUser(User * user){

}
