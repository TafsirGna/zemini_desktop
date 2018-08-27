#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QtCore>
#include <QtNetwork>
#include <Config/Functions.h>

#include <cstdio>

//class QSslError;

//using namespace std;

class DownloadManager : public QObject
{
    Q_OBJECT
        QNetworkAccessManager manager;
        QVector<QNetworkReply *> currentDownloads;

    public:
        DownloadManager();
        void doDownload(const QUrl &url);
        static QString saveFileName(const QUrl &url);
        bool saveToDisk(const QString &filename, QIODevice *data);
        static bool isHttpRedirect(QNetworkReply *reply);

    public slots:
        void execute();
        void downloadFinished(QNetworkReply *reply);
        void sslErrors(const QList<QSslError> &errors);
};

#endif // DOWNLOADMANAGER_H
