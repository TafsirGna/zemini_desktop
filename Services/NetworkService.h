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

class NetworkService : public ZeminiService
{
    Q_OBJECT
private:
        QNetworkAccessManager * networkAccessManager;
        QTimer * timer;
        bool connected;
        int requestCode;

public:
        NetworkService();
        bool isConnected();
        void getInitialDbData();
        void run();
        void checkCredentials(QString, QString);

public slots:
        void handleRequestReply(QNetworkReply*);
        void refreshLocalDb();
        void sendUser(User *);

signals:
        void credentialsChecked(int, User *);
        void initDataGot(QList<Category> *);
};

#endif // NETWORKSERVICE_H
