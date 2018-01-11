#include "Controllers\MainController.h"

/*** Default constructor    ***/
MainController::MainController()
{

    // Connection to the local database
    DatabaseManager dbManager;
    localDb=dbManager.connectToDatabase("ZeminiDB", "root","localhost", 3306, "","QSQLITE", "db");


    //Initialization of class variables
    //---   initialization of entities' managers
    /*
    listEntitiesManager << (*categoryManager);
    listEntitiesManager << (*typeManager);
    listEntitiesManager << (*dirManager);
    listEntitiesManager << (*fileManager);
    listEntitiesManager << (*userManager);
    */

    // Services initialization
    //dirService =  new DirectoryService(this);
    networkService = new NetworkService(this);
    localDbService = new LocalDBService(localDb,this);

    // here's the code for initialization of LocalDBService object
    //localDbService->setEntitiesManagers(dirManager, fileManager);

    //---   Initialization of all the forms
    //zeminiPreferencesForm = NULL;
    registerForm = new RegisterForm(0,networkService);
    logInForm = new LogInForm();

    /*
    trayIcon = new QSystemTrayIcon();

    //Setting connections with the child threads
    QWidget::connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(showDirectory()));
    QWidget::connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this, SLOT(manageActivation(QSystemTrayIcon::ActivationReason)));
    QWidget::connect(registerForm, SIGNAL(userDataEntered(User*)),this,SLOT(handleUserData1(User*)));
    QWidget::connect(userManager, SIGNAL(replyUserReceived(QNetworkReply*)),this,SLOT(handleReplyUserReceived(QNetworkReply*)));
    QWidget::connect(userManager, SIGNAL(criticalError()),this,SLOT(close()));
    //QWidget::connect(netService, SIGNAL(pingReply(bool)),userManager,SLOT(handleReplyPingFinished(bool)));
    QWidget::connect(registerForm,SIGNAL(logInLinkActivated()),this,SLOT(showLogInForm()));
    QWidget::connect(userManager, SIGNAL(replyUserDownload(QNetworkReply*)),this,SLOT(handleUserDataDownloaded(QNetworkReply*)));
    QWidget::connect(dirService, SIGNAL(directoryChecked(QFileInfoList *)), this, SLOT(updateLocalDatabase(QFileInfoList *)));
    QWidget::connect(localDbService,SIGNAL(dataSavedLocally()),this, SLOT(updateRemoteDatabase()));
    QWidget::connect(dirManager, SIGNAL(remoteUpdatesDone(int)),this, SLOT(handleManagerEndofProcessing(int)));
    QWidget::connect(fileManager, SIGNAL(remoteUpdatesDone(int)),this, SLOT(handleManagerEndofProcessing(int)));
    QWidget::connect(logInForm, SIGNAL(signUpLinkActivated()), this, SLOT(showRegisterForm()));

    //building the context menu
    QAction * openFolder = new QAction(QIcon(),tr("Open Zemini folder"),this);
    QWidget::connect(openFolder, SIGNAL(triggered()),this, SLOT(showDirectory()));

    QAction * openPreferences = new QAction(QIcon(),tr("Preferences..."),this);
    QWidget::connect(openPreferences, SIGNAL(triggered()), this , SLOT(showPreferences()));

    QAction * quitZemini = new QAction(QIcon(),tr("Quitter Zemini"),this);
    QWidget::connect(quitZemini, SIGNAL(triggered()), this , SLOT(close()));

    QAction * openZeminiweb = new QAction(QIcon(),"Visiter " + Parameters::webSite(),this);
    QWidget::connect(openZeminiweb, SIGNAL(triggered()), this , SLOT(showZeminiWebSite()));

    contextMenu = new QMenu();
    contextMenu->addAction(openFolder);

    contextMenu->addAction(contextMenu->addSeparator());
    contextMenu->addAction(openPreferences);
    contextMenu->addAction(openZeminiweb);

    contextMenu->addAction(contextMenu->addSeparator());
    contextMenu->addAction(quitZemini);

    //Setting the context menu to the system tray entrie
    trayIcon->setContextMenu(contextMenu);
    trayIcon->setIcon(QIcon(Parameters::iconLocation()));
    trayIcon->setToolTip("Zemini "+Parameters::appVersion() +" \nConnection...");
    */
}

