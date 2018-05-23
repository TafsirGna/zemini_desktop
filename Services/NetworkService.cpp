#include "NetworkService.h"

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
    NetRequest::NetworkAccessManager = networkAccessManager;
    // variable that tells if a backup has been done yet
    firstBackup = true;
    stopUploading = false;
    pauseUploading = false;

    this->timer1 = new QTimer(this);
    this->timer2 = new QTimer(this);
    filesToSend = new QList<File*>();
    QWidget::connect(networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleRequestReply(QNetworkReply*)));
}

// this function sends a request to the remote server to get its initial data to get started
void NetworkService::getInitialDbData()
{
    int list_size = Parameters::DB_INIT_DATA_LIST.size();
    for (int i(0); i < list_size; i++){
        requestsList->append(new NetRequest(Parameters::CODE_DB_INIT, Parameters::DB_INIT_DATA_LIST.at(i)));
    }
    sendNextRequest();
}

void NetworkService::sendNextRequest()
{
    if (pauseUploading)
        return;

    if (stopUploading)
        requestsList->clear();

    //qDebug() << "Finally" << requestsList->size() << stopUploading << endl;
    if (!requestsList->isEmpty()){
        NetRequest * netRequest = requestsList->first();
        netRequest->exec();
    }
}

void NetworkService::onProcessPaused()
{
    pauseUploading = true;
}

void NetworkService::onProcessCancelled()
{
    stopUploading = true;
}

void NetworkService::onProcessResumed()
{
    pauseUploading = false;
}

void NetworkService::onProcessRestarted()
{
    stopUploading = false;
}

void NetworkService::handleRequestReply(QNetworkReply * reply)
{
    if (reply->error() == QNetworkReply::NoError){
        if (!requestsList->isEmpty())
            requestsList->removeFirst();

        handleGoodRequestReply(reply);
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
        emit requestFailed(Parameters::CODE_FILE_SAVE);
        return;
    }
    if (objectType == Parameters::DB_USER){
        User * user = (User *) (data->first());
        registerUser(user);
        return;
    }
    if (objectType == Parameters::DB_FILE){
        int list_size = data->size();
        for (int i(0); i < list_size; i++){
            saveFile((File * ) data->at(i));            
        }
        qDebug() << "files to send received " << data->size() << endl;
        sendNextRequest();
        return;
    }
}

void NetworkService::registerUser(User * user)
{
    qDebug() << "Registering user 0" << endl;
    NetRequest * netRequest = new NetRequest(Parameters::CODE_REGISTER_USER, user);
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
    User * m_user = new User();
    m_user->setEmail(email);
    m_user->setPassword(password);

    if (requestsList->size() > 0){
        emit connectionError(Parameters::CODE_USER_LOGIN);
        return;
    }

    NetRequest * netRequest = new NetRequest(Parameters::CODE_USER_LOGIN, m_user);
    requestsList->append(netRequest);
    sendNextRequest();
}

void NetworkService::checkUserAccount()
{
    NetRequest * netRequest = new NetRequest(Parameters::CODE_ACCOUNT_CHECKING, user);
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
    requestsList->append(new NetRequest(Parameters::CODE_FILE_SAVE, file));
    //networkAccessManager->post((const QNetworkRequest &)*networkRequest, data);
    // if the file has a generated thumbnails
    if (file->getThumbnail() != NULL){
        sendFileThumbnail(file);
    }
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
    if  (requestCode == Parameters::CODE_REGISTER_USER){
        bool success = json_map["success"].toBool();
        if (!success){
            qDebug() << "Emitting request failure signal" << endl;
            emit requestFailed(Parameters::CODE_REGISTER_USER);
        }
        else{
            qDebug() << "Registering user " << json_map << endl;
            User * user = Functions::fromJsonToUser(json_map["user"].toMap());
            //user->toString();
            formRequestReply(Parameters::CODE_REGISTER_USER, Parameters::DB_USER, user);
        }
        return;
    }
    if  (requestCode == Parameters::CODE_USER_LOGIN){
        User * user = NULL;
        if (json_map["success"].toBool()){
            user = Functions::fromJsonToUser(json_map["user"].toMap());
        }
        formRequestReply(Parameters::CODE_USER_LOGIN, Parameters::DB_USER, user);
        return;
    }

    if  (requestCode == Parameters::CODE_DB_INIT){
        QString tableName = json_map["tableName"].toString();
        qDebug() << "in handle reply function  for db initialization "<< tableName << endl;
        if (tableName == Parameters::DB_CATEGORY){
            QList<Category*> * categories  = Functions::fromJsonToCategories(json_map["data"]);
            qDebug() <<  "Initializing  category table " << endl;
            formRequestReply(Parameters::CODE_DB_INIT, tableName, (QList<DbEntity*> *) categories);
            return;
        }
        if (tableName == Parameters::DB_FILE_TYPE){
            qDebug() <<  "Initializing  file_type table " << endl;
            QList<FileType*> * fileTypes  = Functions::fromJsonToFileTypes(json_map["data"]);
            formRequestReply(Parameters::CODE_DB_INIT, Parameters::DB_FILE_TYPE, (QList<DbEntity*>*) fileTypes);
            return;
        }
        if (tableName == Parameters::DB_FILE_FORMAT){
            qDebug() <<  "Initializing  file_format table " << endl;
            QList<FileFormat*> * fileFormats  = Functions::fromJsonToFileFormats(json_map["data"]);
            formRequestReply(Parameters::CODE_DB_INIT, Parameters::DB_FILE_FORMAT, (QList<DbEntity*> * ) fileFormats);
            return;
        }
    }
    if  (requestCode == Parameters::CODE_FILE_SAVE){
        qDebug() << "Reply 'file saved' received" << endl;
        if (json_map["success"].toBool()){
            File * file = new File();
            file->setId(json_map["fileID"].toInt());
            qDebug() << "file ID : " << file->getId() << endl;
            formRequestReply(Parameters::CODE_FILE_SAVE, Parameters::DB_FILE, file);
        }
        return;
    }
    if  (requestCode == Parameters::CODE_DB_REFRESH){
        return;
    }
    if  (requestCode == Parameters::CODE_SAVE_THUMBS){
        qDebug() << "Thumbs saved !" << endl;
        File * file = new File();
        file->setId(json_map["fileID"].toInt());
        formRequestReply(Parameters::CODE_SAVE_THUMBS, Parameters::DB_FILE, (DbEntity *) file);
        return;
    }
    if  (requestCode == Parameters::CODE_ACCOUNT_CHECKING){
        int success = json_map["success"].toBool();
        if (success){
            User * user = new User(this->user);
            user->setActivated(json_map["enabled"].toBool());
            qDebug() << "Checking the account status!" << endl;
            formRequestReply(Parameters::CODE_ACCOUNT_CHECKING, "", user);
        }
        else{
            emit requestFailed(Parameters::CODE_ACCOUNT_CHECKING);
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

/*
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
*/

void NetworkService::sendFileThumbnail(File * file)
{
    NetRequest * netRequest = new NetRequest(Parameters::CODE_SAVE_THUMBS, file);
    //networkAccessManager->post(req,datas); //send all data
    requestsList->append(netRequest);
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

        //after handling the request's response then i remove the current netrequest object from the request list
        if (requestsList->isEmpty()){
            if (firstBackup){
                firstBackup = false;
                emit firstBackUpDone();
            }
            sendThumbnails();
            this->timer1->start(Parameters::networkTimer1Frequency);
        }
*/
