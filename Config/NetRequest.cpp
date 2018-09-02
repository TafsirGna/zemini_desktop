#include "NetRequest.h"

QNetworkAccessManager *NetRequest::NetworkAccessManager = NULL;

QString NetRequest::getType() const
{
    return type;
}

void NetRequest::setType(const QString &value)
{
    type = value;
}

QByteArray NetRequest::getData() const
{
    return data;
}

void NetRequest::setData(const QByteArray &value)
{
    data = value;
}

int NetRequest::getCode() const
{
    return code;
}

void NetRequest::setCode(int value)
{
    code = value;
}

DbEntity *NetRequest::getEntity() const
{
    return entity;
}

void NetRequest::setEntity(DbEntity *value)
{
    entity = value;
}

QString NetRequest::getEntityClass() const
{
    return entityClass;
}

void NetRequest::setEntityClass(const QString &value)
{
    entityClass = value;
}

QNetworkRequest *NetRequest::getNetworkRequest() const
{
    return networkRequest;
}

void NetRequest::setNetworkRequest(QNetworkRequest *value)
{
    networkRequest = value;
}

NetRequest::NetRequest()
{
    code = -1;
    networkRequest = NULL;
    type = "";
    entity = NULL;
    entityClass = "";
    data = NULL;
}

NetRequest::NetRequest(int code, QString tableName)
{
    // default values
    networkRequest = NULL;
    type = "";
    entity = NULL;
    entityClass = tableName;
    data = NULL;
    this->code = code;

    if (code == Parameters::CODE_DB_INIT) {
        this->type = "GET";
        this->networkRequest = new QNetworkRequest(QUrl(Parameters::URL+Parameters::NET_REQUEST_SEPARATOR+"init_db"+
                                                        Parameters::NET_REQUEST_SEPARATOR+tableName));
        this->networkRequest->setSslConfiguration(QSslConfiguration::defaultConfiguration());
    }

    if (code == Parameters::CODE_APP_VERSION_CHECKING){
        this->type = "GET";
        this->networkRequest = new QNetworkRequest(QUrl(Parameters::URL+Parameters::NET_REQUEST_SEPARATOR+"check_desktop_version"));
        this->networkRequest->setSslConfiguration(QSslConfiguration::defaultConfiguration());
    }
}

NetRequest::NetRequest(int code , DbEntity * entity)
{
    // default values
    this->code = code;
    networkRequest = NULL;
    type = "";
    this->entity = entity;
    entityClass = "";
    data = NULL;

    if (code == Parameters::CODE_USER_LOGIN){
        this->type = "GET";
        this->entityClass = Parameters::DB_USER;

        User * user = (User * ) entity;
        QString address = Parameters::URL+
                "/login?email="+user->getEmail()+"&password="+user->getPassword();
        qDebug() << "check " << address << endl;
        this->networkRequest = new QNetworkRequest(QUrl(address));
    }

    if (code == Parameters::CODE_REGISTER_USER){
        this->type = "POST";
        this->entityClass = Parameters::DB_USER;

        User * user = (User *) entity;
        QString url(Parameters::URL+"/register");
        QUrlQuery params;
        params.addQueryItem("username", user->getUsername());
        params.addQueryItem("password", user->getPassword());
        params.addQueryItem("email", user->getEmail());

        QByteArray data;
        data.append(params.toString());
        qDebug() << url << " " << data << endl;

        this->networkRequest = new QNetworkRequest(QUrl(url));
        this->networkRequest->setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    }

    if (code == Parameters::CODE_ACCOUNT_CHECKING){
        this->type = "GET";
        this->entityClass = Parameters::DB_USER;

        User * user = (User *) entity;
        this->networkRequest = new QNetworkRequest(QUrl(Parameters::URL+"/account_confirmation?email="+user->getEmail()+"&password="+user->getPassword()));
    }

    if (code == Parameters::CODE_FILE_SAVE){

        User * user = UserManager::getUser();
        File * file = (File *) entity;
        this->type = "POST";
        this->entityClass = Parameters::DB_FILE;

        QString url(Parameters::URL+"/manage_file");
        QUrlQuery params;
        params.addQueryItem("email", user->getEmail());
        params.addQueryItem("password", user->getPassword());
        file->setRequestParams(params);

        data.append(params.toString());
        qDebug() << "URL_PATH : " << url << data << endl;

        networkRequest = new QNetworkRequest(QUrl(url));
        networkRequest->setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    }
}

NetRequest::NetRequest(int code, QFileInfo fileInfo)
{
    if (code == Parameters::CODE_SAVE_THUMBS){

        QMap<QString, QString> fileProperties;
        fileProperties.insert("thumbnail", fileInfo.fileName());
        File * file = FileManager::getOneBy(fileProperties);

        this->type = "POST";
        this->entityClass = Parameters::DB_FILE;
        this->entity = (DbEntity * ) file;

        //qDebug() << "thumbnail file : " << file->getFileName() << " / " << ((File*) entity)->getThumbnail()->absoluteFilePath() << endl;
        QFile qfile(fileInfo.absoluteFilePath()); //lets get the file by filename
        if (!qfile.open(QIODevice::ReadOnly)) //accessibility controll for file
        {
            qDebug() << "file open failure" << endl; //send message if file cant open
        }

        QByteArray line = qfile.readAll();
        //we read file line by line with no error handling for reading time!!

        qfile.close();
        QByteArray boundary; //actually i cant understand that why we are using a second byte array for file sending.
        // if someone know this trick please write below. I write this code like the other examples.

        QByteArray datas(QString("--" + boundary + "\r\n").toUtf8());
        datas += "Content-Disposition: form-data; name=\"file\"; filename=\""+qfile.fileName()+"\"\r\n";
        //here is the http header for manuplate a normal http form and form file object

        datas += "Content-Type: image/png\r\n\r\n"; //file type is here
        datas += line; //and our file is giving to form object
        datas += "\r\n";
        datas += QString("--" + boundary + "\r\n\r\n").toUtf8();
        datas += "Content-Disposition: form-data; name=\"upload\"\r\n\r\n";
        datas += "Uploader\r\n";
        datas += QString("--" + boundary + "--\r\n").toUtf8();

        this->data = datas;
        User * user = UserManager::getUser();

        QUrlQuery params;
        params.addQueryItem("email", user->getEmail());
        params.addQueryItem("password", user->getPassword());
        file->setRequestParams(params);
        qDebug() << "datas : " << params.toString() << endl;

        networkRequest = new QNetworkRequest(QUrl(Parameters::URL+Parameters::NET_REQUEST_SEPARATOR+"save_thumbnails?"+params.toString()));
        networkRequest->setRawHeader("Content-Type", "multipart/form-data; boundary=" + boundary); // we must set the first header like this. its tell the server, current object is a form
    }
}

void NetRequest::exec()
{
    qDebug() << "In exec function " << networkRequest->url() << " -: " << type <<  endl;
    if (type == "POST"){
        qDebug() << "POST / Url : " << networkRequest->url() << " ; Data : " << data << endl;
        NetworkAccessManager->post(*networkRequest, data);
    }
    else if (type == "GET"){
        qDebug() << "GET / Url : " << networkRequest->url() << endl;
        NetworkAccessManager->get(*networkRequest);
        qDebug() << "Getting data " << endl;
    }
}