/***    function that creates a the link to the directory in order to make it as favorite one if it doesn't exist yet            ***/
void MainController::make_link()
{
    QString dirPath= QDir().homePath()+"/Zemini";
    dirPath.replace("\\","/");
    QString linkPath = QDir().homePath()+"/Links/Zemini.lnk";
    linkPath.replace("\\","/");
    //QMessageBox::information(0,"folder", linkPath +" "+ dirPath);
    QFile dir (dirPath);
    bool link_created = dir.link(linkPath);
    if (!link_created)
        qDebug("Failed to create the link");
}

void MainController::start()
{
    if (!installationCompleted())
        // complete zemini installation by setting user data and other parameters
        completeInstallation();

    if (!localDbService->getUserManager()->getUser()->isActivated())
    // starting zemini services
    localDbService->start();
    networkService->start();

    /*
    Parameters::make_directories(Parameters::storageDirectory());   //this creates the directory

    make_link();    //this creates a link to the directory

    trayIcon->show();   //showing on the screen the application icon

    //Starting the directoryService object
    dirService->start();

    initializeFSWatcher();
    */
}

/***    this opens the windows' explorer on the zemini folder   ***/
void MainController::showDirectory()
{
    QString dirPath = Parameters::storageDirectory().replace("\\","/");
    QDesktopServices::openUrl(QUrl::fromLocalFile(dirPath));
}


/***    this manages events related to the activation of the system tray entrie ***/
void MainController::manageActivation(QSystemTrayIcon::ActivationReason reason)
{
    if (reason = QSystemTrayIcon::Context)  //if the context menu was requested then
    {
        //qDebug()<< "tray entrie activated"<<endl;
        trayIcon->contextMenu()->show();
    }
}

/***    this shows up the preference form on the screen ***/
void MainController::showPreferences()
{
    if (zeminiPreferencesForm == NULL)
    {
        zeminiPreferencesForm = new ZeminiPreferencesForm(&listEntitiesManager,0,&localDb);
        zeminiPreferencesForm->show();
    }
    else
        zeminiPreferencesForm->setVisible(true);

    QWidget::connect(zeminiPreferencesForm, SIGNAL(setupDirectory(QStringList)),this, SLOT(addFileSystemWatcher(QStringList)));
}

/***    this launches a web navigator to the zemini web site url    ***/
void MainController::showZeminiWebSite()
{
    QDesktopServices::openUrl(QUrl(Parameters::webSite()));
}

/***            Once, the user installs the sofware, at the first launch he must complete the pocedure by supplying some informations           ***/
void MainController::completeInstallation()
{
    networkService->initLocalDb();
    registerForm->show();
}

void MainController::handleReplyUserReceived(QNetworkReply * net_reply)
{
    qDebug()<<"Yes Utilisateur";
    qDebug()<<net_reply->readAll();
    {
        QString title = "Zemini Info";
        QString message = "Votre inscription a bien été enregistrée!";
        trayIcon->showMessage(title, message);
        //zeminiValidationAccountForm->show();
    }
}

/***            This function is trigerred when the back button of zeminiAccountForm is pressed to come back to The welcome page            ***/
void MainController::backToWelcomeForm()
{
    //zeminiAccountForm->setVisible(false);
    registerForm->setVisible(true);
}

void MainController::handleUserData1(User * m_user)
{
    this->m_user = m_user;
    //zeminiAccountForm->setUserData(m_user);
    //zeminiAccountForm->show();
}


