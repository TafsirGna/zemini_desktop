#ifndef StaticFileService_H
#define StaticFileService_H

#include <QObject>
#include <QMimeDatabase>

#include "Vendor/qt-http-server-master/lib/QtHttpServer.h"
#include "Vendor/qt-http-server-master/lib/QtHttpReply.h"
#include "Vendor/qt-http-server-master/lib/QtHttpRequest.h"
#include "Vendor/qt-http-server-master/lib/QtHttpHeader.h"

class StaticFileService : public QObject {
    Q_OBJECT

public:
    explicit StaticFileService (QString baseUrl, QObject * parent = NULL);
    virtual ~StaticFileService (void);

public slots:
    void onServerStopped      (void);
    void onServerStarted      (quint16 port);
    void onServerError        (QString msg);
    void onRequestNeedsReply  (QtHttpRequest * request, QtHttpReply * reply);

private:
    QString         m_baseUrl;
    QtHttpServer  * m_server;
    QMimeDatabase * m_mimeDb;
};

#endif // StaticFileService_H
