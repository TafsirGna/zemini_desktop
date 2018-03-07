#include "NetworkService.h"

const int NetworkService::CODE_DB_INIT = 0;
const int NetworkService::CODE_REGISTER_USER = 1;
const int NetworkService::CODE_USER_LOGIN = 2;
const int NetworkService::CODE_FILE_SAVE = 3;
const int NetworkService::CODE_DB_REFRESH = 4;

/**
 * @brief NetworkService::NetworkService
 * Builder
 */
NetworkService::NetworkService()
{
    networkAccessManager = new QNetworkAccessManager(this);
    //cypherService = new CypherService();
    this->connected = false;
    this->timer = new QTimer(this);

    //cypherService->encryptRsa("test");
    //cypherService->decryptRsa("ok");
    //cypherService->decryptRsa(cypherService->encryptRsa("test"));

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
    networkAccessManager->get(QNetworkRequest(QUrl(Parameters::URL+"/init_db")));
    //qDebug() << Parameters::URL+"/init_db" << endl;
}

void NetworkService::handleRequestReply(QNetworkReply * reply)
{
    if (reply->error()!= QNetworkReply::NoError){
        handleBadRequestReply(reply);
        return;
    }
    handleGoodRequestReply(reply);
}

void NetworkService::syncDb()
{
    // at this stage, we only backup the database data to a remote db server
    emit readyToBackUp();
}

void NetworkService::sendUser(User * user){

}

/**
 * @brief NetworkService::checkCredentials
 * @param email
 * @param password
 */
void NetworkService::checkCredentials(QString email, QString password)
{
    connected = false;
    QString address = Parameters::URL+
            "/login/"+email+
            "/"+password;
    qDebug() << "check " << address << endl;
    networkAccessManager->get(QNetworkRequest(QUrl(address)));
}

/**
 * @brief NetworkService::sendFile
 * Send the file to the remote server in order to be saved
 * @param dir
 * @return
 */
void NetworkService::sendFile(File *file)
{
    //this->filesToSave->append(&file);
    networkAccessManager->get(QNetworkRequest(QUrl(Parameters::URL+"/save_file/"+user->getEmail()
                                                   +Parameters::NET_REQUEST_SEPARATOR
                                                   +user->getPassword()+Parameters::NET_REQUEST_SEPARATOR
                                                   +file->serialize())));
    qDebug() << Parameters::URL+"/save_file/"+user->getEmail()+"/"+user->getPassword()+"/"+file->serialize() << endl;
}

/**
 * @brief NetworkService::handleBadRequestReply
 * @param reply
 */
void NetworkService::handleBadRequestReply(QNetworkReply * reply)
{
    int requestCode; //= json_map["requestCode"].toInt();
    switch (requestCode) {
    case NetworkService::CODE_REGISTER_USER :{

        break;
    }
    case NetworkService::CODE_USER_LOGIN:{ // login authentification
        connected = true;
        User * user = NULL;
        int resultCode = 0;
        emit credentialsChecked(resultCode, user);
        break;

    }
    case NetworkService::CODE_DB_INIT:{
        connected = false;
        QList<Category> * categories = NULL;
        emit initDataGot(categories);
        break;
    }
    case NetworkService::CODE_DB_REFRESH:{
        break;
    }
    }
}

/**
 * @brief NetworkService::handleGoodRequestReply
 * @param reply
 */
void NetworkService::handleGoodRequestReply(QNetworkReply * reply)
{
    connected = true;
    // Processing the response of the requests sent
    QString response = (QString)reply->readAll();

    // if encryption is used then we decrypt
    if (Parameters::USE_OPENSSL){

    }

    QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();
    QVariantMap json_map = jsonObject.toVariantMap();

    int requestCode = json_map["requestCode"].toInt();
    switch (requestCode) {
    case NetworkService::CODE_REGISTER_USER:{

        break;
    }
    case NetworkService::CODE_USER_LOGIN:{
        User * user = NULL;
        int resultCode = 1;
        if (json_map["success"].toBool()){
            user = Functions::fromJsonToUser(json_map);
        }
        emit credentialsChecked(resultCode, user);
        break;
    }
    case NetworkService::CODE_DB_INIT:{
        QList<Category> * categories = Functions::fromJsonToCategories(json_map);
        emit initDataGot(categories);
        break;
    }
    case NetworkService::CODE_FILE_SAVE:{
        qDebug() << "Reply 'file saved' received" << endl;
        if (json_map["success"].toBool()){
            int fileID = json_map["fileID"].toInt();
            emit fileSaved(fileID);
        }
        break;
    }
    case NetworkService::CODE_DB_REFRESH:{
        break;
    }
    }
}

/**
 * @brief NetworkService::getFreshDbData
 */
void NetworkService::getFreshDbData()
{
    networkAccessManager->get(QNetworkRequest(QUrl(Parameters::URL+"/refresh_db")));
}
