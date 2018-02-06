#ifndef ZEMINISERVICE_H
#define ZEMINISERVICE_H

#include "QObject"

class ZeminiService : public QObject
{
    Q_OBJECT
public:

    // Constants
    static const QString Network;
    static const QString FileSystem;
    static const QString LocalDatabase;

    ZeminiService();
};

#endif // ZEMINISERVICE_H
