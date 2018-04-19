#include "NetworkService.h"

const int NetworkService::CODE_DB_INIT = 0;
const int NetworkService::CODE_REGISTER_USER = 1;
const int NetworkService::CODE_USER_LOGIN = 2;
const int NetworkService::CODE_FILE_SAVE = 3;
const int NetworkService::CODE_DB_REFRESH = 4;
const int NetworkService::CODE_PING_SERVER = 5;
const int NetworkService::CODE_SAVE_THUMBS = 6;

/**
 * @brief NetworkService::NetworkService
 * Builder
 */
NetworkService::NetworkService()
{
    firstBackup = true;
    networkAccessManager = new QNetworkAccessManager(this);
    //cypherService = new CypherService();
    this->connected = true;
    this->timer1 = new QTimer(this);
    this->timer2 = new QTimer(this);
    initDataList = LocalDBService::INIT_DATA_LIST;
    filesToSend = new QList<File*>();

    settingSslSocket();

    //cypherService->encryptRsa("test");
    //cypherService->decryptRsa("ok");
    //cypherService->decryptRsa(cypherService->encryptRsa("test"));

    // Setting connectors
    QWidget::connect(networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleRequestReply(QNetworkReply*)));
    //QWidget::connect(timer1, SIGNAL(timeout()),this, SLOT(syncDb()));

    //QWidget::connect(timer2, SIGNAL(timeout()),this, SLOT(pingServer()));
    //this->timer1->start(Parameters::networkTimer1Frequency);
    //this->timer2->start(Parameters::networkTimer2Frequency);

    sendFilePicture(new File());
}

/***    this check the user's network status    ***/
bool NetworkService::isConnected()
{
    return connected;
}

void NetworkService::sslSocketConnected()
{
    qDebug() << "connected yes" << endl;
}

// this function sends a request to the remote server to get its initial data to get started
void NetworkService::getInitialDbData()
{
    if (!initDataList.isEmpty()){
        connected = false;
        networkAccessManager->get(QNetworkRequest(QUrl(Parameters::URL+"/init_db"+Parameters::NET_REQUEST_SEPARATOR+initDataList.first())));
        //qDebug() << Parameters::URL+"/init_db"+Parameters::NET_REQUEST_SEPARATOR+initDataList.first() << endl;
    }
}

void NetworkService::handleRequestReply(QNetworkReply * reply)
{
    if (reply->error()== QNetworkReply::NoError){
        connected = true;
        handleGoodRequestReply(reply);
        return;
    }
    handleBadRequestReply(reply);
}

void NetworkService::syncDb()
{
    // at this stage, we only backup the database data to a remote db server
    //if (connected)
    qDebug() << "Time out " << endl;
    emit readyToBackUp();
}

void NetworkService::sendUser(User * user){
    QString url(Parameters::URL+"/register");
    QUrlQuery params;
    params.addQueryItem("username", user->getUsername());
    params.addQueryItem("password", user->getPassword());
    params.addQueryItem("email", user->getEmail());

    QByteArray data;
    data.append(params.toString());
    //data.remove(0,1);
    qDebug() << url << " " << data << endl;

    QNetworkRequest* networkRequest = new QNetworkRequest(QUrl(url));
    networkRequest->setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    networkAccessManager->post((const QNetworkRequest &)*networkRequest, data);
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

void NetworkService::sendFiles(QList<File*>* files)
{
    /*
    //turning the list to a json data
    QNetworkRequest networkRequest(QUrl(Parameters::URL+Parameters::NET_REQUEST_SEPARATOR+
                                        "manage_files"+Parameters::NET_REQUEST_SEPARATOR));
    networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject jsonObject;
    jsonObject.insert()
            */

    (*filesToSend) += (*files);
    //if (connected)
    if (!filesToSend->isEmpty())
        sendFile(filesToSend->first());
    else
        if (firstBackup){
            firstBackup = false;
            emit firstBackUpDone();
            this->timer1->start(Parameters::networkTimer1Frequency);
        }
}

/**
 * @brief NetworkService::sendFile
 * Send the file to the remote server in order to be saved
 * @param dir
 * @return
 */
void NetworkService::sendFile(File* file)
{
    QString url(Parameters::URL+"/manage_file");
    QUrlQuery params;
    params.addQueryItem("email", user->getEmail());
    params.addQueryItem("password", user->getPassword());
    file->setRequestParams(params);

    QByteArray data;
    data.append(params.toString());
    qDebug() << "URL_PATH : " << url << data << endl;

    QNetworkRequest* networkRequest = new QNetworkRequest(QUrl(url));
    networkRequest->setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    networkAccessManager->post((const QNetworkRequest &)*networkRequest, data);
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
        User * user = NULL;
        int resultCode = 0;
        emit credentialsChecked(resultCode, user);
        break;

    }
    case NetworkService::CODE_DB_INIT:{
        //QList<Category> * categories = NULL;
        //emit initDataGot(categories, LocalD);
        break;
    }
    case NetworkService::CODE_DB_REFRESH:{
        break;
    }
        //case NetworkService::CODE_PING_SERVER:
        //    break;
    }
}

