#include "Config/Parameters.h"

const QString Parameters::APP_VERSION = "0.1";

QString Parameters::THUMBS_DIR_PATH = "";

const QString Parameters::WEB_SITE = "http://192.168.56.1/Symfony/web";

const QString Parameters::URL = Parameters::WEB_SITE+"/desktop";

const int Parameters::networkTimer1Frequency = 15000;

const int Parameters::networkTimer2Frequency = 30000;

const QString Parameters::ROOT_DIR_NAME = "Zemini";

//const QString Parameters::STORE_DIR = QDir().homePath()+"/"+Parameters::ROOT_DIR_NAME;

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

const QString Parameters::THUMBS_DIR_NAME = ".thumbs";

const QString Parameters::KEYS_DIR = Parameters::setupDirectory
        +Parameters::FILE_SYS_SEPARATOR+".keystore";

const bool Parameters::USE_OPENSSL = true;

const int Parameters::FRAME_COUNT_DIVIDER = 64;

// Network Request Codes

const int Parameters::CODE_DB_INIT = 0;

const int Parameters::CODE_REGISTER_USER = 1;

const int Parameters::CODE_USER_LOGIN = 2;

const int Parameters::CODE_FILE_SAVE = 3;

const int Parameters::CODE_DB_REFRESH = 4;

const int Parameters::CODE_SAVE_THUMBS = 6;

const int Parameters::CODE_ACCOUNT_CHECKING = 7;

const int Parameters::CODE_APP_VERSION_CHECKING = 8;

// Database services constants

const QString Parameters::DB_USER = "User";

const QString Parameters::DB_CATEGORY = "Category";

const QString Parameters::DB_FILE = "File";

const QString Parameters::DB_FILE_TYPE = "FileType";

const QString Parameters::DB_APP_DATA = "App_data";

const QString Parameters::DB_DRIVE_TYPE = "drive_type";

const QString Parameters::DB_DRIVE = "Drive";

const QString Parameters::DB_FILE_FORMAT = "FileFormat";

const QString Parameters::STARTUP_PROG_FOLDER = QDir().homePath()+"/AppData/Roaming/Microsoft/Windows/Start Menu/Programs/Startup";

const QStringList Parameters::DB_INIT_DATA_LIST = (QStringList() << Parameters::DB_CATEGORY << Parameters::DB_FILE_FORMAT << Parameters::DB_FILE_TYPE );
