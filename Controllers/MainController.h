#ifndef CLASSMAINCONTROLLER_H
#define CLASSMAINCONTROLLER_H

#include <QSystemTrayIcon>
#include <QDir>
#include <QDebug>
#include <QMessageBox>
#include <QWidget>
#include <QFile>
#include <QDesktopServices>
#include <QUrl>
#include <QMenu>
#include "Controllers/PreferencesForm.h"
#include "Controllers/RegisterForm.h"
#include "Database/DatabaseManager.h"
#include "Controllers/LogInForm.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileSystemWatcher>
#include "Services/ServiceContainer.h"

class MainController : public QWidget
{
    Q_OBJECT

private:
    // Variables
    QSystemTrayIcon * trayIcon;
    ZeminiPreferencesForm * zeminiPreferencesForm;
    RegisterForm * registerForm;
    User * m_user;
    LogInForm * logInForm;
    QSqlDatabase localDb;
    bool firstLaunch;
    ServiceContainer * serviceContainer;


    // Functions
    void completeInstallation();
    bool installationCompleted();
    bool displayTrayIcon();
    bool checkInitialDbData();

public:
    MainController();
    void start();
    ZeminiService * getService(QString);
    /*
    void handleReplyUserReceived(QNetworkReply *);
    void handleUserData1(User *);
    void handleUserData2(User *);
    void backToWelcomeForm();
    void handleRecoveryData(User *);
    void updateLocalDatabase(QFileInfoList *);
    void updateRemoteDatabase();
    void handleEndofRequest(QNetworkReply *);
    void handleDirectoryChanges(QString path);
    void handleManagerEndofProcessing(int);
    void addFileSystemWatcher(QFileSystemWatcher *);
    void addFileSystemWatcher(QStringList);
    void initializeFSWatcher();
    */

private slots:
    void showDirectory();
    void showRegisterForm();
    void manageActivation(QSystemTrayIcon::ActivationReason);
    void showLogInForm();
    void insertInitDbData(QList<Category> * );
    void showPreferences();
    void showZeminiWebSite();
    void setArgs(AbstractController * );

public slots:
    bool setUserFolder();


};

#endif // CLASSMAINCONTROLLER_H
