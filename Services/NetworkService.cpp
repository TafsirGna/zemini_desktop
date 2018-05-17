#include "NetworkService.h"

const int NetworkService::CODE_DB_INIT = 0;
const int NetworkService::CODE_REGISTER_USER = 1;
const int NetworkService::CODE_USER_LOGIN = 2;
const int NetworkService::CODE_FILE_SAVE = 3;
const int NetworkService::CODE_DB_REFRESH = 4;
const int NetworkService::CODE_PING_SERVER = 5;
const int NetworkService::CODE_SAVE_THUMBS = 6;
const int NetworkService::CODE_ACCOUNT_CHECKING = 7;

/**
 * @brief NetworkService::NetworkService
 * Builder
 */
NetworkService::NetworkService()
{
    //Setting the pipe of requests to be sent
    requestsList = new QList<NetRequest *>();
    firebase = new Firebase("https://zemini-e1819.firebaseio.com/");
    networkAccessManager = new QNetworkAccessManager(this);
    // variable that tells if a backup has been done yet
    firstBackup = true;

    this->timer1 = new QTimer(this);
    this->timer2 = new QTimer(this);
    filesToSend = new QList<File*>();
    QWidget::connect(networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleRequestReply(QNetworkReply*)));
}

/*
void NetworkService::sslSocketConnected()
{
    qDebug() << "connected yes" << endl;
}
*/

// this function sends a request to the remote server to get its initial data to get started
void NetworkService::getInitialDbData()
{
    int list_size = LocalDBService::INIT_DATA_LIST.size();
    for (int i(0); i < list_size; i++){
        QNetworkRequest request(QUrl(Parameters::URL+Parameters::NET_REQUEST_SEPARATOR+"init_db"+
                                     Parameters::NET_REQUEST_SEPARATOR+LocalDBService::INIT_DATA_LIST.at(i)));
        request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
        requestsList->append(new NetRequest(CODE_DB_INIT, "POST", request));
    }
    sendNextRequest();
}

void NetworkService::sendNextRequest()
{
    if (!requestsList->isEmpty()){
        NetRequest * netRequest = requestsList->first();
        qDebug() << "sending the next request : " << netRequest->getNetworkRequest().url().toString() << endl;
        if (netRequest->getType() == "POST")
            networkAccessManager->post(netRequest->getNetworkRequest(), netRequest->getData());
        else if (netRequest->getType() == "GET"){
            networkAccessManager->get(netRequest->getNetworkRequest());
        }
    }
}

void NetworkService::pauseProcess()
{

}

void NetworkService::cancelProcess()
{

}

void NetworkService::handleRequestReply(QNetworkReply * reply)
{
    if (reply->error() == QNetworkReply::NoError){
        if (!requestsList->isEmpty())
            requestsList->removeFirst();

        handleGoodRequestReply(reply);
        //after handling the request's response then i remove the current netrequest object from the request list
        if (requestsList->isEmpty()){
            if (firstBackup){
                firstBackup = false;
                emit firstBackUpDone();
            }
            sendThumbnails();
            this->timer1->start(Parameters::networkTimer1Frequency);
        }
    }
    else{
        handleBadRequestReply(reply);
        return;
    }
    sendNextRequest();
}

void NetworkService::syncDb()
{
    // at this stage, we only backup the database data to a remote db server
    //if (connected)
    qDebug() << "Time out " << endl;
    emit readyToBackUp();
}

void NetworkService::send(QString objectType, QList<DbEntity *> * data)
{
    if (!user->isActivated()){
        emit requestFailed(CODE_FILE_SAVE);
        return;
    }
    if (objectType == LocalDBService::USER){
        User * user = (User *) (data->first());
        registerUser(user);
        return;
    }
    if (objectType == LocalDBService::FILE){
        int list_size = data->length();
        for (int i(0); i < list_size; i++){
            saveFile((File * ) data->at(i));
        }
        sendNextRequest();
        return;
    }
}

