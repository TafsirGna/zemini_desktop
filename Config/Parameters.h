#ifndef ZEMINIPARAMETERS_H
#define ZEMINIPARAMETERS_H

#include <QString>
#include <QSystemTrayIcon>
#include <QDir>
#include <QRegExpValidator>
#include <QRegExp>
#include <QSqlDatabase>
#include <Database/DatabaseManager.h>

class Parameters
{
public:

    const static QString APP_VERSION;

    const static int CHECK_CON_TIME_OUT;

    const static QString WEB_SITE;

    const static QString URL;

    const static int networkTimer1Frequency;

    const static int networkTimer2Frequency;

    //const static QString STORE_DIR;

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

    const static QString THUMBS_DIR_NAME;

    const static QString FILE_SYS_SEPARATOR;

    const static QString KEYS_DIR;

    const static bool USE_OPENSSL;

    const static int MAX_SCREEN_REC_TIME;

    const static int NB_FRAMES_PER_SEC;

    const static int FRAME_COUNT_DIVIDER;

    static QString THUMBS_DIR_PATH;

    // Network request Codes

    const static int CODE_REGISTER_USER;

    const static int CODE_USER_LOGIN;

    const static int CODE_DB_INIT;

    const static int CODE_FILE_SAVE;

    const static int CODE_DB_REFRESH;

    const static int CODE_SAVE_THUMBS;

    const static int CODE_ACCOUNT_CHECKING;

    const static int CODE_APP_VERSION_CHECKING;

    // Database services constants

    const static QString DB_USER;

    const static QString DB_CATEGORY;

    const static QString DB_TYPE;

    const static QString DB_FILE;

    const static QString DB_FILE_TYPE;

    const static QString DB_APP_DATA;

    const static QString DB_DRIVE_TYPE;

    const static QString DB_DRIVE;

    const static QString DB_FILE_FORMAT;

    const static QStringList DB_INIT_DATA_LIST;

    const static QString STARTUP_PROG_FOLDER;

};

#endif // ZEMINIPARAMETERS_H
