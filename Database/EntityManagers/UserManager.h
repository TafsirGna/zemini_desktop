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
    UserManager();

    //Prototypes of the main functions of this class
    static User * add(User user);
    static User * update(User user);
    static bool remove(User user);

    static User * getUser();

};

#endif // CLASSUSERMANAGER_H