void NetworkService::registerUser(User * user)
{
    qDebug() << "Registering user 0" << endl;
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
    NetRequest * netRequest = new NetRequest(CODE_REGISTER_USER, QString("POST"), *networkRequest, data);
    requestsList->append(netRequest);

    sendNextRequest();

}

/**
 * @brief NetworkService::checkCredentials
 * @param email
 * @param password
 */
void NetworkService::checkCredentials(QString email, QString password)
{
    if (requestsList->size() > 0){
        emit connectionError(NetworkService::CODE_USER_LOGIN);
        return;
    }

    QString address = Parameters::URL+
            "/login?email="+email+"&password="+password;
    qDebug() << "check " << address << endl;

    NetRequest * netRequest = new NetRequest(CODE_USER_LOGIN, "GET", QNetworkRequest(QUrl(address)));
    requestsList->append(netRequest);
    sendNextRequest();
}

void NetworkService::checkUserAccount()
{
    QNetworkRequest request(QUrl(Parameters::URL+"/account_confirmation?email="+user->getEmail()+"&password="+user->getPassword()));
    NetRequest * netRequest = new NetRequest(CODE_ACCOUNT_CHECKING, "GET", request);
    requestsList->append(netRequest);

    sendNextRequest();
}

/*
void NetworkService::sendFiles(QList<File*>* files)
{

    //turning the list to a json data
    QNetworkRequest networkRequest(QUrl(Parameters::URL+Parameters::NET_REQUEST_SEPARATOR+
                                        "manage_files"+Parameters::NET_REQUEST_SEPARATOR));
    networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject jsonObject;
    jsonObject.insert()


    (*filesToSend) += (*files);
    if (!filesToSend->isEmpty())
        sendFile(filesToSend->first());
    else
        if (firstBackup){
            firstBackup = false;
            emit firstBackUpDone();
            this->timer1->start(Parameters::networkTimer1Frequency);
        }
}
*/

/**
 * @brief NetworkService::sendFile
 * Send the file to the remote server in order to be saved
 * @param dir
 * @return
 */

void NetworkService::saveFile(File* file)
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

    requestsList->append(new NetRequest(CODE_FILE_SAVE, "POST", *networkRequest, data));
    //networkAccessManager->post((const QNetworkRequest &)*networkRequest, data);
}

/**
 * @brief NetworkService::handleBadRequestReply
 * @param reply
 */
void NetworkService::handleBadRequestReply(QNetworkReply * reply)
{
    // notify that the request failed
    NetRequest * netRequest = requestsList->first();
    emit connectionError(netRequest->getCode());
}

/**
 * @brief NetworkService::handleGoodRequestReply
 * @param reply
 */
