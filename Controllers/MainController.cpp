#include "Controllers/MainController.h"

/*** Default constructor    ***/
MainController::MainController()
{
    serviceContainer = new ServiceContainer();

    // Variable that determine if it's the first launch
    firstLaunch = false;

    // Initialization of all the forms
    registerForm = new RegisterForm(0, this->serviceContainer);
    logInForm = new LogInForm(0, this->serviceContainer);
    trayIcon = new QSystemTrayIcon();

    //Setting connections with the child threads
    QWidget::connect(registerForm,SIGNAL(logInLinkActivated()),this,SLOT(showLogInForm()));
    QWidget::connect(((DirectoryService *)this->serviceContainer->getService(ZeminiService::FileSystem)), SIGNAL(dirContentToUpdate(QFileInfo)), ((LocalDBService *)this->getService(ZeminiService::LocalDatabase)), SLOT(saveDirChange(QFileInfo)));
    QWidget::connect(((DirectoryService *)this->serviceContainer->getService(ZeminiService::FileSystem)), SIGNAL(fileUpdated(QFileInfo)), ((LocalDBService *)this->getService(ZeminiService::LocalDatabase)), SLOT(saveFileChange(QFileInfo)));
    QWidget::connect(((DirectoryService *)this->serviceContainer->getService(ZeminiService::FileSystem)), SIGNAL(fileDeleted(QFileInfo)), ((LocalDBService *)this->getService(ZeminiService::LocalDatabase)), SLOT(saveFileDeletion(QFileInfo)));
    //QWidget::connect(logInForm, SIGNAL(onStart(AbstractController * )), this, SLOT(setArgs(AbstractController *)));
    QWidget::connect(logInForm, SIGNAL(signUpLinkActivated()), this, SLOT(showRegisterForm()));
    QWidget::connect(logInForm, SIGNAL(userToSave(User*)), ((LocalDBService *)this->getService(ZeminiService::LocalDatabase)), SLOT(save(User *)));
    QWidget::connect(((NetworkService *)this->serviceContainer->getService(ZeminiService::Network)), SIGNAL(initDataGot(QList<Category>*)), ((LocalDBService*)this->serviceContainer->getService(ZeminiService::LocalDatabase)), SLOT(initDb(QList<Category> *)));
    QWidget::connect(((LocalDBService *)this->serviceContainer->getService(ZeminiService::LocalDatabase)), SIGNAL(userLoggedIn()), this, SLOT(setUserFolder()));
    QWidget::connect(((DirectoryService *)this->serviceContainer->getService(ZeminiService::FileSystem)), SIGNAL(dirDeleted(QFileInfo)), ((LocalDBService*)this->serviceContainer->getService(ZeminiService::LocalDatabase)), SLOT(saveDirDeletion(QFileInfo)));
    QWidget::connect(((LocalDBService *)this->serviceContainer->getService(ZeminiService::LocalDatabase)), SIGNAL(fileToSend(File *)), ((NetworkService *)this->serviceContainer->getService(ZeminiService::Network)), SLOT(sendFile(File *)));
    QWidget::connect((FileManager*)((LocalDBService *)this->serviceContainer->getService(ZeminiService::LocalDatabase))->getManager(LocalDBService::FILE), SIGNAL(fileSaved(QFileInfo)), ((DirectoryService *)this->serviceContainer->getService(ZeminiService::FileSystem)), SLOT(watchFile(QFileInfo)));
    QWidget::connect((FileManager*)((LocalDBService *)this->serviceContainer->getService(ZeminiService::LocalDatabase))->getManager(LocalDBService::FILE), SIGNAL(fileSaved(File*)), ((NetworkService *)this->serviceContainer->getService(ZeminiService::Network)), SLOT(sendFile(File*)));
    QWidget::connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this, SLOT(manageActivation(QSystemTrayIcon::ActivationReason)));
    QWidget::connect(((DirectoryService *)this->serviceContainer->getService(ZeminiService::FileSystem)), SIGNAL(fileInfoToSave(QFileInfo)), ((LocalDBService*)this->serviceContainer->getService(ZeminiService::LocalDatabase)), SLOT(save(QFileInfo)));
    QWidget::connect(((NetworkService *)this->serviceContainer->getService(ZeminiService::Network)), SIGNAL(readyToBackUp()), ((LocalDBService*)this->serviceContainer->getService(ZeminiService::LocalDatabase)), SLOT(startBackingUp()));
    QWidget::connect(((NetworkService *)this->serviceContainer->getService(ZeminiService::Network)), SIGNAL(fileSaved(int)), ((LocalDBService*)this->serviceContainer->getService(ZeminiService::LocalDatabase)), SLOT(markFileSaved(int)));

    //QWidget::connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(showDirectory()));
}

void MainController::start()
{
    if (!installationCompleted()){
        //complete zemini installation
        firstLaunch = true;

        // Sending a request to get all initial data
        ((NetworkService *) this->getService(ZeminiService::Network))->getInitialDbData();

        completeInstallation();
    }
    else{
        displayTrayIcon();
        // if the user account is not activated
        LocalDBService * localDbService = (LocalDBService *) this->getService(ZeminiService::LocalDatabase);
        UserManager * userManager = (UserManager *) localDbService->getManager(LocalDBService::USER);

        User * user = userManager->getUser();
        if (!user->isActivated()){
            QString title = "Zemini Info";
            QString message = "You've not activated your account yet! We invite you to do so.";
            trayIcon->showMessage(title, message);
        }
        else{
            if (firstLaunch){
                QString title = "Zemini Info";
                QString message = "Welcome to Zemini, Enjoy zemini's features";
                trayIcon->showMessage(title, message);
            }
        }
        ((NetworkService *) this->getService(ZeminiService::Network))->setUser(user);
        ((NetworkService *) this->getService(ZeminiService::Network))->getFreshDbData();
        DirectoryService * directoryService = (DirectoryService *)this->getService(ZeminiService::FileSystem);
        // noticing to the user that we're starting watching the root folder
        directoryService->start();
    }
}

