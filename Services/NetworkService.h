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

class NetworkService : public QThread
{
    Q_OBJECT
private:
        QNetworkAccessManager * networkAccessManager;
        QTimer * timer;
        bool connected;
        QWidget * parent;
        QString requestAction;

public:
        NetworkService(QWidget *);
        bool isConnected();
        void initLocalDb();
        void run();

//signals:
//        void pingReply(bool);

public slots:
        void handleRequestReply(QNetworkReply*);
        void refreshLocalDb();
        void sendUser(User *);
};

#endif // NETWORKSERVICE_H