void MainController::handleUserData2(User * user)
{
    this->m_user = user;
    if (!userManager->isConnected())
    {
        int response = QMessageBox::information(0,"Message", "It seems that you are not connected to internet! Then do you want continue anyway ?, your subscription will be achieved during your next connection. ", QMessageBox::Yes, QMessageBox::No);
        if (response == QMessageBox::Yes)
        {
            //i close all previous forms i showed
            //zeminiAccountForm->hide();
            registerForm->hide();

            //then i try to enroll the user into the application
            if (userManager->saveUserLocally(*m_user))
            {
                QString title = "Zemini Info";
                QString message = "Votre inscription a bien été enregistrée!";
                trayIcon->showMessage(title, message);
            }
        }
    }
    else
    {
        //i close all previous forms i showed
        //zeminiAccountForm->hide();
        registerForm->hide();

        userManager->saveUser(*m_user);
    }
}

/*
void MainController::saveActivationKey(QString key)
{
    if (m_user->getActivationkey() == key)
    {
        //zeminiValidationAccountForm->setVisible(false);

        QString title = "Zemini Info";
        QString message = "Votre clé d'activation a bien été enregistrée!";
        trayIcon->showMessage(title, message);

        m_user->setActivated(true);;

        userManager->updateUser(*m_user);
    }
    else
    {
        QMessageBox::warning(0,"Message","The activation key i've given is false! Please! give another one.");
    }
}
*/

void MainController::showLogInForm()
{
    registerForm->hide();
    logInForm->show();
}

void MainController::handleRecoveryData(User * user)
{
    //first, i test the internet connectivity
    if (!userManager->isConnected())
    {
        QMessageBox::warning(0,"Message","It seems that you're not connected to internet! Please check first if internet is available and then start again.");
    }
    else
    {
        this->m_user = user;
        userManager->downloadUser(user);
    }
}

void MainController::handleUserDataDownloaded(QNetworkReply * reply)
{
    //qDebug() << reply->readAll();

    if(reply->error() == QNetworkReply::NoError) {

        QString strReply = (QString)reply->readAll();

        //qDebug() << strReply;

        QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());

        QJsonObject jsonObject = jsonResponse.object();
        /*
        QJsonArray jsonArray = jsonObject["status"].toArray();

        qDebug() << jsonObject["status"].toString();

        foreach (const QJsonValue & value, jsonArray)
        {
            QJsonObject obj = value.toObject();
            qDebug() << value.toString();
        }
        */

        //i want to make a second check on the validity of the values returned
        if (jsonObject["res"].toString() == "false")
        {
            QMessageBox::warning(0,"Message", "Oops! it seems like data you've entered are wrong. Please, you're invited to try again");
            return ;
        }
        else
        {

            // we hide the current form and save data received
            logInForm->setVisible(false);

            QString familyname = jsonObject["familyname"].toString();

            QString firstname = jsonObject["firstname"].toString();

            QDateTime dateinscription = QDateTime::fromString(jsonObject["dateinscription"].toString(),Parameters::timeFormat());

            QString id = jsonObject["id"].toString();

            QString username = jsonObject["username"].toString();

            QString phonenumber = jsonObject["phonenumber"].toString();

            User * user = new User(id.toInt(),familyname, firstname, m_user->getEmail(), username, m_user->getPassword());
            //user->setId(id.toInt());
            user->setPhonenumber(phonenumber);
            user->setDateInscription(dateinscription);
            user->setActivated(true);

            qDebug()<<"--- Recovery process ---";
            user->toString();

            this->m_user = user;

            if (userManager->saveUserLocally(*user))
            {
                QString title = "Zemini Info";
                QString message = "Welcome to Zemini and thank you for joining us! ";
                trayIcon->showMessage(title, message);

                // and then i start the service
                this->start();
            }
        }
    } else {
        qDebug() << "ERROR";
    }
    delete reply;
}

/***    This function update the data of both databases, the remote and the local databases    ***/
void MainController::updateLocalDatabase(QFileInfoList * fileInfoList)
{
    localDbService->setArguments(fileInfoList);
    localDbService->start();
}

void MainController::updateRemoteDatabase()
{
    // here, i update the remote database
    if (dirManager->isConnected())
    {
        dirManager->saveUpdatesToRemoteDB();
        fileManager->saveUpdatesToRemoteDB();
    }
}