/**
 * @brief NetworkService::handleGoodRequestReply
 * @param reply
 */
void NetworkService::handleGoodRequestReply(QNetworkReply * reply)
{
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
        emit userSaved(json_map["success"].toBool());
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
        if (!initDataList.isEmpty()){
            if (initDataList.first() == LocalDBService::CATEGORY){
                QList<Category> * categories  = Functions::fromJsonToCategories(json_map);
                emit initDataGot((QList<DbEntity>*)categories, LocalDBService::CATEGORY);
            }
            initDataList.removeFirst();
            getInitialDbData();
        }
        break;
    }
    case NetworkService::CODE_FILE_SAVE:{
        qDebug() << "Reply 'file saved' received" << endl;
        if (json_map["success"].toBool()){
            int fileID = json_map["fileID"].toInt();
            filesToSend->removeFirst();
            //qDebug() << filesToSend << endl;
            if (!filesToSend->isEmpty())
                sendFile(filesToSend->first());
            else
                if (firstBackup){
                    firstBackup = false;
                    emit firstBackUpDone();
                    this->timer1->start(Parameters::networkTimer1Frequency);
                }
            emit fileSaved(fileID);
        }
        break;
    }
    case NetworkService::CODE_DB_REFRESH:{
        break;
    }
    case NetworkService::CODE_PING_SERVER:{
        //connected = true;
        break;
    }
    case NetworkService::CODE_SAVE_THUMBS:{
        qDebug() << "Thumbs saved !" << endl;
        break;
    }
    }
}

void NetworkService::settingSslSocket()
{
    sslSocket = new QSslSocket(this);
    QWidget::connect(sslSocket, SIGNAL(encrypted()), this, SLOT(sslSocketConnected()));
    /*
    QFile file("");
    if (!file.open(QIODevice::ReadOnly)){
        qDebug() << "The file containing the private key can't be opened!" << endl;
        return;
    }
    QSslKey *priv_key = new QSslKey(&file, QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey, "");
    file.close();
    sslSocket->setPrivateKey(*priv_key);
    sslSocket->setLocalCertificate("");
    if (!sslSocket->addCaCertificates("")){
        qDebug() << "Can't open the certificate" << endl;
        return;
    }

    sslSocket->setPeerVerifyMode(QSslSocket::VerifyNone);
    sslSocket->ignoreSslErrors();
    sslSocket->abort();
    */
    sslSocket->connectToHostEncrypted(Parameters::URL, 443);
    //sslSocket->startClientEncryption();
    qDebug() << "encrypt ok" << endl;
}

void NetworkService::sendFilePicture(File * f)
{
    QFile file("C:/Users/Tafsir/Zemini/.thumbs/Wildlife.png"); //lets get the file by filename
    if (!file.open(QIODevice::ReadOnly)) //accessibility controll for file
    {
        qDebug() << "file open failure"; //send message if file cant open
    }
    QByteArray line = file.readAll();
    //we read file line by line with no error handling for reading time!!

    file.close();
    QByteArray boundary; //actually i cant understand that why we are using a second byte array for file sending.
    // if someone know this trick please write below. I write this code like the other examples.

    QByteArray datas(QString("--" + boundary + "\r\n").toUtf8());
    datas += "Content-Disposition: form-data; name=\"file\"; filename=\""+file.fileName()+"\"\r\n";
    //here is the http header for manuplate a normal http form and form file object

    datas += "Content-Type: image/png\r\n\r\n"; //file type is here
    datas += line; //and our file is giving to form object
    datas += "\r\n";
    datas += QString("--" + boundary + "\r\n\r\n").toUtf8();
    datas += "Content-Disposition: form-data; name=\"upload\"\r\n\r\n";
    datas += "Uploader\r\n";
    datas += QString("--" + boundary + "--\r\n").toUtf8();

    QNetworkRequest req;
    req.setUrl(QUrl(Parameters::URL+Parameters::NET_REQUEST_SEPARATOR+"save_thumbnails")); //my virtual servers' ip address and tiny php page url is here
    req.setRawHeader("Content-Type", "multipart/form-data; boundary=" + boundary); // we must set the first header like this. its tell the server, current object is a form
    networkAccessManager->post(req,datas); //send all data
}

/**
 * @brief NetworkService::getFreshDbData
 */
void NetworkService::getFreshDbData()
{
    networkAccessManager->get(QNetworkRequest(QUrl(Parameters::URL+"/refresh_db")));
}

/*
void NetworkService::pingServer()
{

}
*/
