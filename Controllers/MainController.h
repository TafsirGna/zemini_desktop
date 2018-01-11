#ifndef CLASSMAINCONTROLLER_H
#define CLASSMAINCONTROLLER_H

#include <QSystemTrayIcon>
#include <QDir>
#include <QDebug>
#include <QMessageBox>
#include "Services\DirectoryService.h"
#include <QWidget>
#include <QFile>
#include <QDesktopServices>
#include <QUrl>
#include <QMenu>
#include "Controllers\PreferencesForm.h"
#include "Controllers\RegisterForm.h"
#include "Services\NetworkService.h"
#include "Database\EntityManagers\DatabaseManager.h"
#include "Controllers\LogInForm.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "Database\EntityManagers\DirectoryManager.h"
#include <QFileSystemWatcher>
#include "Services\LocalDbService.h"

class MainController : public QWidget
{
    Q_OBJECT

private:
    QSystemTrayIcon * trayIcon;
    DirectoryService * dirService;
    QMenu * contextMenu;
    ZeminiPreferencesForm * zeminiPreferencesForm;
    RegisterForm * registerForm;
    User * m_user;
    UserManager * userManager;
    FileManager * fileManager;
    DirectoryManager * dirManager;
    TypeManager * typeManager;
    CategoryManager * categoryManager;
    NetworkService * networkService;
    LogInForm * logInForm;
    QSqlDatabase localDb;
    QList<AbstractManager> listEntitiesManager;
    QFileSystemWatcher * fsWatcher;
    QList<QFileSystemWatcher *> fsWatchersList;
    LocalDBService * localDbService;

protected:
    void initializeFSWatcher();

public:
    MainController();
    void start();

    //function that create a the directory if it doesn't exist yet
    void make_directory();

    //function that create a the link to the directory in order to make it as favorite one if it doesn't exist yet
    void make_link();

    // this function start all the processes related to the application  

private slots:
    void completeInstallation();
    bool installationCompleted();

public slots:
    void showDirectory();
    void manageActivation(QSystemTrayIcon::ActivationReason);
    void showPreferences();
    void showZeminiWebSite();
    void handleReplyUserReceived(QNetworkReply *);
    void handleUserData1(User *);
    void handleUserData2(User *);
    void backToWelcomeForm();
    void showLogInForm();
    void handleRecoveryData(User *);
    void handleUserDataDownloaded(QNetworkReply*);
    void updateLocalDatabase(QFileInfoList *fileInfoList);
    void updateRemoteDatabase();
    void handleEndofRequest(QNetworkReply *);
    void handleDirectoryChanges(QString path);
    void handleManagerEndofProcessing(int);
    void addFileSystemWatcher(QFileSystemWatcher * fsWatcher);
    void addFileSystemWatcher(QStringList);
    void showRegisterForm();
};

#endif // CLASSMAINCONTROLLER_H