void MainController::handleEndofRequest(QNetworkReply *)
{

}

void MainController::handleDirectoryChanges(QString path)
{
    qDebug() << "A directory change occured in directory : " + path;

    dirService->setPath(path);
    dirService->start();
}

void MainController::initializeFSWatcher()
{
    // here's the initialization of the QFileSystemWatcher object
    QStringList dirsPathList;

    QList<Directory> dirsList = dirManager->getAllDBDirs();

    if (dirsList.size() > 0)
    {
        if (dirsList.size()<=256)
        {
            for (int i = 0;i<dirsList.size();i++)
            {
                Directory dir = dirsList.at(i);
                dirsPathList << dir.getPath();
            }

            fsWatcher = new QFileSystemWatcher(dirsPathList,this);
            fsWatchersList.append(fsWatcher);
            QWidget::connect(fsWatcher, SIGNAL(directoryChanged(QString)),this, SLOT(handleDirectoryChanges(QString)));
        }
        else
        {
            for (int i = 0;i<dirsList.size();i++)
            {
                Directory dir = dirsList.at(i);
                dirsPathList << dir.getPath();

                if (i%255 == 0)
                {
                    fsWatcher = new QFileSystemWatcher(dirsPathList,this);
                    fsWatchersList.append(fsWatcher);
                    QWidget::connect(fsWatcher, SIGNAL(directoryChanged(QString)),this, SLOT(handleDirectoryChanges(QString)));
                    dirsPathList.clear();
                }
            }
        }
    }
    else
    {
        dirsPathList << Parameters::storageDirectory();
        dirsPathList << Parameters::seriesDirectory();
        dirsPathList << Parameters::softwaresDirectory();
        dirsPathList << Parameters::moviesDirectory();

        fsWatcher = new QFileSystemWatcher(dirsPathList,this);
        fsWatchersList.append(fsWatcher);
        QWidget::connect(fsWatcher, SIGNAL(directoryChanged(QString)),this, SLOT(handleDirectoryChanges(QString)));
    }
}

void MainController::handleManagerEndofProcessing(int idmanager)
{
    switch (idmanager) {
    case 3:     // this means that directorymanager's already finished processing
        dirManager->saveUpdatesToRemoteDB();
        break;
    case 4:     // this means that filemanager's already finished processing
        fileManager->saveUpdatesToRemoteDB();
        break;
    default:
        break;
    }
}

void MainController::addFileSystemWatcher(QStringList stringList)
{

    for (int i = 0 ; i< stringList.size();i++)
        qDebug() << stringList.at(i);

    Parameters::make_directories(stringList.at(0));

    QFileInfo * fileInfo;
    fileInfo = new QFileInfo(stringList.at(0));
    dirManager->saveDirectoryLocally(fileInfo);

    fileInfo = new QFileInfo(stringList.at(1));
    dirManager->saveDirectoryLocally(fileInfo);

    fileInfo = new QFileInfo(stringList.at(2));
    dirManager->saveDirectoryLocally(fileInfo);

    fileInfo = new QFileInfo(stringList.at(3));
    dirManager->saveDirectoryLocally(fileInfo);

    fileInfo = new QFileInfo(stringList.at(4));
    dirManager->saveDirectoryLocally(fileInfo);

    // i initialize the QFileSystemWatcher on the directory newly created
    QFileSystemWatcher * fsWatcher = new QFileSystemWatcher(stringList, this);
    QWidget::connect(fsWatcher,SIGNAL(directoryChanged(QString)),this,SLOT(handleDirectoryChanges(QString)));
    fsWatchersList.append(fsWatcher);
}

void MainController::addFileSystemWatcher(QFileSystemWatcher *fsWatcher)
{
    fsWatchersList.append(fsWatcher);
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
    if (localDbService->isDbEmpty())
        return false;

    // Second, the zemini directory status
    if (!QDir(Parameters::storageDirectory().replace("\\","/")).exists())
        return false;

    return true;
}
