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
    //Setting the pipe of requests to be sent
    requestsList = new QList<NetRequest *>();
    networkAccessManager = new QNetworkAccessManager(this);
    // variable that tells if a backup has been done yet
    firstBackup = true;

    this->timer1 = new QTimer(this);
    this->timer2 = new QTimer(this);
    filesToSend = new QList<File*>();
    QWidget::connect(networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleRequestReply(QNetworkReply*)));
    sendFilePicture(new File());
}

/***    this check the user's network status    ***/
bool NetworkService::isConnected()
{
    return false;
}

void NetworkService::sslSocketConnected()
{
    qDebug() << "connected yes" << endl;
}

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

void NetworkService::handleRequestReply(QNetworkReply * reply)
{
    if (reply->error() == QNetworkReply::NoError){
        handleGoodRequestReply(reply);
        //after handling the request's response then i remove the current netrequest object from the request list
        if (!requestsList->isEmpty())
            requestsList->removeFirst();
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
    // notify that the request failed
    NetRequest * netRequest = requestsList->first();
    switch (netRequest->getCode()) {
    case CODE_DB_INIT:{
        // i emit a signal to the "Register form" parent object
        qDebug() << "Database initialization failed " << endl;
        emit connectionError(CODE_DB_INIT);
        break;
    }
    default:
        break;
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

    QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();
    QVariantMap json_map = jsonObject.toVariantMap();

    int requestCode = json_map["requestCode"].toInt();
    switch (requestCode) {
    case NetworkService::CODE_REGISTER_USER:{
        User * user = NULL;
        formRequestReply(CODE_REGISTER_USER, "user", user);
        break;
    }
    case NetworkService::CODE_USER_LOGIN:{
        User * user = NULL;
        if (json_map["success"].toBool()){
            user = Functions::fromJsonToUser(json_map);
        }
        formRequestReply(CODE_USER_LOGIN, "user", user);
        break;
    }
    case NetworkService::CODE_DB_INIT:{
        QString tableName = json_map["tableName"].toString();
        qDebug() << "in handle reply function  for db initialization "<< endl;
        if (tableName == LocalDBService::CATEGORY){
            QList<Category> * categories  = Functions::fromJsonToCategories(json_map["data"]);
            qDebug() <<  "Initializing  category table " << endl;
            formRequestReply(CODE_DB_INIT, tableName, (QList<DbEntity> *) categories);
            return;
        }
        if (tableName == LocalDBService::FILE_TYPE){
            qDebug() <<  "Initializing  file_type table " << endl;
            QList<FileType> * fileTypes  = Functions::fromJsonToFileTypes(json_map["data"]);
            formRequestReply(CODE_DB_INIT, LocalDBService::FILE_TYPE, (QList<DbEntity>*) fileTypes);
            return;
        }
        if (tableName == LocalDBService::FILE_FORMAT){
            qDebug() <<  "Initializing  file_format table " << endl;
            QList<FileFormat> * fileFormats  = Functions::fromJsonToFileFormats(json_map["data"]);
            formRequestReply(CODE_DB_INIT, LocalDBService::FILE_FORMAT, (QList<DbEntity> * ) fileFormats);
            return;
        }
        break;
    }
    case NetworkService::CODE_FILE_SAVE:{
        qDebug() << "Reply 'file saved' received" << endl;
        if (json_map["success"].toBool()){
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

void NetworkService::formRequestReply(int code, QString tableName, QList<DbEntity> * data)
{
    QMap<QString , QString> metaData;
    metaData.insert("code", QString::number(code));
    metaData.insert("objectType", tableName);
    emit requestReplyReceived(metaData, data);
}

void NetworkService::formRequestReply(int code, QString tableName, DbEntity * entity)
{
    QMap<QString , QString> metaData;
    metaData.insert("code", QString::number(code));
    metaData.insert("objectType", tableName);
    QList<DbEntity> * data = new QList<DbEntity>();
    data->append(*entity);
    emit requestReplyReceived(metaData, data);
}

/**
 * @brief NetworkService::getFreshDbData
 */
void NetworkService::getFreshDbData()
{
    networkAccessManager->get(QNetworkRequest(QUrl(Parameters::URL+"/refresh_db")));
}
