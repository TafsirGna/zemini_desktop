#ifndef NETWORKSERVICE_H
#define NETWORKSERVICE_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include "Config\Parameters.h"
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

class NetworkService : public ZeminiService
{
    Q_OBJECT
private:
        QNetworkAccessManager * networkAccessManager;
        QTimer * timer;
        bool connected;
        int requestCode;
        //QSslSocket * sslSocket;

        //Functions


public:
        NetworkService();
        bool isConnected();
        void getInitialDbData();
        void checkCredentials(QString, QString);

public slots:
        void handleRequestReply(QNetworkReply*);
        void syncDb();
        void sendUser(User *);
        void sendFile(File);

private slots:
        void sslSocketConnected();

signals:
        void credentialsChecked(int, User *);
        void initDataGot(QList<Category> *);
        void readyToBackUp();
};

#endif // NETWORKSERVICE_H
