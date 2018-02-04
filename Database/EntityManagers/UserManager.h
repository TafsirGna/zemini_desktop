#ifndef CLASSUSERMANAGER_H
#define CLASSUSERMANAGER_H

#include "Database\EntityManagers\AbstractManager.h"
#include "Database\Entities\User.h"
#include <QUrlQuery>
#include <QPair>

class UserManager : public AbstractManager
{
    Q_OBJECT
private:
    User * user;

public:
    //Constructors
    UserManager(QSqlDatabase &db);

    //Prototypes of the main functions of this class
    bool insertUser(User user);
    void updateUser(User user);
    void deleteUser(User user);
    User * getUser();
    void downloadUser(User * user);

signals:
    void replyUserReceived(QNetworkReply *);
    void criticalError();
    void replyUserDownload(QNetworkReply *);

public slots:
        //void handleReplyUserRequest(QNetworkReply *);
        //void handleReplyPingFinished(bool);
};

#endif // CLASSUSERMANAGER_H
