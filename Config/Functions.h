#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <Database/Entities/User.h>
#include <QVariantMap>
#include "Config/Parameters.h"
#include <QList>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <Database/Entities/File.h>
#include <QSqlRecord>
#include "Database/EntityManagers/FileTypeManager.h"
#include "Database/EntityManagers/CategoryManager.h"
#include "Database/EntityManagers/FileManager.h"
#include "Database/EntityManagers/DriveManager.h"
#include "Database/EntityManagers/AppDataManager.h"
#include <JlCompress.h>
#include <QMessageBox>
#include <QFileIconProvider>
#include <QSequentialIterable>
#include <QVariant>
#include <QVariantList>
#include <QJsonObject>

class Functions
{
public:
    static User * fromJsonToUser(QMap<QString, QVariant>);

    static bool isEmailValid(QString);

    static QList<Category*> *  fromJsonToCategories(QVariant);

    static QList<FileType*> *  fromJsonToFileTypes(QVariant);

    static QList<FileFormat*> *  fromJsonToFileFormats(QVariant);

    static FileFormat* fromJsonToFileFormat(QVariant);

    static bool makeLinkToRootFolder(QDir);

    static QString getRelativePath(QString);

    static QFileInfo * generateThumbnails(QFileInfo);

    static void extractVideoThumbs(QFileInfo, QDir);

    static bool isVideoFile(QFileInfo);

    static bool isImageFile(QFileInfo);

    static bool isMediaFile(QFileInfo);

    static QString getDriveAbsPath(QString);

    static File * fromSqlRecord2File(QSqlRecord);

    static FileFormat * fromSqlRecord2FileFormat(QSqlRecord);

    static double fromOctect2Ko(int size);

    static double fromOctect2Mo(int);

    static double fromOctect2Go(int);

    static QMap<QString, QString> fromOctect2Normal(int);

    static int getLength(QFileInfo);
};

#endif // FUNCTIONS_H