/***    this opens the windows' explorer on the zemini folder   ***/
void MainController::showDirectory()
{
    QString dirPath = Parameters::STORE_DIR;
    QDesktopServices::openUrl(QUrl::fromLocalFile(dirPath.replace("\\","/")));
}


/***    this manages events related to the activation of the system tray entrie ***/
void MainController::manageActivation(QSystemTrayIcon::ActivationReason reason)
{
    trayIcon->contextMenu()->move(QCursor::pos());
    trayIcon->contextMenu()->show();
}

/***    this shows up the preference form on the screen ***/
void MainController::showPreferences()
{
    if (preferencesForm == NULL)
    {
        preferencesForm = new PreferencesForm(0, this->serviceContainer);
        preferencesForm->show();
    }
    else
        preferencesForm->setVisible(true);

    // Connecting signals to slots
    QWidget::connect(preferencesForm, SIGNAL(setupDirectory(QStringList)),this, SLOT(addFileSystemWatcher(QStringList)));
}

/***    this launches a web navigator to the zemini web site url    ***/
void MainController::showZeminiWebSite()
{
    QDesktopServices::openUrl(QUrl(Parameters::WEB_SITE));
}

/***            Once, the user installs the sofware, at the first launch he must complete the pocedure by supplying some informations           ***/
void MainController::completeInstallation()
{
    registerForm->show();
}

void MainController::showLogInForm()
{
    registerForm->hide();
    logInForm->show();
}


void MainController::showRegisterForm()
{
    logInForm->hide();
    registerForm->setVisible(true);
}

bool MainController::installationCompleted()
{
    // Two ways to check the completeness of the installation
    // First, the database status
    if (((LocalDBService * ) this->getService(ZeminiService::LocalDatabase))->isDbEmpty())
        return false;

    // Second, the zemini directory status
    QString dirPath = Parameters::STORE_DIR;
    if (!QDir(dirPath.replace("\\","/")).exists())
        return false;

    return true;
}

void MainController::stop()
{
    // before closing the app, let's save some updates into the db
    LocalDBService * localDbService = (LocalDBService *) this->getService(ZeminiService::LocalDatabase);
    localDbService->update(new AppData(AppData::LAST_EXIT, QDateTime::currentDateTime().toString(Parameters::timeFormat)));

    close();
}

/**
 * @brief MainController::displayTrayIcon
 * @return
 */
bool MainController::displayTrayIcon()
{
    //building the context menu
    QAction * openFolder = new QAction(QIcon(),tr("Open Zemini folder"),this);
    QWidget::connect(openFolder, SIGNAL(triggered()),this, SLOT(showDirectory()));
    QAction * openPreferences = new QAction(QIcon(),tr("Preferences..."),this);
    QWidget::connect(openPreferences, SIGNAL(triggered()), this , SLOT(showPreferences()));
    QAction * quitZemini = new QAction(QIcon(),tr("Quit Zemini"),this);
    QWidget::connect(quitZemini, SIGNAL(triggered()), this , SLOT(stop()));
    QAction * openZeminiweb = new QAction(QIcon(),"Go to Zemini Web",this);
    QWidget::connect(openZeminiweb, SIGNAL(triggered()), this , SLOT(showZeminiWebSite()));
    QAction * startRecord = new QAction(QIcon(),tr("Start recording"),this);
    QWidget::connect(startRecord, SIGNAL(triggered()), this , SLOT(startRecording()));

    QMenu * contextMenu = new QMenu();
    contextMenu->addAction(openFolder);
    contextMenu->addAction(startRecord);
    contextMenu->addAction(contextMenu->addSeparator());
    contextMenu->addAction(openPreferences);
    contextMenu->addAction(openZeminiweb);
    contextMenu->addAction(contextMenu->addSeparator());
    contextMenu->addAction(quitZemini);

    //Setting the context menu to the system tray entrie
    trayIcon->setContextMenu(contextMenu);
    trayIcon->setIcon(QIcon(Parameters::refreshingIconLocation));
    trayIcon->setToolTip("Zemini "+Parameters::APP_VERSION +" \nStarting...");
    trayIcon->show();
}

ZeminiService * MainController::getService(QString service)
{
    return this->serviceContainer->getService(service);
}

bool MainController::checkInitialDbData()
{
    LocalDBService * localDbService = (LocalDBService *) this->getService(ZeminiService::LocalDatabase);
    CategoryManager * categoryManager = (CategoryManager *) localDbService->getManager(LocalDBService::CATEGORY);
    if (categoryManager->isEmpty())
        return false;
    return true;
}

/**
 * @brief MainController::setUserFolder
 * @return
 */
bool MainController::setUserFolder()
{
    LocalDBService * localDbService = (LocalDBService *) this->getService(ZeminiService::LocalDatabase);
    CategoryManager * categoryManager = (CategoryManager *) localDbService->getManager(LocalDBService::CATEGORY);

    // making the directories following the categories
    QList<Category> * categories = categoryManager->getAllCategories();
    ((DirectoryService * ) this->getService(ZeminiService::FileSystem))->makeInitDirectories(categories);
    this->start();
}

void MainController::setArgs(AbstractController * controller)
{
    controller->setServiceContainer(this->serviceContainer);
}

void MainController::startRecording()
{

}


