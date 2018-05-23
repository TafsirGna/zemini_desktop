#ifndef CLASSMAINCONTROLLER_H
#define CLASSMAINCONTROLLER_H

#include <QDir>
#include <QDebug>
#include <QMessageBox>
#include <QWidget>
#include <QFile>
#include <QDesktopServices>
#include <QUrl>
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
#include <Controllers/AboutForm.h>
#include <Controllers/UploadingForm.h>

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
    AboutForm *aboutForm;
    UploadingForm * uploadingForm;
    int totalSize;
    int filesDoneSize;
    bool pausedProcess;

    // Functions
    void completeInstallation();
    bool installationCompleted();
    bool checkInitialDbData();
    cv::Mat3b QImage2Mat(const QImage &src);

public:
    MainController();
    ZeminiService * getService(QString);

private slots:
    void showDirectory();
    void showAboutForm();
    void showRegisterForm();
    void manageActivation(QSystemTrayIcon::ActivationReason);
    void showLogInForm();
    void showPreferences();
    void showZeminiWebSite();
    void showUploadingForm();
    void setArgs(AbstractController * );
    void recordScreen();
    void stop();
    void takeScreenShot();
    void onFirstBackUpDone();
    void onRequestFailed(int);
    void onConnectionError(int);
    void onUserEnabled(bool);
    void checkAccountConfirmation();
    void onProcessPaused();
    void onProcessResumed();
    void onProcessCancelled();
    void onProcessRestarted();

public slots:
    void start();

signals:
    void quitAppSignal();

};

#endif // CLASSMAINCONTROLLER_H
