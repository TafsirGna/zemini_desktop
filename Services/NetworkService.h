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

class NetworkService : public ZeminiService
{
    Q_OBJECT
private:
        QNetworkAccessManager * networkAccessManager;
        QTimer * timer1;
        QTimer * timer2;
        bool connected;
        QString private_key;
        QString public_key;
        RSA * keys;
        CypherService * cypherService;
        //Crypto crypto;
        QList<File*> *filesToSend;
        bool firstBackup;
        QStringList initDataList;
        //QSslSocket * sslSocket;

        //Functions
        void handleBadRequestReply(QNetworkReply*);
        void handleGoodRequestReply(QNetworkReply*);
        //void pingServer();

        // Static constants
        const static int CODE_REGISTER_USER;
        const static int CODE_USER_LOGIN;
        const static int CODE_DB_INIT;
        const static int CODE_FILE_SAVE;
        const static int CODE_DB_REFRESH;
        const static int CODE_PING_SERVER;

public:
        NetworkService();
        bool isConnected();
        void getInitialDbData();
        void getFreshDbData();
        void checkCredentials(QString, QString);

public slots:
        void handleRequestReply(QNetworkReply*);
        void syncDb();
        void sendUser(User *);
        void sendFiles(QList<File*>*);

private slots:
        void sslSocketConnected();
        void sendFile(File*);

signals:
        void credentialsChecked(int, User *);
        void initDataGot(QList<DbEntity> *, QString);
        void readyToBackUp();
        void fileSaved(int);
        void firstBackUpDone();
};

#endif // NETWORKSERVICE_H
