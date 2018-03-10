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
#include <QGuiApplication>
#include <QScreen>
#include <QWindow>

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
    QTimer *screenShotTimer;
    cv::VideoWriter *screenRecordWriter;
    QImage cvMatToQImage( const cv::Mat &inMat );
    QPixmap cvMatToQPixmap( const cv::Mat &inMat );
    int NB_FRAMES_COUNTER;

    // Functions
    void completeInstallation();
    bool installationCompleted();
    bool displayTrayIcon();
    bool checkInitialDbData();
    cv::Mat3b QImage2Mat(const QImage &src);

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
    void recordScreen();
    void stop();
    void takeScreenShot();

public slots:
    bool setUserFolder();

};

#endif // CLASSMAINCONTROLLER_H
