#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <Database/Entities/User.h>
#include <QVariantMap>
#include "Config/Parameters.h"
#include <QList>
#include <Database/Entities/Category.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <Database/Entities/File.h>
#include <QSqlRecord>
#include <Database/EntityManagers/FileTypeManager.h>
#include <Database/EntityManagers/FileManager.h>
#include <Database/EntityManagers/DriveManager.h>
#include <Database/EntityManagers/CategoryManager.h>

class Functions
{
public:
    static User * fromJsonToUser(QVariantMap);

    static bool isEmailValid(QString);

    static QList<Category> *  fromJsonToCategories(QVariantMap);

    static void makeLinkToZeminiFolder();

    static QString getRelativePath(QString);

    static QFileInfo * generateThumbnails(QFileInfo);

    static bool isVideoFile(QFileInfo);

    static QString getDriveAbsPath(QString);

    static File * fromSqlRecord2File(QSqlRecord);
};

#endif // FUNCTIONS_H
