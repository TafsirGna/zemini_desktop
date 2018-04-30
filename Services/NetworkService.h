#ifndef NETWORKSERVICE_H
#define NETWORKSERVICE_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include "Config/Parameters.h"
#include <QWidget>
#include <QNetworkReply>
#include <QDebug>
#include <QTimer>
#include <QtNetwork/QNetworkInterface>
#include <QThread>
#include <Database/Entities/User.h>
#include <QJsonObject>
#include <QJsonDocument>
#include "Config/Functions.h"
#include <QVariantMap>
#include <QList>
#include <Database/Entities/Category.h>
#include <Services/ZeminiService.h>
#include <QSslSocket>
#include "Database/Entities/File.h"
#include <Services/CypherService.h>
#include <Services/LocalDbService.h>
#include <QUrlQuery>
#include <QSslKey>
//#include <QHttpPart>
#include <Config/NetRequest.h>

class NetworkService : public ZeminiService
{
    Q_OBJECT
private:
        QNetworkAccessManager * networkAccessManager;
        QTimer * timer1;
        QTimer * timer2;
        QString private_key;
        QString public_key;
        RSA * keys;
        CypherService * cypherService;
        //Crypto crypto;
        QList<File*> *filesToSend;
        bool firstBackup;
        QSslSocket * sslSocket;
        QList<NetRequest *> * requestsList;

        //Functions
        void handleBadRequestReply(QNetworkReply*);
        void handleGoodRequestReply(QNetworkReply*);
        void settingSslSocket();
        void sendFilePicture(File*);
        void formRequestReply(int, QString, QList<DbEntity> *);
        void formRequestReply(int, QString, DbEntity *);

        //void pingServer();

public:
        // Static constants
        const static int CODE_REGISTER_USER;
        const static int CODE_USER_LOGIN;
        const static int CODE_DB_INIT;
        const static int CODE_FILE_SAVE;
        const static int CODE_DB_REFRESH;
        const static int CODE_PING_SERVER;
        const static int CODE_SAVE_THUMBS;

        NetworkService();
        bool isConnected();
        void getFreshDbData();
        void checkCredentials(QString, QString);

public slots:
        void handleRequestReply(QNetworkReply*);
        void syncDb();
        void sendUser(User *);
        void sendFiles(QList<File*>*);
        void getInitialDbData();

private slots:
        void sslSocketConnected();
        void sendFile(File*);
        void sendNextRequest();

signals:
        //void requestReplyReceived(int requestCode, bool status);
        void requestReplyReceived(QMap<QString, QString> metaData, QList<DbEntity> * data);
        void readyToBackUp();
        void firstBackUpDone();
        void connectionError(int);
        void requestFailed(int);
};

#endif // NETWORKSERVICE_H
