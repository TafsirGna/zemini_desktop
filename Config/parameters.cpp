#include "Config/Parameters.h"

const QString Parameters::APP_VERSION = "1.0";

const QString Parameters::WEB_SITE = "http://192.168.56.1/Symfony/web";

const QString Parameters::URL = Parameters::WEB_SITE+"/desktop";

const int Parameters::networkTimer1Frequency = 15000;

const int Parameters::networkTimer2Frequency = 30000;

const QString Parameters::ROOT_DIR_NAME = "Zemini";

const QString Parameters::STORE_DIR = QDir().homePath()+"/"+Parameters::ROOT_DIR_NAME;

const QString Parameters::setupDirectory = QDir().homePath()+"/ZeminiSetup";

const QString Parameters::iconLocation = Parameters::setupDirectory+"/zemini.png";

const QString Parameters::devAppFolder = "C:\\Users\\Tafsir\\Documents\\QtProjects\\Zemini_desktop";

const QString Parameters::userIcon = Parameters::devAppFolder + "\\Resources\\Images\\user_icon.png";

const QString Parameters::timeFormat = "yyyy-MM-dd hh:mm:ss";

const int Parameters::CHECK_CON_TIME_OUT = 15000;

const int Parameters::MAX_SCREEN_REC_TIME = 30000;

const int Parameters::NB_FRAMES_PER_SEC = 2; //30

const int Parameters::passwordMinLength = 6;

const QRegExp * Parameters::emailRegExpr = new QRegExp("^[a-z0-9._-]+@[a-z0-9._-]{2,}\\.[a-z]{2,4}$");

const QSqlDatabase Parameters::localDb = DatabaseManager::connectToDatabase("ZeminiDB", "root","localhost", 3306, "","QSQLITE", "db");

const QString Parameters::refreshingIconLocation = Parameters::setupDirectory+"/refreshing_icon.png";

const QString Parameters::NET_REQUEST_SEPARATOR = "/";

const int Parameters::MIN_CREAT_DELAY = 1;

const QString Parameters::FILE_SYS_SEPARATOR = "/";

const QString Parameters::THUMBS_DIR = Parameters::STORE_DIR
        +Parameters::FILE_SYS_SEPARATOR+".thumbs";

const QString Parameters::KEYS_DIR = Parameters::setupDirectory
        +Parameters::FILE_SYS_SEPARATOR+".keystore";

const bool Parameters::USE_OPENSSL = true;

const QStringList Parameters::VIDEO_FORMATS = (QStringList() << "avi" << "wmv");

const int Parameters::FRAME_COUNT_DIVIDER = 64;