void NetworkService::handleGoodRequestReply(QNetworkReply * reply)
{
    // Processing the response of the requests sent
    QString response = (QString)reply->readAll();

    QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();
    QVariantMap json_map = jsonObject.toVariantMap();

    int requestCode = json_map["requestCode"].toInt();
    switch (requestCode) {
    case NetworkService::CODE_REGISTER_USER:{
        bool success = json_map["success"].toBool();
        if (!success){
            qDebug() << "Emitting request failure signal" << endl;
            emit requestFailed(CODE_REGISTER_USER);
        }
        else{
            qDebug() << "Registering user " << json_map << endl;
            User * user = Functions::fromJsonToUser(json_map["user"].toMap());
            user->toString();
            formRequestReply(CODE_REGISTER_USER, LocalDBService::USER, user);
        }
        break;
    }
    case NetworkService::CODE_USER_LOGIN:{
        User * user = NULL;
        if (json_map["success"].toBool()){
            user = Functions::fromJsonToUser(json_map["user"].toMap());
        }
        formRequestReply(CODE_USER_LOGIN, LocalDBService::USER, user);
        break;
    }
    case NetworkService::CODE_DB_INIT:{
        QString tableName = json_map["tableName"].toString();
        qDebug() << "in handle reply function  for db initialization "<< tableName << endl;
        if (tableName == LocalDBService::CATEGORY){
            QList<Category*> * categories  = Functions::fromJsonToCategories(json_map["data"]);
            qDebug() <<  "Initializing  category table " << endl;
            formRequestReply(CODE_DB_INIT, tableName, (QList<DbEntity*> *) categories);
            return;
        }
        if (tableName == LocalDBService::FILE_TYPE){
            qDebug() <<  "Initializing  file_type table " << endl;
            QList<FileType*> * fileTypes  = Functions::fromJsonToFileTypes(json_map["data"]);
            formRequestReply(CODE_DB_INIT, LocalDBService::FILE_TYPE, (QList<DbEntity*>*) fileTypes);
            return;
        }
        if (tableName == LocalDBService::FILE_FORMAT){
            qDebug() <<  "Initializing  file_format table " << endl;
            QList<FileFormat*> * fileFormats  = Functions::fromJsonToFileFormats(json_map["data"]);
            formRequestReply(CODE_DB_INIT, LocalDBService::FILE_FORMAT, (QList<DbEntity*> * ) fileFormats);
            return;
        }
        break;
    }
    case NetworkService::CODE_FILE_SAVE:{
        qDebug() << "Reply 'file saved' received" << endl;
        if (json_map["success"].toBool()){
            File * file;
            file->setId(json_map["fileID"].toInt());
            formRequestReply(CODE_FILE_SAVE, json_map["tableName"].toString(), (DbEntity *) file);
        }
        break;
    }
    case NetworkService::CODE_DB_REFRESH:{
        break;
    }
    case NetworkService::CODE_PING_SERVER:{
        break;
    }
    case NetworkService::CODE_SAVE_THUMBS:{
        qDebug() << "Thumbs saved !" << endl;
        break;
    }
    case NetworkService::CODE_ACCOUNT_CHECKING:{
        int success = json_map["success"].toBool();
        if (success){
            User * user = new User(this->user);
            user->setActivated(json_map["enabled"].toBool());
            qDebug() << "Checking the account status!" << endl;
            formRequestReply(CODE_ACCOUNT_CHECKING, "", user);
        }
        else{
            emit requestFailed(CODE_ACCOUNT_CHECKING);
        }
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

void NetworkService::sendThumbnails()
{
    QDir thumbsDir(Parameters::THUMBS_DIR_PATH);
    if (thumbsDir.exists()){
        // counting the numbers of thumbnails to send
        QFileInfoList thumbnail_list = thumbsDir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
        int list_size = thumbnail_list.size();
        if (list_size == 0){
            qDebug() << "No thumbnails to send !" << endl;
            return;
        }

        for (int i(0); i < list_size; i++){
            sendFilePicture(thumbnail_list.at(i));
        }
    }
}

void NetworkService::sendFilePicture(QFileInfo fileInfo)
{
    QFile file(fileInfo.absoluteFilePath()); //lets get the file by filename
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

    NetRequest * netRequest = new NetRequest(CODE_SAVE_THUMBS, "POST", req, datas);
    //networkAccessManager->post(req,datas); //send all data
}

void NetworkService::formRequestReply(int code, QString tableName, QList<DbEntity*> * data)
{
    QMap<QString , QString> metaData;
    metaData.insert("code", QString::number(code));
    if (tableName != "")
        metaData.insert("objectType", tableName);

    emit requestReplyReceived(metaData, data);
}

void NetworkService::formRequestReply(int code, QString tableName, DbEntity * entity)
{
    QMap<QString , QString> metaData;
    metaData.insert("code", QString::number(code));
    if (tableName != "")
        metaData.insert("objectType", tableName);

    QList<DbEntity*> * data = new QList<DbEntity*>();
    data->append(entity);
    emit requestReplyReceived(metaData, data);
}

/*
void NetworkService::getFreshDbData()
{
    networkAccessManager->get(QNetworkRequest(QUrl(Parameters::URL+"/refresh_db")));
}
*/
