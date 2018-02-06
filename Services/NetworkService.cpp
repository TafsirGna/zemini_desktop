#include "NetworkService.h"

/***            Constructors            ***/
NetworkService::NetworkService()
{
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
void NetworkService::getInitialDbData()
{
    // sending the query to get all data necessary
    requestCode = 3;
    networkAccessManager->get(QNetworkRequest(QUrl(Parameters::url+"/"+QString::number(requestCode)+"/categories")));
}

void NetworkService::handleRequestReply(QNetworkReply * reply)
{
    //qDebug() << "connected!" << ((connected) ? "ok" : "not ok") << endl;
    if (reply->error()!=QNetworkReply::NoError){

        switch (requestCode) {
        case 1:{

            break;
        }
        case 2:{ // login authentification
            connected = true;
            User * user = NULL;
            int resultCode = 0;
            emit credentialsChecked(resultCode, user);
            break;

        }
        case 3:{
            connected = false;
            QList<Category> * categories = NULL;
            emit initDataGot(categories);
            break;
        }
        }
        return;

    }

    connected = true;

    // Processing the response of the requests sent
    QString response = (QString)reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();
    QVariantMap json_map = jsonObject.toVariantMap();

    int requestCode = json_map["requestCode"].toInt();
    switch (requestCode) {
    case 1: // sign up request code

        break;
    case 2:{ // login request code
        // building an user object from the data got
        User * user = NULL;
        int resultCode = 1;
        if (json_map["success"].toBool()){
            user = Functions::fromJsonToUser(json_map);
        }
        emit credentialsChecked(resultCode, user);
        break;
    }
    case 3: // getting initial data to get started
        QList<Category> * categories = Functions::fromJsonToCategories(json_map);
        emit initDataGot(categories);
        break;
    //default:
    //    break;
    }

}

void NetworkService::run()
{
    this->timer->start(Parameters::networkTimerFrequency);
}

void NetworkService::refreshLocalDb()
{

}

void NetworkService::sendUser(User * user){

}

void NetworkService::checkCredentials(QString email, QString password)
{
    requestCode = 2; // login request code
    connected = false;
    QString address = Parameters::url+
            "/login/"+QString::number(requestCode)+
            "/"+email+
            "/"+password;
    qDebug() << "check " << address << endl;
    networkAccessManager->get(QNetworkRequest(QUrl(address)));
}
