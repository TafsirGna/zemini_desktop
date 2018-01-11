#ifndef DIRECTORYSERVICE_H
#define DIRECTORYSERVICE_H

#include <QThread>
#include <QDirIterator>
#include <QDir>
#include <QDebug>
#include "Config\Parameters.h"

class DirectoryService : public QThread
{
    Q_OBJECT
public:
    DirectoryService(QObject * parent); //FolderThread's constructor
    void stop();    // this stops a FolderThread
    void setPath(QString path);

protected:
    void run();
    void checkFolder();

private:
    QObject * parent;
    QString path;
    volatile bool stopped;

signals:
    void directoryChecked(QFileInfoList *);
};

#endif // DIRECTORYSERVICE_H
