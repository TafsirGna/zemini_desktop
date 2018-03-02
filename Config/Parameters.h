#ifndef ZEMINIPARAMETERS_H
#define ZEMINIPARAMETERS_H

#include <QString>
#include <QDir>
#include <QRegExpValidator>
#include <QRegExp>
#include <QSqlDatabase>
#include <Database/DatabaseManager.h>

class Parameters
{
public:

    const static QString APP_VERSION;

    const static QString WEB_SITE;

    const static QString URL;

    const static int networkTimerFrequency;

    const static QString STORE_DIR;

    const static QString setupDirectory;

    const static QString iconLocation;

    const static QString devAppFolder;

    const static QString userIcon;

    const static QString timeFormat;

    const static int passwordMinLength;

    const static QRegExp * emailRegExpr;

    const static QSqlDatabase localDb;

    const static QString refreshingIconLocation;

    const static QString NET_REQUEST_SEPARATOR;

    const static QString ROOT_DIR_NAME;

    const static int MIN_CREAT_DELAY;

    const static QString THUMBS_DIR;

    const static QString FILE_SYS_SEPARATOR;

    const static QString KEYS_DIR;

    const static bool USE_OPENSSL;
};

#endif // ZEMINIPARAMETERS_H
