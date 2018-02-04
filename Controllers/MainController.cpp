#include "Controllers\MainController.h"

/*** Default constructor    ***/
MainController::MainController()
{
    serviceContainer = new ServiceContainer();

    // Variable that determine if it's the first launch
    firstLaunch = false;

    // Initialization of all the forms
    registerForm = new RegisterForm(0);
    logInForm = new LogInForm(0);
    trayIcon = new QSystemTrayIcon();

    //Setting connections with the child threads
    QWidget::connect(registerForm,SIGNAL(logInLinkActivated()),this,SLOT(showLogInForm()));
    QWidget::connect(logInForm, SIGNAL(signUpLinkActivated()), this, SLOT(showRegisterForm()));
    QWidget::connect(this->serviceContainer->getService(ZeminiService::Network), SIGNAL(initDataGot(QList<Category>*)), this, SLOT(insertInitDbData(QList<Category> *)));
    QWidget::connect(logInForm, SIGNAL(userLoggedIn()), this, SLOT(start()));
    QWidget::connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this, SLOT(manageActivation(QSystemTrayIcon::ActivationReason)));
    QWidget::connect(this->serviceContainer->getService(ZeminiService::FileSystem), SIGNAL(fileInfoToSave(QFileInfo)), this->serviceContainer->getService(ZeminiService::LocalDatabase), SLOT(save(QFileInfo)));
    //QWidget::connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(showDirectory()));

    /*
    QWidget::connect(registerForm, SIGNAL(userDataEntered(User*)),this,SLOT(handleUserData1(User*)));
    QWidget::connect(userManager, SIGNAL(replyUserReceived(QNetworkReply*)),this,SLOT(handleReplyUserReceived(QNetworkReply*)));
    QWidget::connect(userManager, SIGNAL(criticalError()),this,SLOT(close()));
    QWidget::connect(userManager, SIGNAL(replyUserDownload(QNetworkReply*)),this,SLOT(handleUserDataDownloaded(QNetworkReply*)));
    QWidget::connect(dirService, SIGNAL(directoryChecked(QFileInfoList *)), this, SLOT(updateLocalDatabase(QFileInfoList *)));
    QWidget::connect(localDbService,SIGNAL(dataSavedLocally()),this, SLOT(updateRemoteDatabase()));
    QWidget::connect(dirManager, SIGNAL(remoteUpdatesDone(int)),this, SLOT(handleManagerEndofProcessing(int)));
    QWidget::connect(fileManager, SIGNAL(remoteUpdatesDone(int)),this, SLOT(handleManagerEndofProcessing(int)));
    */
}

void MainController::start()
{
    if (!installationCompleted()){
        //complete zemini installation
        firstLaunch = true;
        completeInstallation();
    }
    else{

        displayTrayIcon();
        // if the user account is not activated
        LocalDBService * localDbService = (LocalDBService *) this->getService(ZeminiService::LocalDatabase);
        UserManager * userManager = (UserManager *) localDbService->getManager(LocalDBService::USER);

        if (!userManager->getUser()->isActivated()){
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
        DirectoryService * directoryService = (DirectoryService *)this->getService(ZeminiService::FileSystem);
        directoryService->start();
    }
    //initializeFSWatcher();
}

/***    this opens the windows' explorer on the zemini folder   ***/
void MainController::showDirectory()
{
    QString dirPath = Parameters::storageDirectory;
    QDesktopServices::openUrl(QUrl::fromLocalFile(dirPath.replace("\\","/")));
}


/***    this manages events related to the activation of the system tray entrie ***/
void MainController::manageActivation(QSystemTrayIcon::ActivationReason reason)
{
    trayIcon->contextMenu()->move(QCursor::pos());
    trayIcon->contextMenu()->show();
    /*
    switch(reason){
    case QSystemTrayIcon::Context:{
        trayIcon->contextMenu()->show();
        break;
    }
    }
    */
}

/***    this shows up the preference form on the screen ***/
void MainController::showPreferences()
{
    if (zeminiPreferencesForm == NULL)
    {
        //zeminiPreferencesForm = new ZeminiPreferencesForm(&listEntitiesManager,0,&localDb);
        //zeminiPreferencesForm->show();
    }
    else
        zeminiPreferencesForm->setVisible(true);

    QWidget::connect(zeminiPreferencesForm, SIGNAL(setupDirectory(QStringList)),this, SLOT(addFileSystemWatcher(QStringList)));
}

/***    this launches a web navigator to the zemini web site url    ***/
void MainController::showZeminiWebSite()
{
    QDesktopServices::openUrl(QUrl(Parameters::webSite));
}

/***            Once, the user installs the sofware, at the first launch he must complete the pocedure by supplying some informations           ***/
void MainController::completeInstallation()
{
    ((NetworkService *) this->getService(ZeminiService::Network))->getInitialDbData();
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
    QString dirPath = Parameters::storageDirectory;
    if (!QDir(dirPath.replace("\\","/")).exists())
        return false;

    return true;
}

void MainController::insertInitDbData(QList<Category> * categories)
{
    if (categories != NULL){
        for (int i = 0; i < categories->size(); i++){
            Category category = categories->at(i);
            if (localDbService->getCategoryManager()->getByName(category.getName()) != NULL){
                if (!localDbService->getCategoryManager()->addCategory(categories->at(i)))
                    qDebug() << "category " << category.getName() <<" not inserted : " << endl;
                else
                    qDebug() << "category " << category.getName() <<" inserted : " << endl;
            }
        }
    }
}

bool MainController::displayTrayIcon()
{
    //building the context menu
    QAction * openFolder = new QAction(QIcon(),tr("Open Zemini folder"),this);
    QWidget::connect(openFolder, SIGNAL(triggered()),this, SLOT(showDirectory()));
    QAction * openPreferences = new QAction(QIcon(),tr("Preferences..."),this);
    QWidget::connect(openPreferences, SIGNAL(triggered()), this , SLOT(showPreferences()));
    QAction * quitZemini = new QAction(QIcon(),tr("Quit Zemini"),this);
    QWidget::connect(quitZemini, SIGNAL(triggered()), this , SLOT(close()));
    QAction * openZeminiweb = new QAction(QIcon(),"Go to Zemini Web",this);
    QWidget::connect(openZeminiweb, SIGNAL(triggered()), this , SLOT(showZeminiWebSite()));

    QMenu * contextMenu = new QMenu();
    contextMenu->addAction(openFolder);
    contextMenu->addAction(contextMenu->addSeparator());
    contextMenu->addAction(openPreferences);
    contextMenu->addAction(openZeminiweb);
    contextMenu->addAction(contextMenu->addSeparator());
    contextMenu->addAction(quitZemini);

    //Setting the context menu to the system tray entrie
    trayIcon->setContextMenu(contextMenu);
    trayIcon->setIcon(QIcon(Parameters::iconLocation));
    trayIcon->setToolTip("Zemini "+Parameters::appVersion +" \nConnection...");
    trayIcon->show();
}

ZeminiService * MainController::getService(QString service)
{
    return this->serviceContainer->getService(service);
}
