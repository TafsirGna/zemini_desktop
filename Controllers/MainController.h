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
    PreferencesForm * preferencesForm;
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

private slots:
    void showDirectory();
    void showRegisterForm();
    void manageActivation(QSystemTrayIcon::ActivationReason);
    void showLogInForm();
    void showPreferences();
    void showZeminiWebSite();
    void setArgs(AbstractController * );
    void startRecording();
    void stop();

public slots:
    bool setUserFolder();

};

#endif // CLASSMAINCONTROLLER_H
