#include "Config/Parameters.h"

const QString Parameters::appVersion = "1.0";
const QString Parameters::webSite = "Groovieandroid.com";
const QString Parameters::url = "http://192.168.56.1/Symfony/web/desktop";
const int Parameters::networkTimerFrequency = 10000;
const QString Parameters::storageDirectory = QDir().homePath()+"/Zemini";
const QString Parameters::setupDirectory = QDir().homePath()+"/ZeminiSetup";
const QString Parameters::iconLocation = Parameters::setupDirectory+"/zemini.ico";
const QString Parameters::devAppFolder = "C:\\Users\\Tafsir\\Documents\\QtProjects\\Zemini_desktop";
const QString Parameters::userIcon = Parameters::devAppFolder + "\\Resources\\Images\\user_icon.png";
const QString Parameters::timeFormat = "yyyy-MM-dd hh:mm:ss";
const int Parameters::passwordMinLength = 6;
const QRegExp * Parameters::emailRegExpr = new QRegExp("^[a-z0-9._-]+@[a-z0-9._-]{2,}\\.[a-z]{2,4}$");
QSqlDatabase Parameters::localDb = DatabaseManager::connectToDatabase("ZeminiDB", "root","localhost", 3306, "","QSQLITE", "db");
