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
#include <Vendor/qt-firebaseapi-master/firebase.h>

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
        Firebase * firebase;
        bool stopUploading;
        bool pauseUploading;
        File * currentFile;

        //Functions
        void handleBadRequestReply(QNetworkReply*);
        void handleGoodRequestReply(QNetworkReply*);
        void settingSslSocket();
        //void sendThumbnails();
        void sendFileThumbnail(File *file);
        void formRequestReply(int, QString, QList<DbEntity *> *);
        void formRequestReply(int, QString, DbEntity *);
        void registerUser(User *);
        void saveFile(File * file);
        //void pingServer();

public:
        NetworkService();
        //void getFreshDbData();
        void checkCredentials(QString, QString);
        void checkUserAccount();
        void checkNewVersion();
        void downloadNewVersion();

public slots:
        void handleRequestReply(QNetworkReply*);
        void syncDb();
        void send(QString, QList<DbEntity *> *);
        void getInitialDbData();
        void sendNextRequest();
        void onProcessPaused();
        void onProcessCancelled();
        void onProcessResumed();
        void onProcessRestarted();

private slots:
        //void sslSocketConnected();

signals:
        void requestReplyReceived(QMap<QString, QString> metaData, QList<DbEntity*> * data);
        void readyToBackUp();
        void firstBackUpDone();
        void connectionError(int);
        void requestFailed(int);
};

#endif // NETWORKSERVICE_H
