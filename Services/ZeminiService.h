#ifndef ZEMINISERVICE_H
#define ZEMINISERVICE_H

#include "QObject"
#include "Database/Entities/User.h"

class ZeminiService : public QObject
{
    Q_OBJECT

protected:
    User * user;

public:

    // Constants
    static const QString Network;
    static const QString FileSystem;
    static const QString LocalDatabase;
    static const QString Cypher;

    ZeminiService();

    // Getters
    const User * getUser();
    void setUser(User *);
};

#endif // ZEMINISERVICE_H
