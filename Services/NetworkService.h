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

class NetworkService : public ZeminiService
{
    Q_OBJECT
private:
        QNetworkAccessManager * networkAccessManager;
        QTimer * timer;
        bool connected;
        CypherService * cypherService;
        //QList<File*> *filesToSave;
        //QSslSocket * sslSocket;

        //Functions
        void handleBadRequestReply(QNetworkReply*);
        void handleGoodRequestReply(QNetworkReply*);

        // Static constants
        const static int CODE_REGISTER_USER;
        const static int CODE_USER_LOGIN;
        const static int CODE_DB_INIT;
        const static int CODE_FILE_SAVE;
        const static int CODE_DB_REFRESH;

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
        void sendFile(File *file);

private slots:
        void sslSocketConnected();

signals:
        void credentialsChecked(int, User *);
        void initDataGot(QList<Category> *);
        void readyToBackUp();
        void fileSaved(int);
};

#endif // NETWORKSERVICE_H
