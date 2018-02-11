#include "NetworkService.h"

/***            Constructors            ***/
NetworkService::NetworkService()
{
    networkAccessManager = new QNetworkAccessManager(this);
    this->connected = false;
    this->timer = new QTimer(this);
    //sslSocket = new QSslSocket(this);

    // Setting connectors
    QWidget::connect(networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleRequestReply(QNetworkReply*)));
    QWidget::connect(timer, SIGNAL(timeout()),this, SLOT(syncDb()));
    //QWidget::connect(sslSocket, SIGNAL(encrypted()), this, SLOT(sslSocketConnected()));

    this->timer->start(Parameters::networkTimerFrequency);
    //this->sslSocket->connectToHostEncrypted(Parameters::url, 993);
}

/***    this check the user's network status    ***/
bool NetworkService::isConnected()
{
    return connected;
}

void NetworkService::sslSocketConnected()
{

}

// this function sends a request to the remote server to get its initial data to get started
void NetworkService::getInitialDbData()
{
    // sending the query to get all data necessary
    requestCode = 3;
    networkAccessManager->get(QNetworkRequest(QUrl(Parameters::url+"/"+QString::number(requestCode)+"/categories")));
    //qDebug() << Parameters::url+"/"+QString::number(requestCode)+"/categories" << endl;
}

void NetworkService::handleRequestReply(QNetworkReply * reply)
{
    if (reply->error()!= QNetworkReply::NoError){

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

void NetworkService::syncDb()
{
    // at this stage, we only backup the database data to a remote db server
    emit readyToBackUp();
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

void NetworkService::sendFile(File dir)
{

}
